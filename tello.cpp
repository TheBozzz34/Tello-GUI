#include "tello.h"
#include <iostream>
#include <thread>
#include <chrono>

tello::tello(std::string ip, int port)
{
    this->ip = ip;
    this->port = port;
    this->connected = false;
}

void sendWaitingMessage()
{
    for (int i = 0; i < 5; ++i) {
        std::cout << "Waiting for response..." << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

int tello::send_command(const std::string& command)
{

    auto connection_thread = [this, command]() {

        boost::asio::io_service io_service;
        boost::asio::ip::udp::socket socket(io_service);
        boost::asio::ip::udp::endpoint remote_endpoint;

        socket.open(boost::asio::ip::udp::v4());

        remote_endpoint = boost::asio::ip::udp::endpoint(boost::asio::ip::address::from_string(this->ip), this->port);

        boost::system::error_code err;
        socket.send_to(boost::asio::buffer(command), remote_endpoint, 0, err);

        if (err)
        {
            std::cout << "Error: " << err.message() << std::endl;
            return;  // Don't proceed if there was an error sending the command.
        }
        else
        {
            std::cout << "Command sent!" << std::endl;
        }

        boost::asio::ip::udp::endpoint receive_endpoint(boost::asio::ip::address::from_string(this->ip), this->port);

        // Set a timeout for the receive operation (5 seconds in this example).
        boost::asio::deadline_timer timer(io_service);
        timer.expires_from_now(boost::posix_time::seconds(5));
        std::thread messageThread(sendWaitingMessage);

        timer.async_wait([&](const boost::system::error_code& ec) {
            if (!ec)
            {
                // Timeout occurred, handle the timeout here.
                std::cout << "Timeout: Response not received within 5 seconds." << std::endl;
                socket.cancel();  // Cancel the receive operation.
            }
            });

        messageThread.join();

        socket.async_receive_from(boost::asio::buffer(this->buffer), receive_endpoint,
            [&](const boost::system::error_code& recv_error, std::size_t bytes_received) {
                timer.cancel();  // Cancel the timer since we received a response.
                if (!recv_error)
                {
                    std::cout << "Response: " << this->buffer << std::endl;

                }
                else
                {
                    std::cout << "Error receiving response: " << recv_error.message() << std::endl;
                }
            });

        io_service.run();  // Start the asynchronous operations.
    };

    std::thread thread(connection_thread);
    thread.detach(); // Deta

    return 0;

    
}

void tello::takeoff()
{
	this->send_command("takeoff");
}

void tello::testConnection()
{
    auto connection_thread = [this]() {

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
            return;  // Don't proceed if there was an error sending the command.
        }
        else
        {
            std::cout << "Command sent!" << std::endl;
        }

        boost::asio::ip::udp::endpoint receive_endpoint(boost::asio::ip::address::from_string(this->ip), this->port);

        socket.async_receive_from(boost::asio::buffer(this->buffer), receive_endpoint,
            [&](const boost::system::error_code& recv_error, std::size_t bytes_received) {
                if (!recv_error)
                {
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
                else
                {
                    std::cout << "Error receiving response: " << recv_error.message() << std::endl;
                }
            });

        io_service.run();  // Start the asynchronous operations.
        };

    std::thread thread(connection_thread);
    thread.detach(); // Deta
}


bool tello::isConnected() const
{
    return this->connected;
}

std::string tello::getSN() const
{
	return this->sn;
}

std::string tello::getSpeed() const
{
	return this->speed;
}

std::string tello::getBattery() const
{
	return this->battery;
}

std::string tello::getTime() const
{
	return this->time;
}

std::string tello::getWifi() const
{
	return this->wifi;
}



