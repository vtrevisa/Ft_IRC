#include "Client.hpp"

Client::Client() {}

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

std::string Client::getClientname() {
	return _clientname;
}

std::string Client::getNickname() {
	return _nickname;
}

std::string Client::getIpAdd() {
	return _IPadd;
}
int Client::GetFd() {
	return _Fd;
}

std::string Client::getPassword() {
	return _password;
}