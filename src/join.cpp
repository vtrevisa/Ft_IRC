#include "../includes/Server.hpp"

void Server::join(std::vector<std::string> string, int fd) {
	std::string response;

	if (string.size() == 0 || string[0] == "" || string.size() > 2) {
		response = std::string(RED) + 
				   "Invalid number of arguments\r\nUsage: /join <channel name> (optional)<password>\r\n"
				   + std::string(WHITE);
		send(fd, response.c_str(), response.size(), 0);
		return;
	}

	Client* client = getClientByFD(fd);
	Channel* channel = getChannel(string[0]);
	const std::string& channelName = string[0];
	bool owner = false;
	//verifica se o canal existe e se não existir, cria
	if (channel == NULL) {
		createChannel(channelName);
		channel = getChannel(string[0]);
		owner = true;
	}

	if (channel->getMode("i") == true) {
		if (channel->isInvited(client->getNickname()) || channel->isOnChannel(client->getNickname())) {
			if (channel->isInvited(client->getNickname()))
				channel->removeFromInviteList(client->getNickname());
		} else {
			response = std::string(RED) + "You have not been invited for this channel\r\n" + std::string(WHITE);
			send(fd, response.c_str(), response.size(), 0);
			return;
		}
	}

	std::string channelPassword = channel->getPassword();
	//verifica se o canal possui senha
	if (!channelPassword.empty() && string.size() == 1) {
		response = std::string(RED) + "You must provide a password to join this channel\r\n" + std::string(WHITE);
		send(fd, response.c_str(), response.size(), 0);
		return;
	}

	std::string password = string[1];
	//verifica se a senha é a correta
	if (!channelPassword.empty() && channelPassword != password) {
		response = std::string(RED) + "Wrong password\r\n" + std::string(WHITE);
		send(fd, response.c_str(), response.size(), 0);
		return;
	}

	//verifica se o canal está cheio
	if(channel->getClientCount() < channel->getLimit()) {
		channel->addClient(client);
	} else {
		response = std::string(RED) + "You cannot join this channel\r\nThis channel has reached the client limit\r\n" 
				   + std::string(WHITE);
		send(fd, response.c_str(), response.size(), 0);
		return;
	}

	if (owner)
		channel->promoteToOperator(client->getNickname());

	std::vector<Client *> clients = channel->getAllClients();
	response = std::string(YELLOW) + "#" + channel->getName() + ": " +
			   client->getNickname() + " has joined this channel\r\n" +
			   std::string(WHITE);
	for (size_t i = 0; i < clients.size(); i++)
		send(clients[i]->getFd(), response.c_str(), response.size(), 0);
}