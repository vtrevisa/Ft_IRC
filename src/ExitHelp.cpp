#include "../includes/Server.hpp"

void	Server::exit(int fd)
{
	std::string response = "Goodbye!\r\n";
	send(fd, response.c_str(), response.size(), 0);
	std::cout << RED << "Client <" << fd << "> Disconnected" << WHI << std::endl;
	close(fd);
	ClearClients(fd);
}

/* void	Server::join()
{
	std::cout << "Join" << std::endl;
} */

/* void	Server::channel()
{
	std::cout << "Channel" << std::endl;
} */

/* void	Server::promote(int fd)
{
	std::cout << "Promote" << std::endl;
} */

/* void	Server::demote(int fd)
{
	std::cout << "Demote" << std::endl;
} */

void	Server::help(std::string* commands, int fd)
{
	std::string response = "Available commands:\r\n";
	for (int i = 0; i < 13; i++)
		response += commands[i] + "\r\n";
	send(fd, response.c_str(), response.size(), 0);
}

void	Server::nickname(std::vector<std::string> nick, int fd)
{
	std::string response;
	Client *client = getClientByFD(fd);
	if (client == NULL) //segurança pra testar se o client existe
	{
		response = "Client not found\r\n";
		return;
	}

	if (nick.size() < 1 || nick[1].empty()) //se o comando não tiver argumentos
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
		client->setNickname(nick[1]);
		response = "Nickname changed to " + nick[1] + "\r\n";
		send(fd, response.c_str(), response.size(), 0);
	}
}

/* void Server::username(std::vector<std::string> user, int fd)
{
	std::string response = "Username changed to " + user[1] + "\r\n";
	send(fd, response.c_str(), response.size(), 0);
} */

/* void	Server::pmsg(std::vector<std::string> msg, int fd)
{
	std::string response = "Private message sent to " + msg[1] + "\r\n";
	send(fd, response.c_str(), response.size(), 0);
} */