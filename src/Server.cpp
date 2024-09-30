#include "../includes/Server.hpp"

Server::Server() {_SerSocketFd = -1;}
		
Server::~Server() {}

void Server::ServerInit(int port, std::string passwd) {
	this->_Port = port;
	this->_passwd = passwd;
	SerSocket(); //-> create the server socket

	std::cout << GRE << "Server <" << _SerSocketFd << "> Connected" << WHI << std::endl;
	std::cout << "Waiting to accept a connection...\n";

	while (Server::_Signal == false) { //-> run the server until the signal is received

		if((poll(&_fds[0],_fds.size(),-1) == -1) && Server::_Signal == false) //-> wait for an event
			throw(std::runtime_error("poll() faild"));

		for (size_t i = 0; i < _fds.size(); i++){ //-> check all file descriptors
			if (_fds[i].revents & POLLIN) { //-> check if there is data to read
				if (_fds[i].fd == _SerSocketFd)
					AcceptNewClient(); //-> accept new client
				else if (getClientByFD(_fds[i].fd)->getStatus() == UNAUTHORIZED)
					Authentication(getClientByFD(_fds[i].fd)); //-> authenticate the client
				else
					ReceiveNewData(_fds[i].fd); //-> receive new data from a registered client
			}
		}
	}
	CloseFds(); //-> close the file descriptors when the server stops
}

void Server::SerSocket() {
	int en = 1;
	struct sockaddr_in add;
	struct pollfd NewPoll;
	add.sin_family = AF_INET; //-> set the address family to ipv4
	add.sin_addr.s_addr = INADDR_ANY; //-> set the address to any local machine address
	add.sin_port = htons(this->_Port); //-> convert the port to network byte order (big endian)

	#ifdef DEBUG
		std::cout << YEL << std::endl;
		std::cout << "Port: " << _Port << std::endl;
		std::cout << "Password: " << _passwd << std::endl;
		std::cout << "Server IP: " << inet_ntoa(add.sin_addr) << std::endl;
		std::cout << WHI << std::endl;
	#endif

	_SerSocketFd = socket(AF_INET, SOCK_STREAM, 0); //-> create the server socket
	if(_SerSocketFd == -1) //-> check if the socket is created
		throw(std::runtime_error("faild to create socket"));

	if(setsockopt(_SerSocketFd, SOL_SOCKET, SO_REUSEADDR, &en, sizeof(en)) == -1) //-> set the socket option (SO_REUSEADDR) to reuse the address
		throw(std::runtime_error("faild to set option (SO_REUSEADDR) on socket"));
	if (fcntl(_SerSocketFd, F_SETFL, O_NONBLOCK) == -1) //-> set the socket option (O_NONBLOCK) for non-blocking socket
		throw(std::runtime_error("faild to set option (O_NONBLOCK) on socket"));
	if (bind(_SerSocketFd, (struct sockaddr *)&add, sizeof(add)) == -1) //-> bind the socket to the address
		throw(std::runtime_error("faild to bind socket"));
	if (listen(_SerSocketFd, SOMAXCONN) == -1) //-> listen for incoming connections and making the socket a passive socket
		throw(std::runtime_error("listen() faild"));

	NewPoll.fd = _SerSocketFd; //-> add the server socket to the pollfd
	NewPoll.events = POLLIN; //-> set the event to POLLIN for reading data
	NewPoll.revents = 0; //-> set the revents to 0
	_fds.push_back(NewPoll); //-> add the server socket to the pollfd
}

void Server::AcceptNewClient() {
	Client cli; //-> create a new client
	struct sockaddr_in cliadd;
	struct pollfd NewPoll;
	socklen_t len = sizeof(cliadd);

	int incofd = accept(_SerSocketFd, (sockaddr *)&(cliadd), &len); //-> accept the new client
	if (incofd == -1) {
		std::cout << "accept() failed" << std::endl;
		return;
	}

	if (fcntl(incofd, F_SETFL, O_NONBLOCK) == -1) { //-> set the socket option (O_NONBLOCK) for non-blocking socket
		std::cout << "fcntl() failed" << std::endl;
		return;
	}
	
	NewPoll.fd = incofd; //-> add the client socket to the pollfd
	NewPoll.events = POLLIN; //-> set the event to POLLIN for reading data
	NewPoll.revents = 0; //-> set the revents to 0

	cli.SetFd(incofd); //-> set the client file descriptor
	cli.setIpAdd(inet_ntoa((cliadd.sin_addr))); //-> convert the ip address to string and set it
	_clients.push_back(cli); //-> add the client to the vector of clients
	_fds.push_back(NewPoll); //-> add the client socket to the pollfd

	std::cout << YEL << "Client <" << incofd << "> requested connection" << WHI << std::endl; // Accept the client
	std::string passwordRequest = "Please enter the password:\r\n";
	send(incofd, passwordRequest.c_str(), passwordRequest.size(), 0);
}

void Server::Authentication(Client* client)
{
	char buffer[1024];
	memset(buffer, 0, sizeof(buffer));

	ssize_t bytes = recv(client->getFd(), buffer, sizeof(buffer) - 1, 0);
	if (bytes <= 0) {
		std::cerr << "Error receiving password from client" << std::endl;
		std::cout << RED << "Client <" << client->getFd() << "> Disconnected" << WHI << std::endl;
		return;
	}

	std::string receivedPassword(buffer);
	receivedPassword = receivedPassword.substr(0, receivedPassword.find(10));

	if (receivedPassword == this->_passwd) {
		client->setStatus(CONNECTED);
		std::cout << GRE << "Client <" << client->getFd() << "> Connected!" << WHI << std::endl;
		std::string response = "Password accepted!\r\n";
		send(client->getFd(), response.c_str(), response.size(), 0);
	} else {
		std::cout << RED << "Client <" << client->getFd() << "> Disconnected" << WHI << std::endl;
		std::string response = "Password incorrect!\r\n";
		send(client->getFd(), response.c_str(), response.size(), 0);
		close(client->getFd());
		ClearClients(client->getFd());
	}
}

void Server::ReceiveNewData(int fd) {
	char buff[1024]; //-> buffer for the received data
	memset(buff, 0, sizeof(buff)); //-> clear the buffer
	Client* client = getClientByFD(fd);

	ssize_t bytes = recv(fd, buff, 1023, 0); //-> receive the data

	if (bytes <= 0) { //-> check if the client disconnected
		std::cout << RED << "Client <" << fd << "> Disconnected" << WHI << std::endl;
		client->clientBuff.clear();
		ClearClients(fd); //-> clear the client
		close(fd); //-> close the client socket
		return;
	}
	client->clientBuff.append(buff);

	if (client->clientBuff.find("\n") == std::string::npos)
		return;

	if (buff[0] == '/')
		Server::identifyCommand(client->clientBuff, fd);
	else
		std::cout << "Client <" << fd << "> says: " << buff << std::endl;
}

bool Server::_Signal = false; //-> initialize the static boolean
void Server::SignalHandler(int signum) {
	(void)signum;
	std::cout << std::endl << "Signal Received!" << std::endl;
	Server::_Signal = true; //-> set the static boolean to true to stop the server
}

void Server::CloseFds() {
	for(size_t i = 0; i < _clients.size(); i++) { //-> close all the clients
		std::cout << RED << "Client <" << _clients[i].getFd() << "> Disconnected" << WHI << std::endl;
		close(_clients[i].getFd());
	}
	if (_SerSocketFd != -1) { //-> close the server socket
		std::cout << RED << "Server <" << _SerSocketFd << "> Disconnected" << WHI << std::endl;
		close(_SerSocketFd);
	}
}

void Server::ClearClients(int fd) { //-> clear the clients
	for(size_t i = 0; i < _fds.size(); i++) {//-> remove the client from the pollfd
		if (_fds[i].fd == fd) {
			_fds.erase(_fds.begin() + i);
			break;
		}
	}
	for(size_t i = 0; i < _clients.size(); i++) { //-> remove the client from the vector of clients
		if (_clients[i].getFd() == fd) {
			_clients.erase(_clients.begin() + i);
			break;
		}
	}
}

Client* Server::getClientByFD(int fd) {
	for (size_t i = 0; i < _clients.size(); i++)
		if (_clients[i].getFd() == fd)
			return &_clients[i];
	return NULL;
}

Client* Server::getClientByNick(std::string nickName) {
	for (size_t i = 0; i < _clients.size(); i++)
		if (_clients[i].getNickname() == nickName)
			return &_clients[i];
	return NULL;
}

Channel* Server::getChannel(const std::string& channelName) {
    for (std::vector<Channel>::iterator it = _channels.begin(); it != _channels.end(); ++it) {
        if (it->getName() == channelName)
            return &(*it);
	}
	return NULL;
}

bool Server::channelExists(std::string& channelName) {
    for (std::vector<Channel>::const_iterator it = _channels.begin(); it != _channels.end(); ++it)
        if (it->getName() == channelName)
            return true;
    return false;
}

void Server::createChannel(std::string channelName) {
	Channel newChannel(channelName);
	_channels.push_back(newChannel);
}

void Server::deleteChannel(std::string channelName) {
	if (_channels.empty())
		return;

	for (size_t i = 0; i < _channels.size(); i++) {
		if (_channels[i].getName() == channelName) {
			_channels.erase(_channels.begin() + i);
			break;
		}
	}
}

std::vector<std::string> splitstr(std::string string) {
	std::vector<std::string> splittedStr;
	std::string word;
	std::stringstream ss(string);

	while (!ss.eof() && std::getline(ss, word, '\n'))
		splittedStr.push_back(word);

	return splittedStr;
}

void Server::identifyCommand(std::string& string, int fd) {
	std::vector<std::string> splittedStr = splitstr(string);
	Client* client = Server::getClientByFD(fd);
	std::string requests[] = {"mode", "exit", "invite"}; //aqui entra nossa cadeia de comandos possiveis, exemplo {"KICK", "JOIN"}

	do {
		int i = 0;
		std::string command = splittedStr[0].substr(0, splittedStr[0].find_first_of(" "));
		std::cout << "message: " << string << std::endl;
		std::cout << "client: " << fd << std::endl;

		//loop que vai identificar o comando
		for (; i < 3; i++) //substituir XXXX pelo numero de comandos totais descritos acima
			if(command == requests[i])
				break;


		std::string parsedCommand = splittedStr[0].substr(splittedStr[0].find_first_of(" ") + 1);

		//cada comando vai ser um case diferente, então é só chamar o método correspondente em cada caso
		//ex: case 0:
		//kick(parseCommand(parsedCommand), ...); break;
		//case 1:
		//join(parseCommand(parsedCommand), ...); break;
		std::string response;
		switch (i) {
			case 0:
				mode(parseCommand(parsedCommand), fd);
				break;
			case 1:
				response = "Goodbye!\r\n";
				send(fd, response.c_str(), response.size(), 0);
				std::cout << RED << "Client <" << fd << "> Disconnected" << WHI << std::endl;
				close(fd);
				ClearClients(fd);
				break;
			case 2:
				invite(parseCommand(parsedCommand), fd);
				break;
			default:
				unknownCommand(command, fd);
				break;
		}
		splittedStr.erase(splittedStr.begin());
	} while (!splittedStr.empty());

	client->clientBuff.clear();
}

std::vector<std::string> Server::parseCommand(std::string string) {
	std::string word;
	std::stringstream ss(string);
	std::vector<std::string> splittedVector;

	while (std::getline(ss, word, ' ')) {
		if (word.find('\r') != std::string::npos)
			splittedVector.push_back(word.substr(0, word.find('\r')));
		else
			splittedVector.push_back(word);
	}
	if (splittedVector.size() == 0)
		splittedVector.push_back("");

	return splittedVector;
}

void Server::unknownCommand(std::string command, int fd) {
	std::string response = command + ": Unknown command\r\n";
	if(send(fd, response.c_str(), response.size(), 0) == -1)
		std::cerr << "Error sending message" << std::endl;
}