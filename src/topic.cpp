#include "../includes/Server.hpp"

static bool isValidChannelName(const std::string& channelName) {
    if (channelName.empty() || channelName[0] != '#')
        return false;

    return true;
}

static bool isValidTopic(const std::string& channelName) {
    if (channelName.empty() || channelName[0] != ':')
        return false;

    return true;
}

void Server::topic(std::vector<std::string> string, int fd) {
	std::string response;

	//verifica se os parametros estão vazios
	if (string.size() == 0 || string[0] == "" || string[0] == "TOPIC" || string.size() > 2) {
		response = std::string(RED) +
		"Invalid command\r\nUsage: TOPIC #<channel name> (optional)<topic>\r\n"
		+ std::string(WHITE);
		send(fd, response.c_str(), response.size(), 0);
		return;
	}

	bool validChannelName = isValidChannelName(string[0]);
	if (validChannelName == false) {
		std::cout << RED << "Error getting channel topic..." << WHITE << std::endl;
		response = std::string(RED) + "Invalid channel name\r\nUsage: TOPIC #<channel name> (optional):<topic>\r\n" + std::string(WHITE);
		send(fd, response.c_str(), response.size(), 0);
		return;
	}
	
	std::string channelName = string[0].substr(1);
	Channel* channel = getChannel(string[0].substr(1));

	//verifica se o canal existe
	if (channel == NULL) {
		std::cout << RED << "Error getting channel topic..." << WHITE << std::endl;
		response = std::string(RED) + "Channel does not exist\r\n" + std::string(WHITE);
		send(fd, response.c_str(), response.size(), 0);
		return;
	}

	Client* client = Server::getClientByFD(fd);

	if (string.size() == 1) { //retorna o tópico do canal, caso não seja passado um novo tópico
		std::cout << YELLOW << "Getting channel topic..." << WHITE << std::endl;
		response = std::string(YELLOW) + "#" + channel->getName() + " topic: " + channel->getTopic() + "\r\n" + std::string(WHITE);
		send(fd, response.c_str(), response.size(), 0);
		return;
	} else { //verifica o status de operador do usuário que chamou o comando, as restrições de topico e aplica um novo tópico ao canal
		if (!isValidTopic(string[1])) {
			std::cout << RED << "Error getting channel topic..." << WHITE << std::endl;
			response = std::string(RED) + "Invalid topic\r\nUsage: TOPIC #<channel name> (optional):<topic>\\r\n" + std::string(WHITE);
			send(fd, response.c_str(), response.size(), 0);
			return;
		}
		std::string topic = string[1].substr(1);

		//verifica se o client que chamou o comando está no canal
		if (!channel->isOnChannel(client->getNickname())) {
			std::cout << RED << "Error getting channel topic..." << WHITE << std::endl;
			response = std::string(RED) + "You must be on the channel to set a topic\r\n" + std::string(WHITE);
			send(fd, response.c_str(), response.size(), 0);
			return;
		}

		if (channel->getMode("t") == true && !channel->isOperator(client->getNickname())) {
			std::cout << RED << "Error getting channel topic..." << WHITE << std::endl;
			response = std::string(RED) +
			"The topic restrictions are set on this channel\r\nYou must have operator privileges\r\n"
			+ std::string(WHITE);
			send(fd, response.c_str(), response.size(), 0);
			return;
		}
		channel->setTopic(topic);

		std::vector<Client *> clients = channel->getAllClients();
		std::cout << YELLOW << "Setting channel topic..." << WHITE << std::endl;
		response = std::string(YELLOW) + "#" + channel->getName() +
				   ": " + client->getNickname() + " has set this channel topic to: " + channel->getTopic()
				   + "\r\n" + std::string(WHITE);
		for (size_t i = 0; i < clients.size(); i++)
			send(clients[i]->getFd(), response.c_str(), response.size(), 0);
	}
}