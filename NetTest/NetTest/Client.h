#pragma once

#include <cstdio>
#include <string>
#include<exception>
#include <boost/exception/all.hpp>
#include <boost/exception/exception.hpp>
#include <boost/asio.hpp>
#include <boost/filesystem.hpp>
#include <boost/chrono.hpp>
#include <boost/timer/timer.hpp>
#include <iostream>

#define PORT_C "12345"
#define MAX_BUF	257

using namespace std;

class Client
{
public:
	Client(const char * ip);
	~Client();
	void startConnection(const char* host);
	void sendSeq(char * msg);
private:
	boost::asio::io_service* IO_handler;
	boost::asio::ip::tcp::socket* socket_forClient;
	boost::asio::ip::tcp::resolver* client_resolver;
	boost::asio::ip::tcp::resolver::iterator endpoint;
	char NextIp[16];
};