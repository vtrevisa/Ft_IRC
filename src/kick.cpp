#include "../includes/Server.hpp"

void Server::kick(std::vector<std::string> string, int fd) {
	Client* client = getClientByFD(fd);
	std::string response;

    if (string.size() == 0 || string[0] == "") {
		std::cout << RED << "Error kicking client from channel..." << WHITE << std::endl;
		response = IRC + ERR_NEEDMOREPARAMSNBR + " KICK " + ERR_NEEDMOREPARAMS + END;
		send(fd, response.c_str(), response.size(), 0);
		return;
    }

	std::string reason;
    const std::string& channelName = string[0];
	
	if (string.size() == 3)
		reason = string[2].substr(1);
	else
		reason = "no reason";

    Channel* channel = getChannel(channelName);
    if (channel == NULL) {
		std::cout << RED << "Error kicking client from channel..." << WHITE << std::endl;
		response = IRC + ERR_NOSUCHCHANNELNBR + channelName + " " + channelName + ERR_NOSUCHCHANNEL + END;
		send(fd, response.c_str(), response.size(), 0);
		return;
    }

    if (!channel->isOperator(client->getNickname())) {
		std::cout << RED << "Error kicking client from channel..." << WHITE << std::endl;
		response = IRC + ERR_CHANOPRIVSNEEDEDNBR + client->getNickname() + " " + channelName + ERR_CHANOPRIVSNEEDED + END;
		send(fd, response.c_str(), response.size(), 0);
		return;
    }

	std::istringstream iss(string[1]);
	std::string KickedClientNick;
	while (std::getline(iss, KickedClientNick, ',')) {
		Client* kickedClient = getClientByNick(KickedClientNick);
		if (kickedClient == NULL) {
			std::cout << RED << "Error kicking client from channel..." << WHITE << std::endl;
			response = IRC + ERR_NOSUCHNICKNBR + " " + channelName + " " + KickedClientNick + ERR_NOSUCHNICK + END;
			send(fd, response.c_str(), response.size(), 0);
			continue;
		}

		if (!channel->isOnChannel(KickedClientNick)) {
			std::cout << RED << "Error kicking client from channel..." << WHITE << std::endl;
			response = IRC + ERR_USERNOTINCHANNELNBR + channelName + " " + KickedClientNick + " " + channelName + ERR_USERNOTINCHANNEL + END;
			send(fd, response.c_str(), response.size(), 0);
			continue;
		}

		if (!channel->isOperator(client->getNickname())) {
			std::cout << RED << "Error kicking client from channel..." << WHITE << std::endl;
			response = IRC + ERR_CHANOPRIVSNEEDEDNBR + client->getNickname() + " " + channelName + ERR_CHANOPRIVSNEEDED + END;
			send(fd, response.c_str(), response.size(), 0);
			return;
		}

		std::cout << YELLOW << "Kicking client from channel..." << WHITE << std::endl;
		response = ":" + client->getNickname() + "!" + client->getClientname() + "@ft.irc KICK " + channelName + " " + KickedClientNick + " :"+ reason + END;
		std::vector<Client*> clients = channel->getAllClients();
		for (size_t i = 0; i < clients.size(); i++)
			send(clients[i]->getFd(), response.c_str(), response.size(), 0);
		int kickedClientFd = kickedClient->getFd();
		channel->removeClient(kickedClientFd);
	}
}