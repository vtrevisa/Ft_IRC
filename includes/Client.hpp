#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "Libs.hpp"

typedef enum {
	UNAUTHORIZED,
	CONNECTED,
	OPERATOR
} status;
class Client //-> class for client
{
	private:
		int			_Fd; //-> client file descriptor
		std::string _IPadd; //-> client ip address
		std::string	_nickname;
		std::string	_clientname;
		std::string _password;
		status		_status;

	public:
		Client(); //-> default constructor
		~Client();
		Client(const Client& other); // Copy constructor
		Client& operator=(const Client& other); // Copy assignment operator

		std::string	clientBuff;

		void 		setClientname(std::string clientname);
		void 		setNickname(std::string nickname);
		void		setIpAdd(std::string ipadd); //-> setter for ipadd
		void		SetFd(int fd); //-> setter for fd
		void 		setPassword(std::string password);
		void		setStatus(status status);
		std::string	getClientname();
		std::string	getNickname();
		std::string	getIpAdd();
		int			getFd(); //-> getter for fd
		status		getStatus();
		std::string	getPassword();
		bool		isAuth();
};

#endif