#ifndef CLIENT_HPP
#define CLIENT_HPP


class Client //-> class for client
{
	private:
		int			_Fd; //-> client file descriptor
		std::string _IPadd; //-> client ip address
		std::string	_nickname;
		std::string	_clientname;
		std::string _password;

	public:
		Client(){}; //-> default constructor
		~Client();

		void 		setClientname(std::string clientname);
		void 		setNickname(std::string nickname);
		void		setIpAdd(std::string ipadd){_IPadd = ipadd;} //-> setter for ipadd
		void		SetFd(int fd){_Fd = fd;} //-> setter for fd
        void 		setPassword(std::string password);	
		std::string	getClientname();
		std::string	getNickname();
		std::string	getIpAdd();
		int			GetFd(){return _Fd;} //-> getter for fd
		std::string	getPassword();
};

#endif