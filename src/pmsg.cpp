#include "../includes/Server.hpp"

void Server::pmsg(std::vector<std::string> string, int fd) {
	std::string response;
	Client* client = getClientByFD(fd);

	if (string.size() == 0 || string[0] == "" || string.size() != 2) {
		response = std::string(RED) +
				   "Invalid number of arguments\r\nUsage: /psmg <client to send message> <message in double quotes>\r\n"
				   + std::string(WHITE);
		send(fd, response.c_str(), response.size(), 0);
		return;
	}

	if (string[1] == "") {
		response = std::string(RED) + "The message must not be empty\r\n" + std::string(WHITE);
		send(fd, response.c_str(), response.size(), 0);
		return;
	}

	Client* clientToReach = getClientByNick(string[0]);
	if (clientToReach == NULL) {
		response = std::string(RED) + "This client does not exist\r\n" + std::string(WHITE);
		send(fd, response.c_str(), response.size(), 0);
		return;
	}

	response = std::string(BLUE) + "Private message from " +
			   client->getNickname() + ": " + string[1] + "\r\n" + std::string(WHITE);
	send(clientToReach->getFd(), response.c_str(), response.size(), 0);
}