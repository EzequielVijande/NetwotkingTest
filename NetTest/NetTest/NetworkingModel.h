#pragma once
#pragma once

#include <string>
#include <vector>
#include <boost/asio/deadline_timer.hpp>
#include <boost/asio.hpp>
#include <boost/function.hpp>
#include <boost/bind.hpp>

using boost::asio::deadline_timer;

//Headers de los paquetes
#define ACK_HEADER (char)0x01
#define NAME_HEADER (char)0x10
#define NAME_IS_HEADER (char)0x11
#define YOU_START_HEADER (char)0x20
#define I_START_HEADER (char)0x21
#define R_U_READY_HEADER (char)0x22
#define I_AM_READY_HEADER (char)0x23
#define MOVE_HEADER (char)0x30
#define ATTACK_HEADER (char)0x31
#define YOU_WON_HEADER (char)0x40
#define PLAY_AGAIN_HEADER (char)0x50
#define GAME_OVER_HEADER (char)0x51
#define ERROR_HEADER (char)0xFE
#define QUIT_HEADER (char)0xFF

#define		PORT	13225
#define		PORT_C	"13225"

enum serverStatus { SERVER, CLIENT, UNINITIALIZED };
class NetworkingModel
{
public:
	NetworkingModel();
	bool sendPackage(char * message, int size); //envia un paquete de chars de tamanio size
	std::vector<char> readPackage(); // busca si llego un package 
	int getState();
	void setState(int state_);
	serverStatus getServer();
	void setServer(serverStatus server_);
	bool GetServerFinishedPlacing()const;
	void SetServerFinishedPlacing(bool);
	std::string getMe();
	void setMe(std::string me_);
	std::string getYou();
	void setYou(std::string you_);
	bool connectAsClient(int timer, char * ip); //trata de conectarse al puerto como client por un determinado tiempo
	bool connectAsServer();
	void Shutdown();
	~NetworkingModel();
private:
	int state;
	int port;
	bool server_Finished_placing_fichas;
	bool time_done;
	serverStatus serverStat;
	std::string me;
	std::string you;
	void client_connect_handler(const boost::system::error_code& error);
	void timer_handler(const boost::system::error_code& error);
	//boost
	boost::asio::io_service*  IO_handler;
	boost::asio::ip::tcp::socket* socket_a;
	boost::asio::ip::tcp::acceptor* server_acceptor;
	//boost::asio::ip::tcp::resolver* client_resolver;
	boost::asio::ip::tcp::endpoint* endpoint_a;
	deadline_timer* deadline_;
	deadline_timer* heartbeat_timer_;
};

