#include "Server.hpp"

void Server::channelMsg(std::vector<std::string> string, int fd) {
	std::string response;

	//verifica se os parametros estão vazios
	if (string.size() == 0 || string[0] == "" || string.size() != 2) {
		response = std::string(RED) + "Invalid number of arguments\r\nUsage: /channel <channel name> <message in double quotes>\r\n";
		send(fd, response.c_str(), response.size(), 0);
		return;
	}

	//verifica se o client escreveu uma mensagem
	if (string[1] == "") {
		response = std::string(RED) + "You must write a message\r\n";
		send(fd, response.c_str(), response.size(), 0);
		return;
	}

	Client* client = getClientByFD(fd);
	Channel* channel = getChannel(string[0]);

	//verifica se o canal existe
	if (channel == NULL) {
		response = std::string(RED) + "Channel does not exist\r\n";
		send(fd, response.c_str(), response.size(), 0);
		return;
	}

	//verifica se o client que chamou o comando está no canal
	if (!channel->isOnChannel(client->getNickname())) {
		response = std::string(RED) + "You must be on the channel to send messages\r\n";
		send(fd, response.c_str(), response.size(), 0);
		return;
	}

	std::vector<Client *> clients = channel->getAllClients();
	std::stringstream ss;
	ss << client->getFd();
	for (size_t i = 0; i < clients.size(); i++) {
		if (client->getNickname() == "")
			response = std::string(YELLOW) + "#" + channel->getName() + " Client " + ss.str() + ": " + string[1] + "\r\n" + std::string(WHITE);
		else
			response = std::string(YELLOW) + "#" + channel->getName() + " " + client->getNickname() + ": " + string[1] + "\r\n" + std::string(WHITE);
		send(clients[i]->getFd(), response.c_str(), response.size(), 0);
	}
}