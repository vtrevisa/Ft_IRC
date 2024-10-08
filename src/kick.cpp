#include "Server.hpp"

void Server::kick(std::vector<std::string> string, int fd) {
	std::string response;

	//verifica se os parametros estão vazios
	if (string.size() == 0 || string[0] == "" || string.size() > 3) {
		response = "Invalid number of arguments\r\nUsage: /kick <client to be kicked> <channel name> (optional)<reason in double quotes>\r\n";
		send(fd, response.c_str(), response.size(), 0);
		return;
	}

	Client* client = getClientByFD(fd);
	Channel* channel = getChannel(string[1]);
	const std::string& channelName = string[1];

	//verifica se o canal existe
	if (channel == NULL) {
		response = "Channel does not exist\r\n";
		send(fd, response.c_str(), response.size(), 0);
		return;
	}

	//verifica se o client que chamou o comando está no canal
	if (!channel->isOnChannel(client->getNickname())) {
		response = "You must be on the channel to use kick command\r\n";
		send(fd, response.c_str(), response.size(), 0);
		return;
	}

	//verifica se o client que chamou o comando é operador no canal
	if (!channel->isOperator(client->getNickname())) {
		response = "You don't have operator privileges on this channel\r\n";
		send(fd, response.c_str(), response.size(), 0);
		return;
	}

	//verifica se o client a ser expulso existe
	Client* kickedClient = getClientByNick(string[0]);
	if (kickedClient == NULL) {
		response = "This client does not exist\r\n";
		send(fd, response.c_str(), response.size(), 0);
		return;
	}

	//verifica se o client a ser expulso esta no canal
	if (!channel->isOnChannel(kickedClient->getNickname())) {
		response = "This client is not on this channel\r\n";
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
	response = "You kicked " + kickedClient->getNickname() + " from " + channelName + "\r\n";
	send(fd, response.c_str(), response.size(), 0);

	int kickedClientFD = kickedClient->getFd();
	response = "You were kicked from the channel " + channelName + "\r\nReason: " + reason + "\r\n";
	send(kickedClientFD, response.c_str(), response.size(), 0);

	std::vector<Client *> clients = channel->getAllClients();
	response = client->getNickname() + " has been kicked from this channel\r\nReason: " + reason + "\r\n";
	for (size_t i = 0; i < clients.size(); i++)
		send(clients[i]->getFd(), response.c_str(), response.size(), 0);
}