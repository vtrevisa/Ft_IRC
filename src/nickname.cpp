#include "../includes/Server.hpp"

// void	Server::nickname(std::vector<std::string> nick, int fd)
// {
// 	std::string response;
// 	Client *client = getClientByFD(fd);
// 	if (client == NULL) //segurança pra testar se o client existe
// 	{
// 		response = "Client not found\r\n";
// 		return;
// 	}

// 	if (nick.size() < 1 || nick[1].empty()) //se o comando não tiver argumentos
// 	{
// 		if (client->getNickname() == "")
// 			response = "You don't have a nickname yet\r\n";
// 		else
// 			response = "Your nickname is " + client->getNickname() + "\r\n";
// 		send(fd, response.c_str(), response.size(), 0);
// 		return;
// 	}
// 	else
// 	{
// 		client->setNickname(nick[1]);
// 		response = "Nickname changed to " + nick[1] + "\r\n";
// 		send(fd, response.c_str(), response.size(), 0);
// 	}
// }