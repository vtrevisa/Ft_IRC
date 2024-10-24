#include "../includes/Server.hpp"

void Server::pass(std::vector<std::string> string, int fd) {
    Client* client = Server::getClientByFD(fd);
	std::string response;
	
	if(string.size() == 0 || string[0] == "PASS" || string.size() != 1) {
		response = "Invalid\r\nUsage: PASS <password>\r\n";
		send(fd, response.c_str(), response.size(), 0);
		return;
	}

    std::string receivedPassword = string[0];
	if (receivedPassword == this->_passwd) {
		client->setStatus(CONNECTED);
		std::cout << GREEN << "Client <" << client->getFd() << "> Connected!" << WHITE << std::endl;
		std::string response = "Password accepted!\r\n";;
		send(client->getFd(), response.c_str(), response.size(), 0);
		client->setPassword(receivedPassword);
	} else {
		std::cout << RED << "Client <" << client->getFd() << "> Disconnected" << WHITE << std::endl;
		std::string response = "Password incorrect!\r\n";
		send(client->getFd(), response.c_str(), response.size(), 0);
		close(client->getFd());
		ClearClients(client->getFd());
		delete client;
	}
}