#include "../includes/Server.hpp"

static bool isValidChannelName(const std::string& channelName) {
    if (channelName.empty() || channelName[0] != '#')
        return false;
    return true;
}

static std::string parseMessage(std::vector<std::string> string) {
	std::string message;
	message = string[1].substr(0) + " ";
	for (size_t i = 2; i < string.size(); i++)
		message += string[i] + " ";

	return message;
}

void Server::pmsg(std::vector<std::string> string, int fd) {
	std::string response;
	Client* client = Server::getClientByFD(fd);
	std::string channelName = string[0].substr(0, string[0].find(' '));

	if (string.size() < 1 || channelName.empty()){
		response = "Invalid recipient to reach\r\n";
		send(fd, response.c_str(), response.size(), 0);
		return;
	}

	if (string.size() < 2 || string[1].empty()) {
		response = "Invalid command\r\nUsage: PRIVMSG <client to send message> <message>\r\n";
		send(fd, response.c_str(), response.size(), 0);
		return;
	}
	std::string message = parseMessage(string);

	Channel* channel = getChannel(channelName);
	if (isValidChannelName(channelName) == false) {
		std::cout << RED << "Error while sending message..." << WHITE << std::endl;
		response = "This channel does not exist\r\n";
		send(fd, response.c_str(), response.size(), 0);
		return;
	} else if (channelName[0] != '#'){
		Client* recipientToReach = getClientByNick(channelName);
		if (recipientToReach == NULL) {
			std::cout << RED << "Error while sending message..." << WHITE << std::endl;
			response = "Client not found\r\n";
			send(fd, response.c_str(), response.size(), 0);
			return;
		}
		std::cout << YELLOW << "Sending message..." << WHITE << std::endl;
		response = client->getNickname() + " to " + recipientToReach->getNickname() + " :" + message + "\r\n";
		send(recipientToReach->getFd(), response.c_str(), response.size(), 0);
		return;
	}

	response = client->getNickname() + " to #" + channelName + " :" + message + "\r\n";
	std::cout << YELLOW << "Sending message..." << WHITE << std::endl;
	std::vector<Client *> clients = channel->getAllClients();
	for (size_t i = 0; i < clients.size(); i++)
		send(clients[i]->getFd(), response.c_str(), response.size(), 0);
}

// void Server::pmsg(std::vector<std::string> string, int fd) {
// 	std::string response;
// 	Client* client = getClientByFD(fd);

// 	if (string.size() == 0 || string[0] == "" || string[0] == "PRIVMSG" || string.size() < 2) {
// 		response = "Invalid command\r\nUsage: PRIVMSG <client to send message> <message>\r\n";
// 		send(fd, response.c_str(), response.size(), 0);
// 		return;
// 	}

// 	if (string[1] == "") {
// 		std::cout << RED << "Error while sending message..." << WHITE << std::endl;
// 		response = "The message must not be empty\r\n";
// 		send(fd, response.c_str(), response.size(), 0);
// 		return;
// 	}

// 	Client* recipientToReach = getClientByNick(string[0]);
// 	if (recipientToReach == NULL) {
// 		std::cout << RED << "Error while sending message..." << WHITE << std::endl;
// 		response = "This client does not exist\r\n";
// 		send(fd, response.c_str(), response.size(), 0);
// 		return;
// 	}

// 	std::string message = parseMessage(string);
// 	response = "Private message from " + client->getNickname() + ": " + message + "\r\n";
// 	send(recipientToReach->getFd(), response.c_str(), response.size(), 0);
// 	std::cout << YELLOW << "Sending message..." << WHITE << std::endl;
// }