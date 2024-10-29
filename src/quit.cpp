#include "Server.hpp"

void	Server::quit(int fd) {
	Client* client = getClientByFD(fd);

	// procura e remove o client de todos os canais
	std::deque<Channel*> channels = getAllChannels();
	for (std::deque<Channel*>::iterator chanIt = channels.begin(); chanIt != channels.end(); ++chanIt) {
		std::deque<Client*> clients = (*chanIt)->getAllClients();
		for (std::deque<Client*>::iterator clientIt = clients.begin(); clientIt != clients.end(); ++clientIt) {
			if ((*clientIt)->getFd() == fd) {
				(*chanIt)->removeClient(client->getFd());
				break;
			}
		}
	}
	for (size_t i = 0; i < _channels.size(); i++) {
		_channels[i].removeClient(fd);
	}
	std::cout << RED << "Client <" << client->getNickname() << "> Disconnected" << WHITE << std::endl;
	ClearClients(fd);
	close(fd);
	fd = -1;
}