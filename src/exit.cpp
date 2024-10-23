#include "Server.hpp"

void	Server::exit(std::vector<std::string> string, int fd) {
	std::string response = "Goodbye!\r\n";
	Client* client = getClientByFD(fd);

	if (string.size() != 0) {
		response = std::string(RED) + "Too many arguments\r\nUsage: /exit\r\n";
		send(fd, response.c_str(), response.size(), 0);
		return;
	}

	send(fd, response.c_str(), response.size(), 0);
	if (client->getNickname() == "")
		std::cout << RED << "Client " << client->getFd() << "Disconnected" << WHITE << std::endl;
	else
		std::cout << RED << "Client <" << client->getNickname() << "> Disconnected" << WHITE << std::endl;
	close(fd);
	ClearClients(fd);
}