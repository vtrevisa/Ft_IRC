#include "../includes/Server.hpp"

void Server::username(std::vector<std::string> string, int fd) {
	// std::string response;
	
	// // verifica linha de comando passada
	// if (string.size() > 1) {
	// 	response = "Invalid command\r\nUsage: USER <username>\r\n";
	// 	send(fd, response.c_str(), response.size(), 0);
	// 	return;
	// }

	// Client* client = Server::getClientByFD(fd);
	// //verifica se o cliente existe
	// if (client == NULL) {
	// 	std::cout << RED << "Error while setting username..." << WHITE << std::endl;
	// 	response = "Client not found\r\n";
	// 	send(fd, response.c_str(), response.size(), 0);
	// 	return;
	// }

	// //verifica se foi passado um username
	// if (string.size() == 0 || string[0] == "" || string[0] == "USER" || string.size() > 1) {
	// 	if (client->getClientname() == "")
	// 		response = "You don't have a username yet\r\n";
	// 	else
	// 		response = "Your username is " + client->getClientname() + "\r\n";
	// 	send(fd, response.c_str(), response.size(), 0);
	// 	return;
	// }

	// //verifica se o cliente já possui um username
	// if (!client->getClientname().empty()){
	// 	std::cout << RED << "Error while setting username..." << WHITE << std::endl;
	// 	response =	"You have already set a username\r\n";
	// 	send(fd, response.c_str(), response.size(), 0);
	// 	return;
	// }

	// std::vector<Client*> clients = getAllClients();
	// std::string username = string[0];

	// //verifica se o username escolhido já está em uso
	// for (std::vector<Client*>::iterator it = clients.begin(); it != clients.end(); ++it) {
	// 	if ((*it)->getClientname() == username) {
	// 		std::cout << RED << "Error while setting username..." << WHITE << std::endl;
	// 		response = "This username is already in use\r\n";
	// 		send(fd, response.c_str(), response.size(), 0);
	// 		return;
	// 	}
	// }

	// client->setClientname(username);
	// std::cout << YELLOW << "Setting username..." << WHITE << std::endl;
	// response = "Your username has been set to: " + username + "\r\n";
	// send(fd, response.c_str(), response.size(), 0);

	


	Client* client = Server::getClientByFD(fd);
	if(client == NULL)
		return;
	std::vector<Client*> clients = getAllClients();
	std::string response;
	std::string username = string[0].substr(0, string[0].find(' '));

	for (std::vector<Client*>::iterator it = clients.begin(); it != clients.end(); ++it) {
		if ((*it)->getClientname() == username) {
			response = "This username is already in use\r\n";
			send(fd, response.c_str(), response.size(), 0);
			return;
		}
	}

	if (!client->getClientname().empty()){
		response = "You have already set a username\r\n";
		send(fd, response.c_str(), response.size(), 0);
		return;
	}
	response = "Your username has been set to: " + username + "\r\n";
	send(fd, response.c_str(), response.size(), 0);
	client->setClientname(username);
	
	if(client->isAuth() == true) {
		std::cout << YELLOW << "Setting username..." << WHITE << std::endl;
		response =	"Welcome to IRCSERV!!\r\n";
		send(fd, response.c_str(), response.size(), 0);
		return;
	}
}