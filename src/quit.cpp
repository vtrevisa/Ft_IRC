#include "Server.hpp"

void	Server::quit(std::vector<std::string> string, int fd) {
	std::string response = "Goodbye!\r\n";
	Client* client = getClientByFD(fd);

	if (string.size() == 0 || string[0] == "" || string[0] == "QUIT" || string.size() > 1) {
		response = "Invalid command\r\nUsage: QUIT\r\n";
		send(fd, response.c_str(), response.size(), 0);
		return;
	}

	// procura e remove o client de todos os canais
	std::vector<Channel*> channels = getAllChannels();
	for (std::vector<Channel*>::iterator chanIt = channels.begin(); chanIt != channels.end(); ++chanIt) {
		std::vector<Client*> clients = (*chanIt)->getAllClients();
		for (std::vector<Client*>::iterator clientIt = clients.begin(); clientIt != clients.end(); ++clientIt) {
			if ((*clientIt)->getFd() == fd) {
				(*chanIt)->removeClient(client->getNickname());
				break;
			}
		}
	}

	send(fd, response.c_str(), response.size(), 0);
	std::cout << RED << "Client <" << client->getNickname() << "> Disconnected" << WHITE << std::endl;
	close(fd);
	ClearClients(fd);
}