#include "tello.h"

tello::tello(std::string ip, int port)
{
	this->ip = ip;
	this->port = port;
	this->connected = false;
}

void tello::testConnection()
{
	boost::asio::io_service io_service;
	boost::asio::ip::udp::socket socket(io_service);
	boost::asio::ip::udp::endpoint remote_endpoint;

	socket.open(boost::asio::ip::udp::v4());

	remote_endpoint = boost::asio::ip::udp::endpoint(boost::asio::ip::address::from_string(this->ip), this->port);

	boost::system::error_code err;
	socket.send_to(boost::asio::buffer("command"), remote_endpoint, 0, err);

	if (err)
	{
		std::cout << "Error: " << err.message() << std::endl;
	}
	else
	{
		std::cout << "Command sent!" << std::endl;
	}

	boost::asio::ip::udp::endpoint receive_endpoint(boost::asio::ip::address::from_string(this->ip), this->port);
	socket.receive_from(boost::asio::buffer(this->buffer), receive_endpoint);

	std::cout << "Response: " << this->buffer << std::endl;

	if (strcmp(this->buffer, "ok") == 0)
	{
		this->connected = true;
	}
	else
	{
		this->connected = false;
	}
}	
