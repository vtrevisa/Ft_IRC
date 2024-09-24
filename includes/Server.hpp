#ifndef SERVER_HPP
#define SERVER_HPP

#include "Libs.hpp"
#include "Client.hpp"
#include "Channel.hpp"

class Client;
class Channel;

class Server //-> class for server
{
	private:
		std::string					_passwd; //-> server password
		int							_Port; //-> server port
		int							_SerSocketFd; //-> server socket file descriptor
		static bool					_Signal; //-> static boolean for signal
		std::vector<Client>		_clients; //-> vector of clients
		std::vector<struct pollfd>	_fds; //-> vector of pollfd
		std::vector<Channel>		_channels;

	public:
		Server(); //-> default constructor
		~Server();

		void						ServerInit(int port, std::string passwd); //-> server initialization
		void						SerSocket(); //-> server socket creation
		void						AcceptNewClient(); //-> accept new client
		void						ReceiveNewData(int fd); //-> receive new data from a registered client

		static void					SignalHandler(int signum); //-> signal handler

		void						CloseFds(); //-> close file descriptors
		void						ClearClients(int fd); //-> clear clients

		Client*						getClientByFD(int fd);
		Channel*					getChannel(const std::string& channelName);

		bool						channelExists(std::string &channelName);
		void						createChannel(std::string channelName);
		void						deleteChannel(std::string channelName);

		// void						identifyCommand(std::string& string, int fd);
		// std::vector<std::string>	parseCommand(std::string string);
		// void						unknownCommand(std::string command, int fd);
};

#endif