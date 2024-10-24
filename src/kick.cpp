#include "../includes/Server.hpp"

void Server::kick(std::vector<std::string> string, int fd) {
	std::string response;

	//verifica se os parametros estão vazios
	if (string.size() == 0 || string[0] == "" || string[0] == "/kick" || string.size() > 3) {
		response = std::string(RED) + "Invalid number of arguments\r\n" +
				   "Usage: /kick <client to be kicked> <channel name> (optional)<reason in double quotes>\r\n"
				   + std::string(WHITE);
		send(fd, response.c_str(), response.size(), 0);
		return;
	}

	Client* client = getClientByFD(fd);
	Channel* channel = getChannel(string[1]);
	const std::string& channelName = string[1];

	//verifica se o canal existe
	if (channel == NULL) {
		response = std::string(RED) + "Channel does not exist\r\n";
		send(fd, response.c_str(), response.size(), 0);
		return;
	}

	//verifica se o client que chamou o comando está no canal
	if (!channel->isOnChannel(client->getNickname())) {
		response = std::string(RED) + "You must be on the channel to use kick command\r\n";
		send(fd, response.c_str(), response.size(), 0);
		return;
	}

	//verifica se o client que chamou o comando é operador no canal
	if (!channel->isOperator(client->getNickname())) {
		response = std::string(RED) + "You don't have operator privileges on this channel\r\n";
		send(fd, response.c_str(), response.size(), 0);
		return;
	}

	//verifica se o client a ser expulso existe
	Client* kickedClient = getClientByNick(string[0]);
	if (kickedClient == NULL) {
		response = std::string(RED) + "This client does not exist\r\n";
		send(fd, response.c_str(), response.size(), 0);
		return;
	}

	//verifica se o client a ser expulso esta no canal
	if (!channel->isOnChannel(kickedClient->getNickname())) {
		response = std::string(RED) + "This client is not on this channel\r\n";
		send(fd, response.c_str(), response.size(), 0);
		return;
	}

	//verifica se existe uma razão para a expulsão
	std::string reason;
	if (string.size() == 3)
		reason = string[2].substr(1);
	else
		reason = "no reason";

	channel->removeClient(kickedClient->getNickname());

	int kickedClientFD = kickedClient->getFd();
	response = std::string(YELLOW) + "You were kicked from the channel " + channelName + "\r\nReason: " + reason + "\r\n" + std::string(WHITE);
	send(kickedClientFD, response.c_str(), response.size(), 0);

	std::vector<Client *> clients = channel->getAllClients();
	response = std::string(YELLOW) + "#" + channel->getName() + ": " + client->getNickname() + " has been kicked from this channel\r\nReason: " + reason + "\r\n" + std::string(WHITE);
	for (size_t i = 0; i < clients.size(); i++)
		send(clients[i]->getFd(), response.c_str(), response.size(), 0);
}