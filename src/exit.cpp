#include "Server.hpp"

void	Server::exit(std::vector<std::string> string, int fd) {
	std::string response = "Goodbye!\r\n";

	if (string.size() != 0) {
		response = "Too many arguments\r\nUsage: /exit\r\n";
		send(fd, response.c_str(), response.size(), 0);
		return;
	}

	send(fd, response.c_str(), response.size(), 0);
	std::cout << RED << "Client <" << fd << "> Disconnected" << WHI << std::endl;
	close(fd);
	ClearClients(fd);
}