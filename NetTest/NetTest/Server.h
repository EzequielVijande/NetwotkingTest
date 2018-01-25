#pragma once
#include <boost/asio.hpp>
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <iostream>
#include <fstream>
#include <string.h>

using namespace std;

#define PORT_S 12345
#define YO	1
#define ESCUCHO 2

class Server
{
public:
	Server(int port);
	~Server();

	void listening();
	bool noerrror();
	bool itsMe(const char * mi_ip);
	bool lastOne();

	char * getNext();
	char * getMsg();
	bool getQuit();
	char getAnim();
	int getipsCount();
	void closeSocket();

	void setAnim(char anim);
	void setTurno(int turno);
	void setQuit(bool stat);

	bool setBuf(int pos, char value);
	bool setSeq(int pos, char value);
	char getBuf(int pos);
	char getSeq(int pos);

private:
	bool quit_status;
	bool error_status;

	int turno;
	int ipsCount;

	char animation;

	char ipNext[16];
	char buf[512];
	char sequence[255];

	boost::asio::io_service*  IO_handler;
	boost::asio::ip::tcp::socket* socket_forServer;
	boost::asio::ip::tcp::acceptor* server_acceptor;
};
