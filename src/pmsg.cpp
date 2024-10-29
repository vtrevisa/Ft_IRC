#include "../includes/Server.hpp"
bool isClientInChannel(int clientFd, Channel* channel) {
	if (channel == NULL) {
		return false;
	}
	std::deque<int> clientFds = channel->getFdClientList();
	for (size_t i = 0; i < clientFds.size(); i++) {
		if (clientFds[i] == clientFd) {
			return true;
		}
	}
	return false;
}

std::string parseMessage(std::deque<std::string> string) {
	std::string message;
	if (string[1].find(':') == 0) {
		message = string[1].substr(1) + " ";
		for (size_t i = 2; i < string.size(); i++)
			message += string[i] + " ";
		return message;
	}
	message = string[1].substr(0) + " ";
	for (size_t i = 2; i < string.size(); i++)
		message += string[i] + " ";
	return message;
}

void Server::pmsg(std::deque<std::string> string, int fd) {
	std::string response;
	Client* client = getClientByFD(fd);
	if (client == NULL) {
		std::cout << RED << "Error: Client not found..." << WHITE << std::endl;
		return;
	}
	std::string channelName = string[0].substr(0, string[0].find(' '));

	if (string.size() < 1 || channelName.empty()) {
		std::cout << RED << "Error sending message..." << WHITE << std::endl;
		response = IRC + ERR_NORECIPIENTNBR + client->getClientname() + ERR_NORECIPIENT + END;
		send(fd, response.c_str(), response.size(), 0);
		return;
	}

	if (string.size() < 2 || string[1].empty()) {
		std::cout << RED << "Error sending message..." << WHITE << std::endl;
		response = IRC + ERR_NOTEXTTOSENDNBR + client->getClientname() + ERR_NOTEXTTOSEND + END;
		send(fd, response.c_str(), response.size(), 0);
		return;
	}
	std::string message = parseMessage(string);

	Channel* channel = getChannel(channelName);
	if (channel == NULL && channelName[0] == '#') {
		std::cout << RED << "Error sending message..." << WHITE << std::endl;
		response = IRC + ERR_NOSUCHNICKNBR + channelName + ERR_NOSUCHNICK + END;
		send(fd, response.c_str(), response.size(), 0);
		return;
	} else if (channelName[0] != '#') {
		Client* toReach = getClientByNick(channelName);
		if (toReach == NULL) {
			std::cout << RED << "Error sending message..." << WHITE << std::endl;
			response = IRC + ERR_NOSUCHNICKNBR + channelName + ERR_NOSUCHNICK + END;
			send(fd, response.c_str(), response.size(), 0);
			return;
		}
		std::cout << YELLOW << "Sending message..." << WHITE << std::endl;
		response = ":" + client->getNickname() + " PRIVMSG " + toReach->getNickname() + " :" + message + END;
		send(toReach->getFd(), response.c_str(), response.size(), 0);
		return;
	}

	if (!isClientInChannel(fd, channel)) {
		std::cout << RED << "Error sending message..." << WHITE << std::endl;
		response = IRC + ERR_CANNOTSENDTOCHAN + channelName + " :Cannot send to channel" + END;
		send(fd, response.c_str(), response.size(), 0);
		return;
	}

	std::cout << YELLOW << "Sending message..." << WHITE << std::endl;
	response = ":" + client->getNickname() + " PRIVMSG " + channelName + " :" + message + END;
	channel->removeDuplicateClientsByFD();
	std::deque<Client*> clients = channel->getAllClients();
	for (size_t i = 0; i < clients.size(); i++)
	{
		if (clients[i]->getFd() != fd)
			send(clients[i]->getFd(), response.c_str(), response.size(), 0);
	}
}