#include "tello.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <spdlog/spdlog.h>
#include <future>

tello::tello(boost::asio::io_service& io_service, boost::asio::ip::udp::socket& socket, const boost::asio::ip::udp::endpoint& remote_endpoint)
    : ip(remote_endpoint.address().to_string()),
    port(remote_endpoint.port()),
    io_service(io_service),
    socket(socket),
    remote_endpoint(remote_endpoint)
{

}



int tello::send_command(const std::string& command)
{
    boost::system::error_code err;
    socket.send_to(boost::asio::buffer(command), remote_endpoint, 0, err);

    if (err)
    {
        spdlog::error("Error sending command: {}", err.message());

        return -1;
    }
    else
    {
        spdlog::info("Command sent!");
    }

    return 0;
}

void tello::ReceiveThread() {
    while (isListening_) {
        try {
            char receivedData[1024];
            boost::system::error_code error;

            size_t len = socket.receive_from(boost::asio::buffer(receivedData), remote_endpoint, 0, error);

            if (error && error != boost::asio::error::message_size) {
                throw boost::system::system_error(error);
            }

            receivedData[len] = '\0';

            std::cout << "Received UDP data: " << receivedData << std::endl;
        }
        catch (const std::exception& e) {
            std::cerr << "Error receiving UDP data: " << e.what() << std::endl;
        }
    }
}

void tello::StartListening() {
    std::thread udpThread(&tello::ReceiveThread, this);
    udpThread.detach(); // Detach the thread so it runs independently
}

void tello::StopListening() {
    isListening_ = false;
}



void tello::takeoff()
{
    this->send_command("takeoff");
}

void tello::land()
{
    this->send_command("land");
}

void tello::emergency()
{
    this->send_command("emergency");
}

void tello::up(int x)
{
    this->send_command("up " + std::to_string(x));
}

void tello::down(int x)
{
    this->send_command("down " + std::to_string(x));
}

void tello::left(int x)
{
    this->send_command("left " + std::to_string(x));
}

void tello::right(int x)
{
    this->send_command("right " + std::to_string(x));
}

void tello::forward(int x)
{
    this->send_command("forward " + std::to_string(x));
}

void tello::back(int x)
{
    this->send_command("back " + std::to_string(x));
}

void tello::cw(int x)
{
    this->send_command("cw " + std::to_string(x));
}

void tello::ccw(int x)
{
    this->send_command("ccw " + std::to_string(x));
}

void tello::flip(std::string direction)
{
    this->send_command("flip " + direction);
}

void tello::go(int x, int y, int z, int speed)
{
    this->send_command("go " + std::to_string(x) + " " + std::to_string(y) + " " + std::to_string(z) + " " + std::to_string(speed));
}

void tello::stop()
{
    this->send_command("stop");
}

void tello::curve(int x1, int y1, int z1, int x2, int y2, int z2, int speed)
{
    this->send_command("curve " + std::to_string(x1) + " " + std::to_string(y1) + " " + std::to_string(z1) + " " + std::to_string(x2) + " " + std::to_string(y2) + " " + std::to_string(z2) + " " + std::to_string(speed));
}

void tello::setSpeed(int x)
{
    this->send_command("speed " + std::to_string(x));
}

void tello::initConnection()
{
    this->send_command("command");
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



