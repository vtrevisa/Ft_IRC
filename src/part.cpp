#include "../includes/Server.hpp"

void Server::part(const std::vector<std::string> string, int fd) {
	Client* client = getClientByFD(fd);
	std::string reason = "";
	std::string response;

    if (string.empty()) {
		std::cout << RED << "Error quitting channel..." << WHITE << std::endl;
		response = IRC + ERR_NEEDMOREPARAMSNBR + " PART " + ERR_NEEDMOREPARAMS + END;
		return;
    } else if (string.size() > 1) {
		for (size_t i = 1; i < string.size(); i++)
			reason += string[i];
	}

    std::istringstream iss(string[0]);
    std::string channelName;

	while (std::getline(iss, channelName, ',')) {
		Channel* channel = getChannel(channelName);
		if (channel == NULL) {
			std::cout << RED << "Error quitting channel..." << WHITE << std::endl;
			response = IRC + ERR_NOSUCHCHANNELNBR + channelName + ERR_NOSUCHCHANNEL + END;
			send(fd, response.c_str(), response.size(), 0);
		} else {
			std::cout << YELLOW << "Quitting channel..." << WHITE << std::endl;
			response = ":" + client->getNickname() + "!~" + client->getClientname() + "@* PART " + channelName + " :" + reason + END;
			std::vector<Client*> clients = channel->getAllClients();
			for (size_t i = 0; i < clients.size(); i++)
				send(clients[i]->getFd(), response.c_str(), response.size(), 0);
		}
		channel->removeClient(client->getNickname());

		if (channel->getAllClients().size() == 0)
			deleteChannel(channelName);
	}
}
