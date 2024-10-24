#include "../includes/Server.hpp"

static bool isValidChannelName(const std::string& channelName) {
    if (channelName.empty() || channelName[0] != '#')
        return false;

    return true;
}

void Server::invite(std::vector<std::string> string, int fd) {
	std::string response;

	//verifica se os parametros estão vazios
	if (string.size() == 0 || string[0] == "" || string[0] == "INVITE" || string.size() > 2) {
		response = std::string(RED) + "Invalid command\r\nUsage: INVITE <client to be invited> #<channel name>\r\n" + std::string(WHITE);
		send(fd, response.c_str(), response.size(), 0);
		return;
	}

	Client* client = getClientByFD(fd);

	bool validChannelName = isValidChannelName(string[1]);
	if (validChannelName == false) {
		std::cout << RED << "Error while inviting to channel..." << WHITE << std::endl;
		response = std::string(RED) + "Invalid channel name\r\nUsage: INVITE <client to be invited> #<channel name>\r\n" + std::string(WHITE);
		send(fd, response.c_str(), response.size(), 0);
		return;
	}

	Channel* channel = getChannel(string[1].substr(1));
	const std::string& channelName = string[1].substr(1);

	//verifica se o canal existe
	if (channel == NULL) {
		std::cout << RED << "Error while inviting to channel..." << WHITE << std::endl;
		response = std::string(RED) + "Channel does not exist\r\n" + std::string(WHITE);
		send(fd, response.c_str(), response.size(), 0);
		return;
	}

	//verifica se o client que chamou o comando está no canal
	if (!channel->isOnChannel(client->getNickname())) {
		std::cout << RED << "Error while inviting to channel..." << WHITE << std::endl;
		response = std::string(RED) + "You must be on the channel to use invite command\r\n" + std::string(WHITE);
		send(fd, response.c_str(), response.size(), 0);
		return;
	}

	//verifica se o client que chamou o comando é operador no canal
	if (!channel->isOperator(client->getNickname())) {
		std::cout << RED << "Error while inviting to channel..." << WHITE << std::endl;
		response = std::string(RED) + "You don't have operator privileges on this channel\r\n" + std::string(WHITE);
		send(fd, response.c_str(), response.size(), 0);
		return;
	}

	//verifica se o client convidado existe
	Client* invitedClient = getClientByNick(string[0]);
	if (invitedClient == NULL) {
		std::cout << RED << "Error while inviting to channel..." << WHITE << std::endl;
		response = std::string(RED) + "This client does not exist\r\n" + std::string(WHITE);
		send(fd, response.c_str(), response.size(), 0);
		return;
	}

	//verifica se o client convidado já esta no canal
	if (channel->isOnChannel(invitedClient->getNickname())) {
		std::cout << RED << "Error while inviting to channel..." << WHITE << std::endl;
		response = std::string(RED) + "This client is already on this channel\r\n" + std::string(WHITE);
		send(fd, response.c_str(), response.size(), 0);
		return;
	}

	channel->addToInviteList(invitedClient->getNickname());

	int invitedClientFD = invitedClient->getFd();
	response = std::string(YELLOW) + client->getNickname() +
				" sent you an invite to enter the chanel " +
				channelName + "\r\n" + std::string(WHITE);
	send(invitedClientFD, response.c_str(), response.size(), 0);
	std::cout << YELLOW << "Sending invite..." << WHITE << std::endl;
}