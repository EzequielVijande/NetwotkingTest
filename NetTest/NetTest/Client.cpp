#include "Client.h"


Client::Client(const char * ip) {
	IO_handler = new boost::asio::io_service();
	socket_forClient = new boost::asio::ip::tcp::socket(*IO_handler);
	client_resolver = new boost::asio::ip::tcp::resolver(*IO_handler);
	for (int i = 0; i < 16; i++)
	{
		NextIp[i] = ip[0];
	}

}

void Client::sendSeq(char * msg) {
	char buf[MAX_BUF];
	for (int i = 0; i < MAX_BUF; i++) {
		buf[i] = msg[i];
	}
	size_t len;
	boost::system::error_code error;

	do
	{
		len = socket_forClient->write_some(boost::asio::buffer(buf, strlen(buf)), error);
	} while ((error.value() == WSAEWOULDBLOCK));
	if (error)
	{
		std::cout << "Error while trying to connect to server " << error.message() << std::endl;
	}
	else
	{
		std::cout << "Message Sent" << std::endl;
	}
}


void Client::startConnection(const char* host) {
	endpoint = client_resolver->resolve(boost::asio::ip::tcp::resolver::query(host, PORT_C));
	cout << "Trying to connect to " << host << " on port " << PORT_C << std::endl;
	boost::asio::connect(*socket_forClient, endpoint);
	socket_forClient->non_blocking(true);
}

Client::~Client() {
	socket_forClient->close();
	delete client_resolver;
	delete socket_forClient;
	delete IO_handler;
}