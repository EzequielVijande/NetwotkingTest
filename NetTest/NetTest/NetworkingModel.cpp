#include "NetworkingModel.h"
#include <iostream>


NetworkingModel::NetworkingModel()
{
	boost::asio::io_service* IO_handler = new boost::asio::io_service;
	deadline_ = new deadline_timer(*IO_handler);
	heartbeat_timer_ = new deadline_timer(*IO_handler);
	time_done = false;
	socket_a = new boost::asio::ip::tcp::socket(*IO_handler);
	socket_a->non_blocking(true);
	serverStat = UNINITIALIZED;
	server_Finished_placing_fichas = false;
	server_acceptor = nullptr;
	endpoint_a = nullptr;

}

bool NetworkingModel::sendPackage(char * message, int size)
{
	size_t len;
	boost::system::error_code error;
	do
	{
		len = socket_a->write_some(boost::asio::buffer(message, size), error);
	} while ((error.value() == WSAEWOULDBLOCK));
	if (!error)
	{
		return true;
	}
	else
	{
		return false;
	}
}

std::vector<char> NetworkingModel::readPackage()
{
	std::vector<char> aux;
	size_t len = 0;
	boost::system::error_code error;
	do
	{
		len = socket_a->read_some(boost::asio::buffer(aux), error);

	} while (error.value() == WSAEWOULDBLOCK);
	return aux;
}

int NetworkingModel::getState()
{
	return state;
}

void NetworkingModel::setState(int state_)
{
	state = state_;
}

serverStatus NetworkingModel::getServer()
{
	return serverStat;
}

void NetworkingModel::setServer(serverStatus server_)
{
	serverStat = server_;
}

std::string NetworkingModel::getMe()
{
	return me;
}

void NetworkingModel::setMe(std::string me_)
{
	me = me_;
}

std::string NetworkingModel::getYou()
{
	return you;
}

void NetworkingModel::setYou(std::string you_)
{
	you = you_;
}


bool NetworkingModel::GetServerFinishedPlacing()const
{
	return server_Finished_placing_fichas;
}

void NetworkingModel::SetServerFinishedPlacing(bool value)
{
	server_Finished_placing_fichas = value;
}

bool NetworkingModel::connectAsClient(int time, char * ip)
{
	//client_resolver = new boost::asio::ip::tcp::resolver(*IO_handler);
	//endpoint = client_resolver->resolve(boost::asio::ip::tcp::resolver::query(ip, PORT_C));
	endpoint_a = new boost::asio::ip::tcp::endpoint(boost::asio::ip::address::from_string(ip), PORT);
	std::cout << "Trying to connect to " << ip << " on port " << PORT_C << std::endl;
	deadline_->expires_from_now(boost::posix_time::milliseconds(time)); //Tiempo a tratar la conexion.
	deadline_->async_wait(&NetworkingModel::timer_handler);
	socket_a->async_connect(*endpoint_a,
		boost::bind(&NetworkingModel::client_connect_handler, this,
			socket_a,
			boost::asio::placeholders::error)
	);
	//boost::asio::async_connect(*socket, endpoint, &NetworkingModel::client_connect_handler); //Creo que seria asi para que funque con timer
	while ((!time_done) && (serverStat != CLIENT))
	{
		//Espera hasta que se conecte como cliente o hasta que termine el tiempo de time out.
	};

	if (time_done)
	{
		time_done = false;
		return false;
	}
	else
	{
		return true;
	}

}

bool NetworkingModel::connectAsServer()
{
	server_acceptor = new boost::asio::ip::tcp::acceptor(*IO_handler,
		boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), PORT));
	boost::system::error_code error;
	std::cout << std::endl << "Ready. Port " << PORT << " created" << std::endl;
	server_acceptor->accept(*socket_a, error);
	if (!error)
	{
		std::cout << "Established Connection as server" << std::endl;
		return true;
	}
	else //error connecting to client
	{
		std::cout << error.message() << std::endl;
		return false;
	}

}


NetworkingModel::~NetworkingModel()
{
	socket_a->close();
	delete socket_a;
	delete endpoint_a;
	delete IO_handler;
	if (server_acceptor != nullptr)
	{
		server_acceptor->close();
		delete server_acceptor;
	}
}

void NetworkingModel::Shutdown()
{
	time_done = true;
	boost::system::error_code ignored_ec;
	(*socket_a).close(ignored_ec);
	deadline_->cancel();
	heartbeat_timer_->cancel();
}


void NetworkingModel::client_connect_handler(const boost::system::error_code& error)
{

	if (!error)
	{
		deadline_->expires_at(boost::posix_time::pos_infin); //Para que no vuelva a llamarlo hasta que se defina nuevo tiempo.
		std::cout << "Connected Succesfully As Client." << std::endl;
		serverStat = CLIENT;
	}
	else
	{
		std::cout << error.message() << std::endl;
	}
}

void NetworkingModel::timer_handler(const boost::system::error_code& error)
{
	if (!error)
	{
		//Expiro el timer.
		time_done = true;
		//socket_a->cancel();
		socket_a->close(); //Interrumpe la conexion.
		deadline_->expires_at(boost::posix_time::pos_infin); //Para que no vuelva a llamarlo hasta que se defina nuevo tiempo.
	}
	else if (error && error != boost::asio::error::operation_aborted)
	{
		std::cout << error.message() << std::endl;
		//socket_a->cancel();
		socket_a->close(); //Interrumpe la conexion.
	}
}