#include "../includes/Server.hpp"

static bool isValidChannelName(const std::string& channelName) {
    if (channelName.empty() || channelName[0] != '#')
        return false;

    return true;
}

void Server::mode(std::vector<std::string> string, int fd) {
	std::string response;

	Client* client = getClientByFD(fd);

	if (string.size() == 0 || string[0] == "" || string[0] == "MODE" || string.size() > 4) {
		response = "You must provide a client to be demoted\r\nUsage: MODE #<channel name> +/-<mode> <option>\r\n";
		send(fd, response.c_str(), response.size(), 0);
		return;
	}

	bool validChannelName = isValidChannelName(string[0]);
	if (validChannelName == false) {
		response = "Invalid channel name\r\nUsage: MODE #<channel name> +/-<mode> <option>\r\n";
		send(fd, response.c_str(), response.size(), 0);
		return;
	}

	std::string channelName = string[0].substr(1);
	Channel* channel = getChannel(channelName);

	//verifica se o canal existe no server
	if (channel == NULL) {
		response = "Channel does not exist\r\n";
		send(fd, response.c_str(), response.size(), 0);
		return;
	}

	//verifica se o client que chamou o comando está no canal
	if (!channel->isOnChannel(client->getNickname())) {
		response = "You must be on the channel to use mode command\r\n";
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
		response = "#" + channel->getName() + ": Modes on this channel: "+ modes + "\r\n";
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
				response = "You must provide a password to be set\r\nUsage: MODE #<channel name> +k <password>\r\n";
				send(fd, response.c_str(), response.size(), 0);
				return;
			}
			channel->setMode("k", true);
			channel->setPassword(modeArg);
			break;

		case 6:
			if (modeArg == "") {
				response = "You must provide a client to be demoted\r\nUsage: MODE #<channel name> -o <nickname>\r\n";
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
			break;

		case 7:
			if (modeArg == "") {
				response = "You must provide a client to be promoted\r\nUsage: MODE #<channel name> +o <nickname>\r\n";
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
			break;

		case 8:
			channel->setMode("l", false);
			channel->setLimit(1000);
			break;

		case 9:
			if (modeArg == "") {
				response =  "You must provide a limit to be set\r\nUsage: MODE #<channel name> +l <limit>\r\n";
				send(fd, response.c_str(), response.size(), 0);
				return;
			}
			channel->setMode("l", true);
			int limit;
			ss >> limit;
			channel->setLimit(limit);
			break;

		default:
			response = "Invalid mode\r\n";
			break;
	}

	response = "#" + channel->getName() +
			   ": " + client->getNickname() + "#" + channel->getName() + ": " + client->getNickname() + " has changed mode setting\r\n"
			  ;
	std::vector<Client*> clients = channel->getAllClients();
	for (size_t i = 0; i < clients.size(); i++)
		send(clients[i]->getFd(), response.c_str(), response.size(), 0);
}