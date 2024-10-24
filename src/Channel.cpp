#include "Channel.hpp"

Channel::Channel(const std::string& name) : _name(name) {
	_topic = "";
	initModes(_modes);
	_password = "";
	_clientCount = 0;
	_limit = 1000;
}

Channel::~Channel() {
	_clients.clear();
	_operators.clear();
}

void Channel::initModes(std::map<std::string, bool>& modes) {
	modes["i"] = false;
	modes["t"] = false;
	modes["k"] = false;
	modes["o"] = false;
	modes["l"] = false;
}

void Channel::setName(const std::string name) {
	this->_name = name;
}

void Channel::setTopic(const std::string topic) {
	_topic = topic;
}

void Channel::setPassword(std::string password) {
	_password = password;
}

void Channel::setMode(std::string mode, bool value) {
	_modes[mode] = value;
}

void Channel::setLimit(int limit) {
	_limit = limit;
}

void Channel::increaseClientCount() {
	_clientCount++;
}

void Channel::decreaseClientCount() {
	_clientCount--;
}

std::string Channel::getName() const {
	return this->_name;
}

std::string Channel::getTopic() const {
	return this->_topic;
}

std::string Channel::getPassword() const {
	return this->_password;
}

bool Channel::getMode(std::string mode) const {
	std::map<std::string, bool>::const_iterator it = _modes.find(mode);
	if (it != _modes.end())
		return it->second;
	return false;
}

int Channel::getLimit() const {
	return _limit;
}

int Channel::getClientCount() const {
	return _clientCount;
}

std::vector<Client*> Channel::getAllClients() const {
	std::vector<Client*> allClients;

	for (size_t i = 0; i < _clients.size(); ++i)
		allClients.push_back(_clients[i]);

	return allClients;
}

std::string Channel::getAllModes() const {
	std::string modes = "+";
	if (_modes.at("o")) modes += "o";
	if (_modes.at("t")) modes += "t";
	if (_modes.at("i")) modes += "i";
	if (_modes.at("k")) modes += "k";
	if (_modes.at("l")) modes += "l";
	return modes;
}

std::vector<Client*> Channel::getOperators() const {
    return this->_operators;
}

std::vector<Client*> Channel::getNonOperators() const {
    return this->_clients;
}

bool Channel::isInvited(const std::string nickname) const {
	for (std::vector<std::string>::const_iterator it = _inviteList.begin(); it != _inviteList.end(); ++it)
		if (*it == nickname)
			return true;
	return false;
}

bool Channel::isOperator(const std::string nickname) const {
	bool isOperator = false;

	for (size_t i = 0; i < _clients.size(); i++)
		if (nickname == _clients[i]->getNickname())
			isOperator = true;

    return isOperator;
}

bool Channel::isOnChannel(const std::string nickname) const {
	bool isOnChannel = false;

	std::vector<Client*> clients = getAllClients();
	for (size_t i = 0; i < clients.size(); i++)
		if (nickname == clients[i]->getNickname())
			isOnChannel = true;

    return isOnChannel;
}

void Channel::addClient(Client* client) {
	_clients.push_back(client);
	increaseClientCount();
}

void Channel::removeClient(const std::string& nickname) {
	// std::vector<Client*> clientList = getAllClients();
	for (size_t i = 0; i < _clients.size(); ++i) {
		if (_clients[i]->getNickname() == nickname) {
			_clients.erase(_clients.begin() + i);
			decreaseClientCount();
		}
	}

	for (size_t i = 0; i < _operators.size(); ++i)
		if (_operators[i]->getNickname() == nickname)
			_operators.erase(_operators.begin() + i);
}

void Channel::addToInviteList(const std::string nickname) {
    _inviteList.push_back(nickname);
}

void Channel::removeFromInviteList(const std::string nickname) {
    for (std::vector<std::string>::iterator it = _inviteList.begin(); it != _inviteList.end(); ++it) {
        if (*it == nickname) {
            _inviteList.erase(it);
            break;
        }
    }
}

void Channel::promoteToOperator(const std::string nickname) {
	for (size_t i = 0; i < _clients.size(); ++i)
		if (_clients[i]->getNickname() == nickname)
			_operators.push_back(_clients[i]);
}

void Channel::demoteFromOperator(const std::string nickname) {
	for (size_t i = 0; i < _clients.size(); ++i)
		if (_clients[i]->getNickname() == nickname)
			_operators.erase(_clients.begin() + i);
}