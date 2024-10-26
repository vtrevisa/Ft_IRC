#ifndef LIBS_HPP
#define LIBS_HPP

#include <bits/stdc++.h>
#include <iostream>
#include <vector> //-> for vector
#include <sys/socket.h> //-> for socket()
#include <sys/types.h> //-> for socket()
#include <netinet/in.h> //-> for sockaddr_in
#include <fcntl.h> //-> for fcntl()
#include <unistd.h> //-> for close()
#include <arpa/inet.h> //-> for inet_ntoa()
#include <poll.h> //-> for poll()
#include <csignal> //-> for signal()
#include <cstring> //-> for memset()
#include <cstdlib> //-> for atoi()
//-------------------------------------------------------//
#define RED "\e[1;31m"
#define WHITE "\e[0;37m"
#define GREEN "\e[1;32m"
#define YELLOW "\e[1;33m"
#define BLUE "\e[1;36m"
//-------------------------------------------------------//
//reply and error numeric codes
# define RPL_WELCOMENBR std::string(" 001 ")
# define RPL_ENDOFWHONBR std::string(" 315 ")
# define RPL_CHANNELMODEISNBR std::string(" 324 ")
# define RPL_NOTOPICNBR std::string(" 331 ")
# define RPL_TOPICNBR std::string(" 332 ")
# define RPL_INVITINGNBR std::string(" 341 ")
# define RPL_WHOREPLYNBR std::string(" 352 ")
# define RPL_NAMREPLYNBR std::string(" 353 ")
# define RPL_ENDOFNAMESNBR std::string(" 366 ")
# define ERR_NOSUCHNICKNBR std::string(" 401 ")
# define ERR_NOSUCHCHANNELNBR std::string(" 403 ")
# define ERR_NORECIPIENTNBR std::string(" 411 ")
# define ERR_NOTEXTTOSENDNBR std::string(" 412 ")
# define ERR_NONICKNAMEGIVENNBR std::string(" 431 ")
# define ERR_ERRONEUSNICKNAMENBR std::string(" 432 ")
# define ERR_NICKNAMEINUSENBR std::string(" 433 ")
# define ERR_USERNOTINCHANNELNBR std::string(" 442 ")
# define ERR_NOTONCHANNELNBR std::string(" 442 ")
# define ERR_USERONCHANNELNBR std::string(" 443 ")
# define ERR_NEEDMOREPARAMSNBR std::string(" 461 ")
# define ERR_ALREADYREGISTEREDNBR std::string(" 462 ")
# define ERR_PASSWDMISMATCHNBR std::string(" 464 *")
# define ERR_CHANNELISFULLNBR std::string(" 471 ")
# define ERR_INVITEONLYCHANNBR std::string(" 473 ")
# define ERR_BADCHANNELKEYNBR std::string(" 475 ")
# define ERR_BADCHANMASKNBR std::string(" 476 ")
# define ERR_CHANOPRIVSNEEDEDNBR std::string(" 482 ")
# define ERR_CANNOTSENDTOCHAN std::string(" 404 ")

//reply messages
# define RPL_ENDOFWHO std::string(" :End of /WHO list ")
# define RPL_NOTOPIC std::string(" :No topic is set ")
# define RPL_ENDOFNAMES std::string(" :End of /NAMES list")
# define RPL_WELCOME std::string(" :Welcome to IRCSERV")

//error messages
# define ERR_ACCEPT std::string(" Error on accept ")
# define ERR_ALREADYREGISTERED std::string(" :You may not reregister")
# define ERR_BADCHANMASK std::string(" :Invalid channel name ")
# define ERR_BADCHANNELKEY std::string(" :Cannot join channel (+k) ")
# define ERR_BIND std::string(" Error on bind ")
# define ERR_CHANNELISFULL std::string(" :Cannot join channel (+l) ")
# define ERR_CHANOPRIVSNEEDED std::string(" :You're not channel operator ")
# define ERR_ERRONEUSNICKNAME std::string(" :Erroneus nickname ")
# define ERR_FCNTL std::string(" Error on fcntl ")
# define ERR_INVITEONLYCHAN std::string(" :Cannot join channel (+i) ")
# define ERR_LISTEN std::string(" Error on listen ")
# define ERR_NEEDMOREPARAMS std::string(" :Not enough parameters ")
# define ERR_NICKNAMEINUSE std::string(" :Nickname is already in use ")
# define ERR_NONICKNAMEGIVEN std::string(" :No nickname given ")
# define ERR_NORECIPIENT std::string(" :No recipient given ")
# define ERR_NOSUCHCHANNEL std::string(" :No such channel ")
# define ERR_NOSUCHNICK std::string(" :No such nick ")
# define ERR_NOTEXTTOSEND std::string(" :No text to send ")
# define ERR_NOTONCHANNEL std::string(" :You're not on that channel! ")
# define ERR_PASSWDMISMATCH std::string(" :Password incorrect ")
# define ERR_POLL std::string(" Error on poll ")
# define ERR_SETSOCKOPT std::string(" Error on setsockopt ")
# define ERR_SOCKET std::string(" Error creating socket ")
# define ERR_USERNOTINCHANNEL std::string(" :They are not on that channel ")
# define ERR_USERONCHANNEL std::string(" :is already on channel ")

# define END std::string("\r\n")
# define IRC std::string(":ft.irc")

#endif