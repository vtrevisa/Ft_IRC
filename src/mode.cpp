#include "../includes/Server.hpp"

void Server::mode(std::vector<std::string> string, int fd) {
	std::string response;

	Client* client = getClientByFD(fd);
	std::string channelName = string[0];
	Channel* channel = getChannel(channelName);

	//verifica se o canal existe no server
	if (channel == NULL) {
		response = "Channel does not exist\r\n";
		send(fd, response.c_str(), response.size(), 0);
		return;
	}

	//verifica se o usuário é um operador
	if (!channel->isOperator(client->getNickname())) {
		response = "You don't have operator privileges on this channel\r\n";
		send(fd, response.c_str(), response.size(), 0);
		return;
	}

	//verifica se o usuário enviou um modo para alterar, caso não tenha enviado, retorna a lista de modos ativos no canal
	if (string.size() == 1 || string[1] == "") {
		std::string modes = channel->getAllModes();
		response = "Modes in this channel: "+ modes + "\r\n";
		send(fd, response.c_str(), response.size(), 0);
		return;
	}

	int i = 0;
	std::string modes[] = {"-i", "+i", "-t", "+t", "-k", "+k", "-o", "+o", "-l", "+l"};
	std::string mode;
	mode = string[1].substr(0, string[1].find('\r'));
	std::string modeArg = "";
	if (string.size() > 2)
		modeArg = string[2];
	Client* clientArg = getClientByNick(modeArg);
	std::stringstream ss(modeArg);

	for (; i < 10; i++)
		if (mode == modes[i])
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
				response = "You must provide a password to be set\r\nUsage: /mode +k <password>\r\n";
				send(fd, response.c_str(), response.size(), 0);
				return;
			}
			channel->setMode("k", true);
			channel->setPassword(modeArg);
			response += "+k";
			break;

		case 6:
			if (modeArg == "") {
				response = "You must provide a client to be demoted\r\nUsage: /mode -o <nickname>\r\n";
				send(fd, response.c_str(), response.size(), 0);
				return;
			}

			if (clientArg == NULL) {
				response = "This client is not on this channel\r\n";
				send(fd, response.c_str(), response.size(), 0);
				return;
			}

			if (!channel->isOperator(clientArg->getNickname())) {
				response = "This client is not an operator on this channel\r\n";
				send(fd, response.c_str(), response.size(), 0);
				return;
			}

			channel->demoteFromOperator(clientArg->getNickname());
			response += "-o";
			break;

		case 7:
			if (modeArg == "") {
				response = "You must provide a client to be promoted\r\nUsage: /mode +o <nickname>\r\n";
				send(fd, response.c_str(), response.size(), 0);
				return;
			}

			if (clientArg == NULL) {
				response = "This client is not on this channel\r\n";
				send(fd, response.c_str(), response.size(), 0);
				return;
			}

			if (channel->isOperator(clientArg->getNickname())) {
				response = "This client is already an operator on this channel\r\n";
				send(fd, response.c_str(), response.size(), 0);
				return;
			}

			channel->promoteToOperator(clientArg->getNickname());
			response += "+o";
			break;

		case 8:
			channel->setMode("l", false);
			channel->setLimit(-1);
			response += "-l";
			break;

		case 9:
			if (modeArg == "") {
				response = "You must provide a limit to be set\r\nUsage: /mode +l <limit>\r\n";
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
			response = "Invalid mode\r\n";
			break;
	}

	response += mode + " " + modeArg + "\r\n";
	send(fd, response.c_str(), response.size(), 0);
}