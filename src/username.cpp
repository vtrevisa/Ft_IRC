#include "../includes/Server.hpp"

void Server::username(std::vector<std::string> string, int fd) {
	std::string response;
	Client* client = Server::getClientByFD(fd);
	//verifica se o cliente existe
	if (client == NULL) {
		response = "Client not found\r\n";
		send(fd, response.c_str(), response.size(), 0);
		return;
	}

	//verifica linha de comando passada
	if (string.size() > 1) {
		response = std::string(RED) + "Too many arguments\r\nUsage: /username (optional)<username>\r\n" + std::string(WHITE);
		send(fd, response.c_str(), response.size(), 0);
		return;
	}

	//verifica se foi passado um username
	if (string.size() == 0 || string[0] == "") {
		if (client->getClientname() == "")
			response = std::string(YELLOW) + "You don't have a username yet\r\n" + std::string(WHITE);
		else
			response = std::string(YELLOW) + "Your username is " + client->getClientname() + "\r\n" + std::string(WHITE);
		send(fd, response.c_str(), response.size(), 0);
		return;
	}

	//verifica se o cliente já possui um username
	if (!client->getClientname().empty()){
		response =	std::string(RED) + "You have already set a username\r\n" + std::string(WHITE);
		send(fd, response.c_str(), response.size(), 0);
		return;
	}

	std::vector<Client*> clients = getAllClients();
	std::string username = string[0];

	//verifica se o username escolhido já está em uso
	for (std::vector<Client*>::iterator it = clients.begin(); it != clients.end(); ++it) {
		if ((*it)->getClientname() == username) {
			response = std::string(RED) + "This username is already in use\r\n" + std::string(WHITE);
			send(fd, response.c_str(), response.size(), 0);
			return;
		}
	}

	client->setClientname(username);
}