#include "../includes/Server.hpp"

void Server::kick(std::vector<std::string> string, int fd) {
	std::string response;

	//verifica se os parametros estão vazios
	if (string.size() == 0 || string[0] == "" || string[0] == "/kick" || string.size() > 3) {
		response = std::string(RED) + "Invalid command\r\n" +
				   "Usage: /kick <client to be kicked> <channel name> (optional)<reason>\r\n"
				   + std::string(WHITE);
		send(fd, response.c_str(), response.size(), 0);
		return;
	}

	Client* client = getClientByFD(fd);
	Channel* channel = getChannel(string[1]);

	//verifica se o canal existe
	if (channel == NULL) {
		std::cout << RED << "Error while kicking client from channel..." << WHITE << std::endl;
		response = std::string(RED) + "Channel does not exist\r\n" + std::string(WHITE);
		send(fd, response.c_str(), response.size(), 0);
		return;
	}

	//verifica se o client que chamou o comando está no canal
	if (!channel->isOnChannel(client->getNickname())) {
		std::cout << RED << "Error while kicking client from channel..." << WHITE << std::endl;
		response = std::string(RED) + "You must be on the channel to use kick command\r\n" + std::string(WHITE);
		send(fd, response.c_str(), response.size(), 0);
		return;
	}

	//verifica se o client que chamou o comando é operador no canal
	if (!channel->isOperator(client->getNickname())) {
		std::cout << RED << "Error while kicking client from channel..." << WHITE << std::endl;
		response = std::string(RED) + "You don't have operator privileges on this channel\r\n" + std::string(WHITE);
		send(fd, response.c_str(), response.size(), 0);
		return;
	}

	//verifica se o client a ser expulso existe
	Client* kickedClient = getClientByNick(string[0]);
	if (kickedClient == NULL) {
		std::cout << RED << "Error while kicking client from channel..." << WHITE << std::endl;
		response = std::string(RED) + "This client does not exist\r\n" + std::string(WHITE);
		send(fd, response.c_str(), response.size(), 0);
		return;
	}

	//verifica se o client a ser expulso esta no canal
	if (!channel->isOnChannel(kickedClient->getNickname())) {
		std::cout << RED << "Error while kicking client from channel..." << WHITE << std::endl;
		response = std::string(RED) + "This client is not on this channel\r\n" + std::string(WHITE);
		send(fd, response.c_str(), response.size(), 0);
		return;
	}

	//verifica se existe uma razão para a expulsão
	std::string reason;
	if (string.size() == 3)
		reason = string[2].substr(1);
	else
		reason = "no reason";

	// int kickedClientFD = kickedClient->getFd();
	// response = std::string(YELLOW) + "You were kicked from the channel " + channel->getName() + "\r\nReason: " + reason + "\r\n" + std::string(WHITE);
	// send(kickedClientFD, response.c_str(), response.size(), 0);

	std::vector<Client *> clients = channel->getAllClients();
	response = std::string(YELLOW) + "#" + channel->getName() + ": " + client->getNickname() + " has been kicked from this channel\r\nReason: " + reason + "\r\n" + std::string(WHITE);
	for (size_t i = 0; i < clients.size(); i++)
		send(clients[i]->getFd(), response.c_str(), response.size(), 0);

	std::cout << YELLOW << "Kicking client from channel..." << WHITE << std::endl;
	channel->removeClient(kickedClient->getNickname());
}