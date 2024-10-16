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
#define MODE "/mode - check/set channel modes\r\n       Usage: /mode <channel name> <+/-><mode> (opitional)<complement>\r\n"
#define INVITE "/invite - invite client to channel\r\n       Usage: /invite <client nickname to be invited> <channel name>\r\n"
#define TOPIC "/topic - check/set channel topic\r\n       Usage: /topic <channel name> (optional)<topic in double quotes>\r\n"
#define KICK "/kick - kick client from channel\r\n       Usage: /kick <client nickname to be kicked> <channel name> (optional)<reason in double quotes>\r\n"
#define JOIN "/join - join/create a channel\r\n       Usage: /join <channel name> (opitional)<password>\r\n"
#define CHANNEL "/channel - send message on a channel\r\n       Usage: /channel <channel name> <message in double quotes>\r\n"
#define EXIT "/exit - exit server\r\n       Usage: /exit\r\n"
#define QUIT "/quit - quit channel\r\n       Usage: /quit <channel name>\r\n"
#define NICK "/nickname - set nickname\r\n       Usage: \r\n" //TO BE DONE
#define USER "/username - set useuname\r\n       Usage: \r\n" //TO BE DONE
#define PRIVATEMSG "/pmsg - send private message to a client\r\n       Usage: /pmsg <client nickname to send message> <message in double quotes>\r\n"

#endif