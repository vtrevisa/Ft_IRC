#include "Client.hpp"

Client::Client() : _status(UNAUTHORIZED){}

Client::~Client() {}

Client::Client(const Client& other) {
	_clientname = other._clientname;
	_nickname = other._nickname;
	_IPadd = other._IPadd;
	_Fd = other._Fd;
	_password = other._password;
	_status = other._status;
}

Client& Client::operator=(const Client& other) {
	if (this != &other) {
		_clientname = other._clientname;
		_nickname = other._nickname;
		_IPadd = other._IPadd;
		_Fd = other._Fd;
		_password = other._password;
		_status = other._status;
	}
	return *this;
}

void Client::setClientname(std::string clientname) {
	_clientname = clientname;
}

void Client::setNickname(std::string nickname) {
	_nickname = nickname;
}

void Client::setIpAdd(std::string ipadd) {
	_IPadd = ipadd;
}

void Client::SetFd(int fd) {
	_Fd = fd;
}

void Client::setPassword(std::string password) {
	_password = password;
}

void Client::setStatus(status status) {
	_status = status;
}

std::string Client::getClientname() {
	return _clientname;
}

std::string Client::getNickname() {
	return _nickname;
}

std::string Client::getIpAdd() {
	return _IPadd;
}
int Client::getFd() {
	return _Fd;
}

std::string Client::getPassword() {
	return _password;
}

status Client::getStatus() {
	return _status;
}

bool Client::isAuth() {
	if(this->_clientname.empty() || this->_nickname.empty() || this->_password.empty())
		return false;
	return true;
}