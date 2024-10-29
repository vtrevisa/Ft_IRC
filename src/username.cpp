#include "../includes/Server.hpp"

void Server::username(std::deque<std::string> string, int fd) {
	Client* client = getClientByFD(fd);
	if(client == NULL)
		return;
	std::deque<Client*> clients = getAllClients();
	std::string response;
	std::string username = string[0].substr(0, string[0].find(' '));

	for (std::deque<Client*>::iterator it = clients.begin(); it != clients.end(); ++it) {
		if ((*it)->getClientname() == username) {
			std::cout << RED << "Error setting username..." << WHITE << std::endl;
			response = IRC + ERR_ALREADYREGISTEREDNBR + client->getNickname() + ERR_ALREADYREGISTERED + END;
			send(fd, response.c_str(), response.size(), 0);
			return;
		}
	}

	if (!client->getClientname().empty()) {
		std::cout << RED << "Error setting username..." << WHITE << std::endl;
		response =	IRC + ERR_ALREADYREGISTEREDNBR + client->getNickname() + ERR_ALREADYREGISTERED + END;
		send(fd, response.c_str(), response.size(), 0);
		return;
	}

	std::cout << YELLOW << "Setting username..." << WHITE << std::endl;
	client->setClientname(username);
	
	if(client->isAuth() == true) {
		response = IRC + RPL_WELCOMENBR + client->getNickname() + RPL_WELCOME + END;
		send(fd, response.c_str(), response.size(), 0);
		return;
	}
}