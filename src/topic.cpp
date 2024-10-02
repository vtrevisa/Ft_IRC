#include "Server.hpp"

void Server::topic(std::vector<std::string> string, int fd) {
	std::string response;

	//verifica se os parametros estão vazios
	if (string.size() == 0 || string[0] == "" || string.size() > 2) {
		response = "Invalid number of arguments\r\nUsage: /topic <channel name> (optional)<topic in quotes>\r\n";
		send(fd, response.c_str(), response.size(), 0);
		return;
	}

	std::string channelName = string[0];
	Channel* channel = getChannel(string[0]);

	//verifica se o canal existe
	if (channel == NULL) {
		response = "Channel does not exist\r\n";
		send(fd, response.c_str(), response.size(), 0);
		return;
	}

	Client* client = Server::getClientByFD(fd);

	if (string.size() == 1) { //retorna o tópico do canal, caso não seja passado um novo tópico
		response = channel->getTopic() + "\r\n";
		send(fd, response.c_str(), response.size(), 0);
		return;
	} else { //verifica o status de operador do usuário que chamou o comando, as restrições de topico e aplica um novo tópico ao canal
		std::string topic = string[1];

		if (channel->getMode("t") == true && !channel->isOperator(client->getNickname())) {
			response = "The topic restrictions are set on this channel\r\nYou must have operator privileges\r\n";
			send(fd, response.c_str(), response.size(), 0);
			return;
		}
		channel->setTopic(topic);

		std::vector<Client *> clients = channel->getAllClients();
		response = client->getNickname() + " has set this channel topic to: " + channel->getTopic();
		for (size_t i = 0; i < clients.size(); i++)
			send(clients[i]->getFd(), response.c_str(), response.size(), 0);
	}
}