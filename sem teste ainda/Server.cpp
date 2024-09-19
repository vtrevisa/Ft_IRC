#include "Server.hpp"

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

	std::cout << GRE << "Client <" << incofd << "> Connected" << WHI << std::endl; // Accept the client
}

void Server::ReceiveNewData(int fd) {
	char buff[1024]; //-> buffer for the received data
	memset(buff, 0, sizeof(buff)); //-> clear the buffer

	ssize_t bytes = recv(fd, buff, 1023, 0); //-> receive the data

	if(bytes <= 0) { //-> check if the client disconnected
		std::cout << RED << "Client <" << fd << "> Disconnected" << WHI << std::endl;
		ClearClients(fd); //-> clear the client
		close(fd); //-> close the client socket
	} else { //-> print the received data
		buff[bytes] = '\0';
		std::cout << YEL << "Client <" << fd << "> Data: " << WHI << buff;
		//here you can add your code to process the received data: parse, check, authenticate, handle the command, etc...
	}
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

Channel* Server::getChannel(const std::string& channelName) {
    for (std::vector<Channel>::iterator it = _channels.begin(); it != _channels.end(); ++it) {
        if (it->getName() == channelName) {
            return &(*it);
        }
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

	for(size_t i = 0; i < _channels.size(); i++) {
		if (_channels[i].getName() == channelName) {
			_channels.erase(_channels.begin() + i);
			break;
		}
	}
}