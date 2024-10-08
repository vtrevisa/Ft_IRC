#include "Server.hpp"

void Server::invite(std::vector<std::string> string, int fd) {
	std::string response;

	//verifica se os parametros estão vazios
	if (string.size() == 0 || string[0] == "" || string.size() > 2) {
		response = "Invalid number of arguments\r\nUsage: /invite <client to be invited> <channel name>\r\n";
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
		response = "You must be on the channel to use invite command\r\n";
		send(fd, response.c_str(), response.size(), 0);
		return;
	}

	//verifica se o client que chamou o comando é operador no canal
	if (!channel->isOperator(client->getNickname())) {
		response = "You don't have operator privileges on this channel\r\n";
		send(fd, response.c_str(), response.size(), 0);
		return;
	}

	//verifica se o client convidado existe
	Client* invitedClient = getClientByNick(string[0]);
	if (invitedClient == NULL) {
		response = "This client does not exist\r\n";
		send(fd, response.c_str(), response.size(), 0);
		return;
	}

	//verifica se o client convidado já esta no canal
	if (channel->isOnChannel(invitedClient->getNickname())) {
		response = "This client is already on this channel\r\n";
		send(fd, response.c_str(), response.size(), 0);
		return;
	}

	channel->addToInviteList(invitedClient->getNickname());
	response = "Invite sent\r\n";
	send(fd, response.c_str(), response.size(), 0);

	int invitedClientFD = invitedClient->getFd();
	response = client->getNickname() + " sent you an invite to enter the chanel " + channelName + "\r\n";
	send(invitedClientFD, response.c_str(), response.size(), 0);
}