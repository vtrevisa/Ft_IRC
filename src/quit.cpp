#include "../includes/Server.hpp"

void Server::quit(std::vector<std::string> string, int fd) {
	std::string response;

	//verifica se os parametros estão vazios
	if (string.size() == 0 || string[0] == "" || string[0] == "/quit" || string.size() > 1) {
		response = std::string(RED) + "Invalid command\r\nUsage: /quit <channel name>\r\n" + std::string(WHITE);
		send(fd, response.c_str(), response.size(), 0);
		return;
	}

	Client* client = getClientByFD(fd);
	Channel* channel = getChannel(string[0]);

	//verifica se o canal existe
	if (channel == NULL) {
		std::cout << RED << "Error while quitting channel..." << WHITE << std::endl;
		response = std::string(RED) + "Channel does not exist\r\n" + std::string(WHITE);
		send(fd, response.c_str(), response.size(), 0);
		return;
	}

	//verifica se o client que chamou o comando está no canal
	if (!channel->isOnChannel(client->getNickname())) {
		std::cout << RED << "Error while quitting channel..." << WHITE << std::endl;
		response = std::string(RED) + "You must be on the channel to use quit command\r\n" + std::string(WHITE);
		send(fd, response.c_str(), response.size(), 0);
		return;
	}

	std::vector<Client *> clients = channel->getAllClients();
	response = std::string(YELLOW) + "#" + channel->getName() +
			   ": " + client->getNickname() + " has quit this channel\r\n"
			   + std::string(WHITE);
	for (size_t i = 0; i < clients.size(); i++)
		send(clients[i]->getFd(), response.c_str(), response.size(), 0);

	std::cout << YELLOW << "Quitting channel..." << WHITE << std::endl;
	channel->removeClient(client->getNickname());
}