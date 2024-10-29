#include "../includes/Server.hpp"

void Server::mode(std::deque<std::string> string, int fd) {
	std::string response;
	std::string mode;
	
	Client* client = getClientByFD(fd);
	if (string.size() == 0 || string[0] == "") {
		std::cout << RED << "Error manipulating modes..." << WHITE << std::endl;
		response = IRC + ERR_NEEDMOREPARAMSNBR + " MODE " + ERR_NEEDMOREPARAMS + END;
		send(fd, response.c_str(), response.size(), 0);
		return;
	}

	std::string channelName = string[0];
	Channel* channel = getChannel(channelName);
	if(channel == NULL) {
		std::cout << RED << "Error manipulating modes..." << WHITE << std::endl;
		response = IRC + ERR_NOSUCHCHANNELNBR + client->getNickname() + " " + channelName + ERR_NOSUCHCHANNEL + END;
		send(fd, response.c_str(), response.size(), 0);
		return;
	}

	if(string.size() == 1 || string[1] == "") {
		std::cout << YELLOW << "Showing modes..." << WHITE << std::endl;
		std::string imode = "-i";
		if (channel->getMode("i") == true)
			imode = "+i";
		std::string tmode = "-t";
		if (channel->getMode("t") == true)
			tmode = "+t";
		std::string kmode = "-k";
		if (channel->getMode("k") == true)
			kmode = "+k";
		std::string lmode = "-l";
		if (channel->getMode("l") == true)
			lmode = "+l";
		std::string modes = channel->getAllModes();
		response = IRC + RPL_CHANNELMODEISNBR + client->getNickname() + " " + channelName + " " + imode + tmode + kmode + lmode + END;
		send(fd, response.c_str(), response.size(), 0);
		return;
	}
	
	if (!channel->isOperator(client->getNickname())) {
		std::cout << RED << "Error manipulating modes..." << WHITE << std::endl;
		response = IRC + ERR_CHANOPRIVSNEEDEDNBR + client->getNickname() + " " + channelName + ERR_CHANOPRIVSNEEDED + END;
		send(fd, response.c_str(), response.size(), 0);
		return;
	}

	int i = 0;
	std::string modes[] = {"-i", "+i", "-t", "+t", "-k", "+k", "-o", "+o", "-l", "+l"};
	mode = string[1].substr(0, string[1].find('\r'));
	std::string modeArg = "";
	if(string.size() > 2)
		modeArg = string[2];
	Client* clientArg = getClientByNick(modeArg);
	std::stringstream ss(modeArg);
	response = ":" + client->getNickname() + "!" + client->getClientname() + "@ft.irc MODE " + channelName + " ";

	for(; i < 10; i++)
		if(mode == modes[i])
			break;

	switch (i) {
		case 0:
			channel->setMode("i", false);
			response += "-i";
			break;

		case 1:
			channel->setMode("i", true);
			response += "+i";
			break;

		case 2:
			channel->setMode("t", false);
			response += "-t";
			break;

		case 3:
			channel->setMode("t", true);
			response += "+t";
			break;

		case 4:
			channel->setMode("k", false);
			channel->setPassword("");
			response += "-k";
			break;

		case 5:
			if (modeArg == "") {
				std::cout << RED << "Error manipulating modes..." << WHITE << std::endl;
				response = IRC + ERR_NEEDMOREPARAMSNBR + client->getNickname() + channelName + " " + channelName + " k * :You must specify a parameter for the key mode. Syntax: <key>." + END;
				send(fd, response.c_str(), response.size(), 0);
				return;
			}
			channel->setMode("k", true);
			channel->setPassword(modeArg);
			response += "+k";
			break;

		case 6:
			if (modeArg == "") {
				std::cout << RED << "Error manipulating modes..." << WHITE << std::endl;
				response = IRC + ERR_NEEDMOREPARAMSNBR + client->getNickname() + channelName + " " + channelName +  " o * :You must specify a parameter for the nick mode. Syntax: <nick>." + END;
				send(fd, response.c_str(), response.size(), 0);
				return;
			}

			if (clientArg == NULL) {
				std::cout << RED << "Error manipulating modes..." << WHITE << std::endl;
				response = IRC + ERR_NOSUCHNICKNBR + modeArg + ERR_NOSUCHNICK + END;
				send(fd, response.c_str(), response.size(), 0);
				return;
			}

			if (!channel->isOnChannel(clientArg->getNickname()) || !channel->isOperator(clientArg->getNickname()))
				return;

			channel->demoteFromOperator(clientArg->getFd());
			response += "-o";
			break;

		case 7:
			if (modeArg == "") {
				std::cout << RED << "Error manipulating modes..." << WHITE << std::endl;
				response = IRC + ERR_NEEDMOREPARAMSNBR + client->getNickname() + channelName + " " + channelName +  " o * :You must specify a parameter for the nick mode. Syntax: <nick>." + END;
				send(fd, response.c_str(), response.size(), 0);
				return;
			}

			if (clientArg == NULL) {
				std::cout << RED << "Error manipulating modes..." << WHITE << std::endl;
				response = IRC + ERR_NOSUCHNICKNBR + modeArg + ERR_NOSUCHNICK + END;
				send(fd, response.c_str(), response.size(), 0);
				return;
			}

			if (!channel->isOnChannel(clientArg->getNickname()) || channel->isOperator(clientArg->getNickname())) 
				return;

			channel->promoteToOperator(clientArg->getFd());
			response += "+o";
			break;

		case 8:
			channel->setMode("l", false);
			channel->setLimit(1000);
			response += "-l";
			break;

		case 9:
			if (modeArg == "") {
				std::cout << RED << "Error manipulating modes..." << WHITE << std::endl;
				response = IRC + ERR_NEEDMOREPARAMSNBR + client->getNickname() + channelName + " " + channelName +  " l * :You must specify a parameter for the limit mode. Syntax: <limit>." + END;
				send(fd, response.c_str(), response.size(), 0);
				return;
			}
			channel->setMode("l", true);
			int limit;
			ss >> limit;
			channel->setLimit(limit);
			response += "+l";
			break;

		default:
			response = "Invalid mode";
			break;
	}

	response += " " + modeArg + END;
	send(fd, response.c_str(), response.size(), 0);
}