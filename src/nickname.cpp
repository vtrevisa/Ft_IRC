#include "../includes/Server.hpp"

void	Server::nickname(std::vector<std::string> nick, int fd)
{
	std::string response;
	Client *client = getClientByFD(fd);
	if (client == NULL) //segurança pra testar se o client existe
	{
		response = "Client not found\r\n";
		return;
	}

	if (nick[0] == "NICK") //se o comando não tiver argumentos ou o comando não for "/nickname"
	{
		if (client->getNickname() == "")
			response = "You don't have a nickname yet\r\n";
		else
			response = "Your nickname is " + client->getNickname() + "\r\n";
		send(fd, response.c_str(), response.size(), 0);
		return;
	}
	else
	{
		
		// Check if the nickname is already in use
		for (std::vector<Client>::iterator it = _clients.begin(); it != _clients.end(); ++it)
		{
			if (it->getNickname() == nick[0])
			{
				response = "Nickname already in use\r\n";
				send(fd, response.c_str(), response.size(), 0);
				return;
			}
		}
		client->setNickname(nick[0]);
		response = "Nickname changed to " + nick[0] + "\r\n";
		send(fd, response.c_str(), response.size(), 0);
	}
}