#include "../includes/Server.hpp"

void	Server::nickname(std::deque<std::string> nick, int fd)
{
	std::string response;
	Client* client = getClientByFD(fd);
	//segurança pra testar se o client existe

	if (client == NULL) {
		std::cout << RED << "Error setting nickname..." << WHITE << std::endl;
		return;
	}

	//se o comando não tiver argumentos ou o comando não for "/nickname" 
	if (nick[0] == "NICK") {
		std::cout << RED << "Error setting nickname..." << WHITE << std::endl;
		response = IRC + ERR_ERRONEUSNICKNAMENBR + " " + ERR_ERRONEUSNICKNAME + END;
		send(fd, response.c_str(), response.size(), 0);
		return;
	} else {
		// Check if the nickname is already in use
		for (std::deque<Client>::iterator it = _clients.begin(); it != _clients.end(); ++it) {
			if (it->getNickname() == nick[0]) {
				std::cout << RED << "Error setting nickname..." << WHITE << std::endl;
				response = IRC + ERR_NICKNAMEINUSENBR + " " + ERR_NICKNAMEINUSE + END;
				send(fd, response.c_str(), response.size(), 0);
				return;
			}
		}
		std::cout << YELLOW << "Setting nickname..." << WHITE << std::endl;
		client->setNickname(nick[0]);
		send(fd, response.c_str(), response.size(), 0);
	}
	if(client->isAuth() == true) {
		response = IRC + RPL_WELCOMENBR + client->getNickname() + RPL_WELCOME + END;
		send(fd, response.c_str(), response.size(), 0);
		return;
	}
}