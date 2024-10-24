#include "../includes/Server.hpp"

static bool isValidChannelName(const std::string& channelName) {
    if (channelName.empty() || channelName[0] != '#')
        return false;

    return true;
}

static bool isValidReason(const std::string& channelName) {
    if (channelName.empty() || channelName[0] != ':')
        return false;

    return true;
}

void Server::kick(std::vector<std::string> string, int fd) {
	std::string response;

	//verifica se os parametros estão vazios
	if (string.size() == 0 || string[0] == "" || string[0] == "KICK" || string.size() > 3) {
		response = "Invalid command\r\nUsage: KICK #<channel name>  <client to be kicked> (optional):<reason>\r\n";
		send(fd, response.c_str(), response.size(), 0);
		return;
	}

	Client* client = getClientByFD(fd);
	bool validChannelName = isValidChannelName(string[0]);
	if (validChannelName == false) {
		std::cout << RED << "Error while kicking client from channel..." << WHITE << std::endl;
		response = "Invalid channel name\r\nUsage: KICK #<channel name> <client to be kicked> (optional):<reason>\r\n";
		send(fd, response.c_str(), response.size(), 0);
		return;
	}
	
	Channel* channel = getChannel(string[0].substr(1));

	//verifica se o canal existe
	if (channel == NULL) {
		std::cout << RED << "Error while kicking client from channel..." << WHITE << std::endl;
		response = "Channel does not exist\r\n";
		send(fd, response.c_str(), response.size(), 0);
		return;
	}

	//verifica se o client que chamou o comando está no canal
	if (!channel->isOnChannel(client->getNickname())) {
		std::cout << RED << "Error while kicking client from channel..." << WHITE << std::endl;
		response = "You must be on the channel to use kick command\r\n";
		send(fd, response.c_str(), response.size(), 0);
		return;
	}

	//verifica se o client que chamou o comando é operador no canal
	if (!channel->isOperator(client->getNickname())) {
		std::cout << RED << "Error while kicking client from channel..." << WHITE << std::endl;
		response = "You don't have operator privileges on this channel\r\n";
		send(fd, response.c_str(), response.size(), 0);
		return;
	}

	//verifica se o client a ser expulso existe
	Client* kickedClient = getClientByNick(string[1]);
	if (kickedClient == NULL) {
		std::cout << RED << "Error while kicking client from channel..." << WHITE << std::endl;
		response = "This client does not exist\r\n";
		send(fd, response.c_str(), response.size(), 0);
		return;
	}

	//verifica se o client a ser expulso esta no canal
	if (!channel->isOnChannel(kickedClient->getNickname())) {
		std::cout << RED << "Error while kicking client from channel..." << WHITE << std::endl;
		response = "This client is not on this channel\r\n";
		send(fd, response.c_str(), response.size(), 0);
		return;
	}

	//verifica se existe uma razão para a expulsão

	std::string reason;
	if (string.size() == 3) {
		if (!isValidReason(string[2])) {
			std::cout << RED << "Error while kicking client from channel..." << WHITE << std::endl;
			response = "Invalid reason\r\nUsage: KICK #<channel name> <client to be kicked> (optional):<reason>\r\n";
			send(fd, response.c_str(), response.size(), 0);
			return;
		}
		reason = string[2];
	}
	else
		reason = ": no reason";

	// int kickedClientFD = kickedClient->getFd();
	// response = "You were kicked from the channel " + channel->getName() + "\r\nReason: " + reason + "\r\n";
	// send(kickedClientFD, response.c_str(), response.size(), 0);

	std::vector<Client *> clients = channel->getAllClients();
	response = "#" + channel->getName() + ": " + client->getNickname() + " has been kicked from this channel\r\nReason: " + reason + "\r\n";
	for (size_t i = 0; i < clients.size(); i++)
		send(clients[i]->getFd(), response.c_str(), response.size(), 0);

	std::cout << YELLOW << "Kicking client from channel..." << WHITE << std::endl;
	channel->removeClient(kickedClient->getNickname());
}