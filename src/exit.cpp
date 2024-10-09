#include "Server.hpp"

void	Server::exit(int fd) {
	std::string response = "Goodbye!\r\n";
	send(fd, response.c_str(), response.size(), 0);
	std::cout << RED << "Client <" << fd << "> Disconnected" << WHI << std::endl;
	close(fd);
	ClearClients(fd);
}