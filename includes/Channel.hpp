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
		std::map<std::string, Client*>	_clients;
		std::map<std::string, Client*>	_operators;

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
		std::map<std::string, Client*>	getOperators() const;
		std::map<std::string, Client*>	getNonOperators() const;
		std::string						getChannelClientsList() const;

		bool							isInvited(const std::string clientname) const;
		bool							isOperator(const std::string clientname) const;
		bool							isOnChannel(const std::string clientname) const;
		void							addClient(Client* client);
		bool							removeClient(const std::string& clientname);
		void							addToInviteList(const std::string clientname);
		void 							removeFromInviteList(const std::string clientname);
		void							promoteToOperator(std::string clientname);
		void							demoteFromOperator(std::string clientname);
		void							listClients() const;
		void							listOperators() const;
};

#endif