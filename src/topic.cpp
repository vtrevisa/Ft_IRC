#include "../includes/Server.hpp"

std::string parseTopic(std::vector<std::string> string) {
	std::string message;
	if (string.size() < 2 || string[1] == "") {
		message = "";
		return message;
	}

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

void Server::topic(std::vector<std::string> string, int fd) {
	std::string response;
	if (string.size() == 0 || string[0] == "") {
		std::cout << RED << "Error setting topic channel..." << WHITE << std::endl;
		response = IRC + ERR_NEEDMOREPARAMSNBR + " TOPIC " + ERR_NEEDMOREPARAMS + END;
		send(fd, response.c_str(), response.size(), 0);
		return;
	}
	std::string channelName = string[0];
	Channel* channel = getChannel(channelName);

	if (channel == NULL) {
		std::cout << RED << "Error quitting channel..." << WHITE << std::endl;
		response = IRC + ERR_NOSUCHCHANNELNBR + channelName + ERR_NOSUCHCHANNEL + END;
		send(fd, response.c_str(), response.size(), 0);
		return;
	}

	Client* client = getClientByFD(fd);

	std::string topic = parseTopic(string);
	if (topic == "") {
		if(channel->getTopic().empty()) {
			std::cout << RED << "Error quitting channel..." << WHITE << std::endl;
			response = IRC + RPL_NOTOPICNBR + client->getNickname() + " " + channelName + RPL_NOTOPIC + END;
			send (fd, response.c_str(), response.size(), 0);
			return;
		}

		std::cout << YELLOW << "Showing topic..." << WHITE << std::endl;
		response = IRC + RPL_TOPICNBR + client->getNickname() + " " + channelName + " :" + channel->getTopic() + END;
		send(fd, response.c_str(), response.size(), 0);
		return;

	} else {
		if(channel->getMode("t") == true && !channel->isOperator(client->getNickname())) {
			std::cout << RED << "Topic not changed..." << WHITE << std::endl;
			response = IRC + ERR_CHANOPRIVSNEEDEDNBR + client->getNickname() + " " + channelName + ERR_CHANOPRIVSNEEDED + END;
			send(fd, response.c_str(), response.size(), 0);
			return;
		}

		std::cout << YELLOW << "Setting topic..." << WHITE << std::endl;
		channel->setTopic(topic);

		std::vector<Client*> clients = channel->getAllClients();
		response = ":" + client->getNickname() + "!~" + client->getClientname() + "@ft.irc TOPIC " + channelName + " " + topic + END;
		for (size_t i = 0; i < clients.size(); i++)
			send(clients[i]->getFd(), response.c_str(), response.size(), 0);
	}
}