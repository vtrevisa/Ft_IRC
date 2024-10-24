#include "../includes/Server.hpp"

static bool isValidChannelName(const std::string& channelName) {
    if (channelName.empty() || channelName[0] != '#')
        return false;

    return true;
}

void Server::join(std::vector<std::string> string, int fd) {
	std::string response;

	if (string.size() == 0 || string[0] == "" || string[0] == "JOIN" || string.size() > 2) {
		response = "Invalid command\r\nUsage: JOIN #<channel name> (optional)<password>\r\n";
		send(fd, response.c_str(), response.size(), 0);
		return;
	}

	Client* client = getClientByFD(fd);
	bool validChannelName = isValidChannelName(string[0]);
	if (validChannelName == false) {
		std::cout << RED << "Error while joining channel..." << WHITE << std::endl;
		response = "Invalid channel name\r\nUsage: JOIN #<channel name> (optional)<password>\r\n";
		send(fd, response.c_str(), response.size(), 0);
		return;
	}

	Channel* channel = getChannel(string[0].substr(1));
	const std::string& channelName = string[0].substr(1);
	bool owner = false;
	//verifica se o canal existe e se não existir, cria
	if (channel == NULL) {
		createChannel(channelName);
		channel = getChannel(string[0].substr(1));
		owner = true;
	}

	if (channel->getMode("i") == true) {
		if (channel->isInvited(client->getNickname())) {
			channel->removeFromInviteList(client->getNickname());
		} else {
			std::cout << RED << "Error while joining channel..." << WHITE << std::endl;
			response = "You have not been invited for this channel\r\n";
			send(fd, response.c_str(), response.size(), 0);
			return;
		}
	}

	std::string channelPassword = channel->getPassword();
	//verifica se o canal possui senha
	if (!channelPassword.empty() && string.size() == 1) {
		std::cout << RED << "Error while joining channel..." << WHITE << std::endl;
		response = "You must provide a password to join this channel\r\n";
		send(fd, response.c_str(), response.size(), 0);
		return;
	}

	if (!channelPassword.empty()) {
		std::string password = string[1];
		//verifica se a senha é a correta
		if (!channelPassword.empty() && channelPassword != password) {
			std::cout << RED << "Error while joining channel..." << WHITE << std::endl;
			response = "Wrong password\r\n";
			send(fd, response.c_str(), response.size(), 0);
			return;
		}
	}

	if (channel->isOnChannel(client->getNickname()) == true) {
		std::cout << RED << "Error while joining channel..." << WHITE << std::endl;
		response = "You are already on this channel\r\n";
		send(fd, response.c_str(), response.size(), 0);
		return;
	}

	//verifica se o canal está cheio
	if(channel->getClientCount() < channel->getLimit()) {
		channel->addClient(client);
		std::cout << YELLOW << "Joining channel..." << WHITE << std::endl;
	} else {
		std::cout << RED << "Error while joining channel..." << WHITE << std::endl;
		response = "You cannot join this channel\r\nThis channel has reached the client limit\r\n";
		send(fd, response.c_str(), response.size(), 0);
		return;
	}

	if (owner)
		channel->promoteToOperator(client->getNickname());

	std::vector<Client *> clients = channel->getAllClients();
	response = "#" + channel->getName() + ": " +
				client->getNickname() + " has joined this channel\r\n";
	for (size_t i = 0; i < clients.size(); i++)
	{
		if (clients[i]->getFd() != client->getFd())
			send(clients[i]->getFd(), response.c_str(), response.size(), 0);
		else 
		{
			response = "#" + channel->getName() + ": you joined this channel\r\n";
			send(client->getFd(), response.c_str(), response.size(), 0);
		}
	}
}