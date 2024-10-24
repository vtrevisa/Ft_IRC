#include "../includes/Server.hpp"

std::string parsePMSG(std::vector<std::string> string) {
	std::string message;
	message = string[1].substr(0) + " ";
	for (size_t i = 2; i < string.size(); i++)
		message += string[i] + " ";

	return message;
}

void Server::pmsg(std::vector<std::string> string, int fd) {
	std::string response;
	Client* client = getClientByFD(fd);

	if (string.size() == 0 || string[0] == "" || string[0] == "/pmsg" || string.size() < 2) {
		response = std::string(RED) +
				   "Invalid command\r\nUsage: /psmg <client to send message> <message>\r\n"
				   + std::string(WHITE);
		send(fd, response.c_str(), response.size(), 0);
		return;
	}

	if (string[1] == "") {
		std::cout << RED << "Error while sending message..." << WHITE << std::endl;
		response = std::string(RED) + "The message must not be empty\r\n" + std::string(WHITE);
		send(fd, response.c_str(), response.size(), 0);
		return;
	}

	Client* clientToReach = getClientByNick(string[0]);
	if (clientToReach == NULL) {
		std::cout << RED << "Error while sending message..." << WHITE << std::endl;
		response = std::string(RED) + "This client does not exist\r\n" + std::string(WHITE);
		send(fd, response.c_str(), response.size(), 0);
		return;
	}

	std::string message = parsePMSG(string);
	response = std::string(BLUE) + "Private message from " +
			   client->getNickname() + ": " + message + "\r\n" + std::string(WHITE);
	send(clientToReach->getFd(), response.c_str(), response.size(), 0);
	std::cout << YELLOW << "Sending message..." << WHITE << std::endl;
}