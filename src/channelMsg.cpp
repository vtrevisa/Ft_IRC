#include "Server.hpp"

std::string parseChannelMSG(std::vector<std::string> string) {
	std::string message;
	message = string[1].substr(0) + " ";
	for (size_t i = 2; i < string.size(); i++)
		message += string[i] + " ";

	return message;
}

void Server::channelMsg(std::vector<std::string> string, int fd) {
	std::string response;

	//verifica se os parametros estão vazios
	if (string.size() == 0 || string[0] == "" || string[0] == "/channel" || string.size() < 2) {
		response = std::string(RED) + "Invalid command\r\nUsage: /channel <channel name> <message>\r\n" + std::string(WHITE);
		send(fd, response.c_str(), response.size(), 0);
		return;
	}

	//verifica se o client escreveu uma mensagem
	if (string[1] == "") {
		std::cout << RED << "Error while sending message..." << WHITE << std::endl;
		response = std::string(RED) + "You must write a message\r\n" + std::string(WHITE);
		send(fd, response.c_str(), response.size(), 0);
		return;
	}

	Client* client = getClientByFD(fd);
	Channel* channel = getChannel(string[0]);

	//verifica se o canal existe
	if (channel == NULL) {
		std::cout << RED << "Error while sending message..." << WHITE << std::endl;
		response = std::string(RED) + "Channel does not exist\r\n" + std::string(WHITE);
		send(fd, response.c_str(), response.size(), 0);
		return;
	}

	//verifica se o client que chamou o comando está no canal
	if (!channel->isOnChannel(client->getNickname())) {
		std::cout << RED << "Error while sending message..." << WHITE << std::endl;
		response = std::string(RED) + "You must be on the channel to send messages\r\n" + std::string(WHITE);
		send(fd, response.c_str(), response.size(), 0);
		return;
	}

	std::string message = parseChannelMSG(string);
	std::cout << YELLOW << "Sending message to channel..." << WHITE << std::endl;
	std::vector<Client *> clients = getAllClients();
	for (size_t i = 0; i < clients.size(); i++) {
		response = std::string(YELLOW) + "#" + channel->getName() + " " + client->getNickname() + ": " + message + "\r\n" + std::string(WHITE);
		send(clients[i]->getFd(), response.c_str(), response.size(), 0);
	}
}