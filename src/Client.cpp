#include "../includes/Client.hpp"

Client::Client() : _status(UNAUTHORIZED){}

Client::~Client() {}

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