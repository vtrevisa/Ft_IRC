#include "irc_init.cpp"

bool validateArgs(int argc, char* argv[])
{
	if (argc != 3)
	{
		std::cerr << "Invalid number of arguments. Usage: ./program <port> <password>" << std::endl;
		return false;
	}
	
	// Validate port
	try {
		int port = std::atoi(argv[1]);
		if (port <= 0 || port > 65535) {
			std::cerr << "Invalid port number. Port must be between 1 and 65535." << std::endl;
			return false;
		}
	} catch (const std::exception& e) {
		std::cerr << "Invalid port number. Port must be a valid integer." << std::endl;
		return false;
	}
	
	// Validate password
	std::string password = argv[2];
	if (password.empty())
	{
		std::cerr << "Invalid password. Password cannot be empty." << std::endl;
		return false;
	}
	
	return true;
}

int main(int argc, char* argv[]) {
	Server ser;
	std::cout << "---- SERVER ----" << std::endl;
	try {
		signal(SIGINT, Server::SignalHandler); //-> catch the signal (ctrl + c)
		signal(SIGQUIT, Server::SignalHandler); //-> catch the signal (ctrl + \)
		
		if (!validateArgs(argc, argv)) {
			return 1;
		}
		
		ser.ServerInit(std::atoi(argv[1]), argv[2]); //-> initialize the server
	} catch (const std::exception& e) {
		ser.CloseFds(); //-> close the file descriptors
		std::cerr << e.what() << std::endl;
		return 1;
	}
	
	std::cout << "The Server Closed!" << std::endl;
	return 0;
}