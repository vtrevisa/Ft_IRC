#include "Channel.hpp"

Channel::Channel(const std::string& name) : _name(name) {
	_topic = "";
	initModes(_modes);
	_password = "";
	_clientCount = 0;
	_limit = 10;
}

Channel::~Channel() {
	_clients.clear();
	_operators.clear();
}

void Channel::initModes(std::map<std::string, bool>& modes) {
	modes["i"] = false;
	modes["t"] = true;
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
	return this->_modes.at(mode);
}

int Channel::getLimit() const {
	return _limit;
}

int Channel::getClientCount() const {
	return _clientCount;
}

std::vector<Client*> Channel::getAllClients() const {
	std::vector<Client*> allClients;
	
	for (std::map<std::string, Client*>::const_iterator it = _operators.begin(); it != _operators.end(); it++)
		allClients.push_back(it->second);
	for (std::map<std::string, Client*>::const_iterator it = _clients.begin(); it != _clients.end(); it++)
		allClients.push_back(it->second);
	
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

std::map<std::string, Client*> Channel::getOperators() const {
    return this->_operators;
}

std::map<std::string, Client*> Channel::getNonOperators() const {
    return this->_clients;
}

std::string Channel::getChannelClientsList() const {
	std::string clientsList;
	for (std::map<std::string, Client*>::const_iterator it = _clients.begin(); it != _clients.end(); it++)
		clientsList += it->first + " ";
	for (std::map<std::string, Client*>::const_iterator it = _operators.begin(); it != _operators.end(); it++) {
		clientsList += '@';
		clientsList += it->first + " ";
	}
	return clientsList;
}

bool Channel::isInvited(const std::string clientname) const {
	for (std::vector<std::string>::const_iterator it = _inviteList.begin(); it != _inviteList.end(); ++it)
		if (*it == clientname)
			return true;
	return false;
}

bool Channel::isOperator(const std::string clientname) const {
	return _operators.find(clientname) != _operators.end();
}

bool Channel::isOnChannel(const std::string clientname) const {
    bool isOnChannel = (_clients.find(clientname) != _clients.end());
    bool isOperator = (_operators.find(clientname) != _operators.end());
    return isOnChannel || isOperator;
}

void Channel::addClient(Client* Client) {
	_clients.insert(std::make_pair(Client->getNickname(), Client));
	increaseClientCount();
}

bool Channel::removeClient(const std::string& clientname) {
    std::map<std::string, Client*>::iterator it = _clients.find(clientname);

    if (it != _clients.end()) {
        _clients.erase(it);
		return true;
    } else {
		std::cout << "This client is not on this channel!!" << std::endl;
		return false;
    }
}

void Channel::addToInviteList(const std::string clientname) {
    _inviteList.push_back(clientname);
}

void Channel::removeFromInviteList(const std::string clientname) {
    for (std::vector<std::string>::iterator it = _inviteList.begin(); it != _inviteList.end(); ++it) {
        if (*it == clientname) {
            _inviteList.erase(it);
            break;
        }
    }
}

void Channel::promoteToOperator(const std::string clientname) {
    std::map<std::string, Client*>::iterator it = _clients.find(clientname);
    if (it != _clients.end()) {
        _operators[clientname] = it->second;
        _clients.erase(it);
    }
}

void Channel::demoteFromOperator(const std::string clientname) {
    std::map<std::string, Client*>::iterator it = _operators.find(clientname);
    if (it != _operators.end()) {
        _clients[clientname] = it->second;
        _operators.erase(it);
    }
}

void Channel::listClients() const {
	std::cout << "List of Clients:\n";
	for (std::map<std::string, Client*>::const_iterator it = _clients.begin(); it != _clients.end(); it++)
		std::cout << it->second->getNickname() << std::endl;
}

void Channel::listOperators() const {
	std::cout << "List of Operators:\n";
	for (std::map<std::string, Client*>::const_iterator it = _operators.begin(); it != _operators.end(); it++)
		std::cout << it->second->getNickname() << std::endl;
}