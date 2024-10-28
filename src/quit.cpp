#include "Server.hpp"

void	Server::quit(int fd) {
	Client* client = getClientByFD(fd);

	// procura e remove o client de todos os canais
	std::vector<Channel*> channels = getAllChannels();
	for (std::vector<Channel*>::iterator chanIt = channels.begin(); chanIt != channels.end(); ++chanIt) {
		std::vector<Client*> clients = (*chanIt)->getAllClients();
		for (std::vector<Client*>::iterator clientIt = clients.begin(); clientIt != clients.end(); ++clientIt) {
			if ((*clientIt)->getFd() == fd) {
				(*chanIt)->removeClient(client->getFd());
				break;
			}
		}
	}

	std::cout << RED << "Client <" << client->getNickname() << "> Disconnected" << WHITE << std::endl;
	close(fd);
	ClearClients(fd);
}