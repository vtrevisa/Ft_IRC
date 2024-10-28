#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "Libs.hpp"
#include "Client.hpp"

class Client;

class Channel {
	private:
		std::string						_name;
		std::string						_topic;
		std::string						_password;
		std::map<std::string, bool>		_modes;
		int								_limit;
		int								_clientCount;
		std::vector<std::string>		_inviteList;
		std::vector<Client*>			_clients;
		std::vector<Client*>			_operators;

	public:
		Channel(const std::string& name);
		~Channel();

		void							initModes(std::map<std::string, bool>& modes);

		void							setName(const std::string name);
		void							setTopic(const std::string topic);
		void							setPassword(std::string password);
		void							setMode(std::string mode, bool value);
		void							setLimit(int limit);
		void							increaseClientCount();
		void							decreaseClientCount();
		std::string						getName() const;
		std::string						getTopic() const;
		std::string						getPassword() const;
		bool							getMode(std::string mode) const;
		int								getLimit() const;
		int								getClientCount() const;
		std::vector<Client*>			getAllClients() const;
		std::string						getAllModes() const;
		std::vector<Client*>			getOperators() const;
		std::vector<Client*>			getNonOperators() const;

		bool							isInvited(const std::string nickname) const;
		bool							isOperator(const std::string nickname) const;
		bool							isOnChannel(const std::string nickname) const;
		void							addClient(Client* client);
		void							removeClient(const int fd);
		void							addToInviteList(const std::string nickname);
		void 							removeFromInviteList(const std::string nickname);
		void							promoteToOperator(int fd);
		void							demoteFromOperator(int fd);
		void							removeDuplicateClientsByFD();
		std::vector<int>				getFdClientList() const;
};

#endif