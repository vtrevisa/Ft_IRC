#include "../includes/Server.hpp"

void Server::invite(const std::vector<std::string> string, int fd) {
	std::string response;
	Client* client = getClientByFD(fd);

    if (string.size() == 0 || string[0] == "") {
		response = IRC + ERR_NEEDMOREPARAMSNBR + " INVITE " + ERR_NEEDMOREPARAMS + END;
		send(fd, response.c_str(), response.size(), 0);
		return;
    }

    const std::string& channelName = string[1];
    Channel* channel = getChannel(channelName);

    if (channel == NULL) {
		response = IRC + ERR_NOSUCHCHANNELNBR + "! " + channelName +  ERR_NOSUCHCHANNEL + END;
		send(fd, response.c_str(), response.size(), 0);
		return;
    }

    if (!channel->isOnChannel(client->getNickname())) {
		response = IRC + ERR_NOTONCHANNELNBR + channelName + " " + channelName + ERR_NOTONCHANNEL + END;
		send(fd, response.c_str(), response.size(), 0);
		return;
    }

	std::istringstream iss(string[0]);
	std::string invitedClientNickname;
	while (std::getline(iss, invitedClientNickname, ',')) {
		Client* invitedClient = getClientByNick(invitedClientNickname);
		if (invitedClient == NULL) {
			response = IRC + ERR_NOSUCHNICKNBR + " " + channelName + " " + invitedClientNickname + ERR_NOSUCHNICK + END;
			send(fd, response.c_str(), response.size(), 0);
			continue;
		}

		if (channel->isOnChannel(invitedClientNickname))
		{
			response = IRC + ERR_USERONCHANNELNBR + channelName + " " + invitedClientNickname + " " + channelName + ERR_USERONCHANNEL + END;
			send(fd, response.c_str(), response.size(), 0);
			continue;
		}

		if (!channel->isOperator(client->getNickname())) {
			response = IRC + ERR_CHANOPRIVSNEEDEDNBR + client->getNickname() + " " + channelName + ERR_CHANOPRIVSNEEDED + END;
			send(fd, response.c_str(), response.size(), 0);
			return;
		}

		channel->addToInviteList(invitedClientNickname);
		response = IRC + RPL_INVITINGNBR + channelName + " " + invitedClientNickname  + " " + channelName + END;
		send(fd, response.c_str(), response.size(), 0);

		response = ":" + client->getNickname() + " INVITE " + invitedClientNickname + " " + channelName + END;
		int invitedClientFD = invitedClient->getFd();
		send(invitedClientFD, response.c_str(), response.size(), 0);

		std::vector<Client*> operators = channel->getOperators();
		for (size_t i = 0; i < operators.size(); i++)
			send(client->getFd(), response.c_str(), response.size(), 0);
	}
}