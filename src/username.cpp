#include "../includes/Server.hpp"

void Server::username(std::vector<std::string> string, int fd) {
	Client* client = Server::getClientByFD(fd);
	if(client == NULL)
		return;
	std::vector<Client*> clients = getAllClients();
	std::string response;
	std::string username = string[0].substr(0, string[0].find(' '));

	for (std::vector<Client*>::iterator it = clients.begin(); it != clients.end(); ++it) {
		if ((*it)->getClientname() == username) {
			std::cout << RED << "Error while setting username..." << WHITE << std::endl;
			response = "This username is already in use\r\n";
			send(fd, response.c_str(), response.size(), 0);
			return;
		}
	}

	if (!client->getClientname().empty()){
		response = "You have already set a username\r\n";
		std::cout << RED << "Error while setting username..." << WHITE << std::endl;
		send(fd, response.c_str(), response.size(), 0);
		return;
	}
	response = "Your username has been set to: " + username + "\r\n";
	send(fd, response.c_str(), response.size(), 0);
	client->setClientname(username);
	
	if(client->isAuth() == true) {
		std::cout << YELLOW << "Setting username..." << WHITE << std::endl;
		response =	"Welcome to IRCSERV!!\r\n";
		send(fd, response.c_str(), response.size(), 0);
		return;
	}
}