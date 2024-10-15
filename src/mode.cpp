#include "../includes/Server.hpp"

void Server::mode(std::vector<std::string> string, int fd) {
	std::string response;

	Client* client = getClientByFD(fd);
	std::string channelName = string[0];
	Channel* channel = getChannel(channelName);

	//verifica se o canal existe no server
	if (channel == NULL) {
		response = std::string(RED) + "Channel does not exist\r\n" + std::string(WHITE);
		send(fd, response.c_str(), response.size(), 0);
		return;
	}

	//verifica se o client que chamou o comando está no canal
	if (!channel->isOnChannel(client->getNickname())) {
		response = std::string(RED) + "You must be on the channel to use mode command\r\n" + std::string(WHITE);
		send(fd, response.c_str(), response.size(), 0);
		return;
	}

	//verifica se o usuário é um operador
	if (!channel->isOperator(client->getNickname())) {
		response = std::string(RED) + "You don't have operator privileges on this channel\r\n" + std::string(WHITE);
		send(fd, response.c_str(), response.size(), 0);
		return;
	}

	//verifica se o usuário enviou um modo para alterar, caso não tenha enviado, retorna a lista de modos ativos no canal
	if (string.size() == 1 || string[1] == "") {
		std::string modes = channel->getAllModes();
		response = std::string(YELLOW) + "#" + channel->getName() + ": Modes on this channel: "+ modes + "\r\n";
		send(fd, response.c_str(), response.size(), 0);
		return;
	}

	int i = 0;
	std::string modes[] = {"-i", "+i", "-t", "+t", "-k", "+k", "-o", "+o", "-l", "+l"};
	std::string mode;
	mode = string[1];
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
			break;

		case 1:
			channel->setMode("i", true);
			break;

		case 2:
			channel->setMode("t", false);
			break;

		case 3:
			channel->setMode("t", true);
			break;

		case 4:
			channel->setMode("k", false);
			channel->setPassword("");
			break;

		case 5:			
			if (modeArg == "") {
				response = std::string(RED) +
				"You must provide a password to be set\r\nUsage: /mode <channel name> +k <password>\r\n"
				+ std::string(WHITE);
				send(fd, response.c_str(), response.size(), 0);
				return;
			}
			channel->setMode("k", true);
			channel->setPassword(modeArg);
			break;

		case 6:
			if (modeArg == "") {
				response = std::string(RED) +
				"You must provide a client to be demoted\r\nUsage: /mode <channel name> -o <nickname>\r\n"
				+ std::string(WHITE);
				send(fd, response.c_str(), response.size(), 0);
				return;
			}

			if (clientArg == NULL) {
				response = std::string(RED) + "This client is not on this channel\r\n"
				+ std::string(WHITE);
				send(fd, response.c_str(), response.size(), 0);
				return;
			}

			if (!channel->isOperator(clientArg->getNickname())) {
				response = std::string(RED) + "This client is not an operator on this channel\r\n"
				+ std::string(WHITE);
				send(fd, response.c_str(), response.size(), 0);
				return;
			}

			channel->demoteFromOperator(clientArg->getNickname());
			break;

		case 7:
			if (modeArg == "") {
				response = std::string(RED) +
				"You must provide a client to be promoted\r\nUsage: /mode <channel name> +o <nickname>\r\n"
				+ std::string(WHITE);
				send(fd, response.c_str(), response.size(), 0);
				return;
			}

			if (clientArg == NULL) {
				response = std::string(RED) + "This client is not on this channel\r\n" + std::string(WHITE);
				send(fd, response.c_str(), response.size(), 0);
				return;
			}

			if (channel->isOperator(clientArg->getNickname())) {
				response = std::string(RED) + "This client is already an operator on this channel\r\n" + std::string(WHITE);
				send(fd, response.c_str(), response.size(), 0);
				return;
			}

			channel->promoteToOperator(clientArg->getNickname());
			break;

		case 8:
			channel->setMode("l", false);
			channel->setLimit(1000);
			break;

		case 9:
			if (modeArg == "") {
				response = std::string(RED) +
				"You must provide a limit to be set\r\nUsage: /mode <channel name> +l <limit>\r\n"
				+ std::string(WHITE);
				send(fd, response.c_str(), response.size(), 0);
				return;
			}
			channel->setMode("l", true);
			int limit;
			ss >> limit;
			channel->setLimit(limit);
			break;

		default:
			response = std::string(RED) + "Invalid mode\r\n" + std::string(WHITE);
			break;
	}

	response = std::string(YELLOW) + "#" + channel->getName() +
			   ": " + client->getNickname() + " has set this channel topic to: " +
			   channel->getTopic() + "\r\n"
			   + std::string(WHITE);
	std::vector<Client *> clients = channel->getAllClients();
	for (size_t i = 0; i < clients.size(); i++)
		send(clients[i]->getFd(), response.c_str(), response.size(), 0);
}