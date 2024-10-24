#include "../includes/Server.hpp"

void Server::help(std::vector<std::string> string, int fd) {
	std::string response;
	if (string.size() == 0 || string[0] == "" || string[0] == "/help" || string.size() > 1) {
		response = std::string(RED) + "Too many arguments\r\nUsage: /help\r\n" + std::string(WHITE);
		send(fd, response.c_str(), response.size(), 0);
		return;
	}

    response = std::string(GREEN)
				+ "Commands:\r\n"
				+ MODE
				+ INVITE
				+ TOPIC
				+ KICK
				+ JOIN
				+ CHANNEL
				+ EXIT
				+ QUIT
				+ NICK
				+ USER
				+ PRIVATEMSG
				+ std::string(WHITE);
	send(fd, response.c_str(), response.size(), 0);
}