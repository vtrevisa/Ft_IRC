#include "Server.hpp"

void Server::channelMsg(std::vector<std::string> string, int fd) {
	std::string response;

	//verifica se os parametros estão vazios
	if (string.size() == 0 || string[0] == "" || string.size() != 2) {
		response = "Invalid number of arguments\r\nUsage: /channel <channel name> <message in double quotes>\r\n";
		send(fd, response.c_str(), response.size(), 0);
		return;
	}

	//verifica se o client escreveu uma mensagem
	if (string[1] == "") {
		response = "You must write a message\r\n";
		send(fd, response.c_str(), response.size(), 0);
		return;
	}

	Client* client = getClientByFD(fd);
	Channel* channel = getChannel(string[0]);

	//verifica se o canal existe
	if (channel == NULL) {
		response = "Channel does not exist\r\n";
		send(fd, response.c_str(), response.size(), 0);
		return;
	}

	//verifica se o client que chamou o comando está no canal
	if (!channel->isOnChannel(client->getNickname())) {
		response = "You must be on the channel to send messages\r\n";
		send(fd, response.c_str(), response.size(), 0);
		return;
	}

	std::string message = string[1];
	std::vector<Client *> clients = channel->getAllClients();
	for (size_t i = 0; i < clients.size(); i++) {
		response = client->getNickname() + ": " + message + "\r\n";
		send(clients[i]->getFd(), response.c_str(), response.size(), 0);
	}
}