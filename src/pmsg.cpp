#include "../includes/Server.hpp"

// static bool isValidChannelName(const std::string& channelName) {
//     if (channelName.empty() || channelName[0] != '#')
//         return false;

//     return true;
// }

static std::string parseMessage(std::vector<std::string> string) {
	std::string message;
	message = string[1].substr(0) + " ";
	for (size_t i = 2; i < string.size(); i++)
		message += string[i] + " ";

	return message;
}

void Server::pmsg(std::vector<std::string> string, int fd) {
	std::string response;
	Client* client = getClientByFD(fd);

	if (string.size() == 0 || string[0] == "" || string[0] == "PRIVMSG" || string.size() < 2) {
		response = "Invalid command\r\nUsage: PRIVMSG <client to send message> <message>\r\n";
		send(fd, response.c_str(), response.size(), 0);
		return;
	}

	if (string[1] == "") {
		std::cout << RED << "Error while sending message..." << WHITE << std::endl;
		response = "The message must not be empty\r\n";
		send(fd, response.c_str(), response.size(), 0);
		return;
	}

	Client* clientToReach = getClientByNick(string[0]);
	if (clientToReach == NULL) {
		std::cout << RED << "Error while sending message..." << WHITE << std::endl;
		response = "This client does not exist\r\n";
		send(fd, response.c_str(), response.size(), 0);
		return;
	}

	std::string message = parseMessage(string);
	response = "Private message from " + client->getNickname() + ": " + message + "\r\n";
	send(clientToReach->getFd(), response.c_str(), response.size(), 0);
	std::cout << YELLOW << "Sending message..." << WHITE << std::endl;
}