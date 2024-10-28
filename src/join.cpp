#include "../includes/Server.hpp"

static void splitString(std::string& str, char delimiter, std::vector<std::string>& tokens) {
	std::stringstream ss(str);
	std::string token;
	while (!ss.eof() && std::getline(ss, token, delimiter))
		tokens.push_back(token);
}

static bool isValidChannelName(const std::string& channelName) {
    if (channelName.empty() || channelName[0] != '#')
        return false;
    return true;
}

void Server::join(std::vector<std::string> string, int fd) {
	bool isOperator = false;

	std::string response;
	std::vector<std::string> channels;

	if (string[0].empty() || (string.size() > 1 && string[1].empty())) {
		std::cout << RED << "Error joining channel..." << WHITE << std::endl;
		response = IRC + ERR_NEEDMOREPARAMSNBR + " JOIN " + ERR_NEEDMOREPARAMS + END;
		send(fd, response.c_str(), response.size(), 0);
		return;
    }

	splitString(string[0], ',', channels);

    std::vector<std::string> passwords;
    if (string.size() > 1) {
        splitString(string[1], ',', passwords);
	}

	Client* client = getClientByFD(fd);
	for (size_t i = 0; i < channels.size(); i++) {
		if (!isValidChannelName(channels[i])) {
			std::cout << RED << "Error while joining channel..." << WHITE << std::endl;
			response = IRC + ERR_BADCHANMASKNBR + channels[i] + ERR_BADCHANMASK + END;
			send(fd, response.c_str(), response.size(), 0);
			continue;
		}

		if (!channelExists(channels[i])) {
			createChannel(channels[i]);
			isOperator = true;
		}

		Channel* channel = getChannel(channels[i]);
		if (channel->getMode("i") == true) {
			if (!channel->isInvited(client->getNickname())) {
				std::cout << RED << "Error while joining channel..." << WHITE << std::endl;
				response = IRC + ERR_INVITEONLYCHANNBR + channels[i] + " " + channels[i] + ERR_INVITEONLYCHAN + END;
				send(fd, response.c_str(), response.size(), 0);
				continue;
			} else {
				channel->removeFromInviteList(client->getNickname());
			}
		}

		std::string password = channel->getPassword();
		if (!password.empty() && passwords.size() < i + 1) {
			std::cout << RED << "Error while joining channel..." << WHITE << std::endl;
			response = IRC + ERR_BADCHANNELKEYNBR + channels[i] + " " + channels[i] + ERR_BADCHANNELKEY + END;
			send(fd, response.c_str(), response.size(), 0);
			continue;
		}
		if (!password.empty() && password != passwords[i]) {
			std::cout << RED << "Error while joining channel..." << WHITE << std::endl;
			response = IRC + ERR_PASSWDMISMATCHNBR + channels[i] + ERR_PASSWDMISMATCH + END;
			send(fd, response.c_str(), response.size(), 0);
			continue;
		}

		if(channel->getClientCount() < channel->getLimit()) {
			channel->addClient(client); //client added here
		} else {
			std::cout << RED << "Error while joining channel..." << WHITE << std::endl;
			response = IRC + ERR_CHANNELISFULLNBR + channels[i] + " " + channels[i] + ERR_CHANNELISFULL + END;
			send(fd, response.c_str(), response.size(), 0);
			continue;
		}
		if (isOperator) {
			channel->promoteToOperator(fd);
		}
		
		std::cout << YELLOW << "Joining channel..." << WHITE << std::endl;
		std::string response = ":" + client->getNickname() + " JOIN " + channels[i] + END;
		send(fd, response.c_str(), response.size(), 0);

		if (!channel->getTopic().empty()) {
            response = channels[i] + " :" + channel->getTopic() + "\r\n";
            send(fd, response.c_str(), response.size(), 0);
        }
	}
}
