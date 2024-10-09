#include "Server.hpp"

void Server::help(std::vector<std::string> string, int fd) {
	std::string response;
	if (string.size() != 0) {
		response = "Too many arguments\r\nUsage: /help\r\n";
		send(fd, response.c_str(), response.size(), 0);
		return;
	}

    response = "Commands:\r\n";
	send(fd, response.c_str(), response.size(), 0);
	response = "/mode - check/set channel modes\r\n       Usage: /mode <channel name> <+/-><mode> (opitional)<complement>\r\n";
	send(fd, response.c_str(), response.size(), 0);
	response = "/invite - invite client to channel\r\n       Usage: /invite <client nickname to be invited> <channel name>\r\n";
	send(fd, response.c_str(), response.size(), 0);
	response = "/topic - check/set channel topic\r\n       Usage: /topic <channel name> (optional)<topic in double quotes>\r\n";
	send(fd, response.c_str(), response.size(), 0);
	response = "/kick - kick client from channel\r\n       Usage: /kick <client nickname to be kicked> <channel name> (optional)<reason in double quotes>\r\n";
	send(fd, response.c_str(), response.size(), 0);
	response = "/join - join a channel\r\n       Usage: /join <channel name> (opitional)<password>\r\n";
	send(fd, response.c_str(), response.size(), 0);
	response = "/channel - send message on a channel\r\n       Usage: /channel <channel name> <message in double quotes>\r\n";
	send(fd, response.c_str(), response.size(), 0);
	response = "/exit - exit server\r\n       Usage: /exit\r\n";
	send(fd, response.c_str(), response.size(), 0);
	response = "/nickname - set nickname\r\n       Usage: \r\n";//to be done
	send(fd, response.c_str(), response.size(), 0);
	response = "/username - set useuname\r\n       Usage: \r\n";//to be done
	send(fd, response.c_str(), response.size(), 0);
	response = "/pmsg - send private message to a client\r\n       Usage: /pmsg <client nickname to send message> <message in double quotes>\r\n";
	send(fd, response.c_str(), response.size(), 0);
}