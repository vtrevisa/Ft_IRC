#include "../includes/Server.hpp"

void Server::join(std::vector<std::string> string, int fd) {
	std::string response;

	if (string.size() == 0 || string[0] == "" || string[0] == "/join" || string.size() > 2) {
		response = std::string(RED) + 
				   "Invalid command\r\nUsage: /join <channel name> (optional)<password>\r\n"
				   + std::string(WHITE);
		send(fd, response.c_str(), response.size(), 0);
		return;
	}

	Client* client = getClientByFD(fd);
	Channel* channel = getChannel(string[0]);
	const std::string& channelName = string[0];
	bool owner = false;
	//verifica se o canal existe e se não existir, cria
	if (channel == NULL) {
		createChannel(channelName);
		channel = getChannel(string[0]);
		owner = true;
	}

	if (channel->getMode("i") == true) {
		if (channel->isInvited(client->getNickname())) {
			channel->removeFromInviteList(client->getNickname());
		} else {
			std::cout << RED << "Error while joining channel..." << WHITE << std::endl;
			response = std::string(RED) + "You have not been invited for this channel\r\n" + std::string(WHITE);
			send(fd, response.c_str(), response.size(), 0);
			return;
		}
	}

	std::string channelPassword = channel->getPassword();
	//verifica se o canal possui senha
	if (!channelPassword.empty() && string.size() == 1) {
		std::cout << RED << "Error while joining channel..." << WHITE << std::endl;
		response = std::string(RED) + "You must provide a password to join this channel\r\n" + std::string(WHITE);
		send(fd, response.c_str(), response.size(), 0);
		return;
	}

	if (!channelPassword.empty()) {
		std::string password = string[1];
		//verifica se a senha é a correta
		if (!channelPassword.empty() && channelPassword != password) {
			std::cout << RED << "Error while joining channel..." << WHITE << std::endl;
			response = std::string(RED) + "Wrong password\r\n" + std::string(WHITE);
			send(fd, response.c_str(), response.size(), 0);
			return;
		}
	}

	if (channel->isOnChannel(client->getNickname()) == true) {
		std::cout << RED << "Error while joining channel..." << WHITE << std::endl;
		response = std::string(RED) + "You are already on this channel" + std::string(WHITE);
		send(fd, response.c_str(), response.size(), 0);
		return;
	}

	//verifica se o canal está cheio
	if(channel->getClientCount() < channel->getLimit()) {
		channel->addClient(client);
		std::cout << YELLOW << "Joining channel..." << WHITE << std::endl;
	} else {
		std::cout << RED << "Error while joining channel..." << WHITE << std::endl;
		response = std::string(RED) + "You cannot join this channel\r\nThis channel has reached the client limit\r\n" 
				   + std::string(WHITE);
		send(fd, response.c_str(), response.size(), 0);
		return;
	}

	if (owner)
		channel->promoteToOperator(client->getNickname());

	std::vector<Client *> clients = channel->getAllClients();
	response = std::string(YELLOW) + "#" + channel->getName() + ": " +
				client->getNickname() + " has joined this channel\r\n" +
				std::string(WHITE);
	for (size_t i = 0; i < clients.size(); i++)
	{
		std::cout << "I = " << i <<std::endl;
		std::cout << clients.size() << std::endl;
		std::cout << "fd clients tested = " << clients[i]->getFd() << std::endl;
		std::cout << "fd client tested = " << client->getFd() << std::endl;
		if (clients[i]->getFd() != client->getFd())
			send(clients[i]->getFd(), response.c_str(), response.size(), 0);
		else 
		{
			response = response = std::string(YELLOW) + "#" + channel->getName() + ": " +
							" you joined this channel\r\n" +
						std::string(WHITE);
			send(client->getFd(), response.c_str(), response.size(), 0);
		}
	}
}