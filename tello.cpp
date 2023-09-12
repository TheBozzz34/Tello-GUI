#include "tello.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <spdlog/spdlog.h>
#include <future>

tello::tello(std::string ip, int port)
{
    this->ip = ip;
    this->port = port;
    this->connected = false;
}

using CommandCallback = std::function<void(int)>;

void tello::send_command(const std::string& command, CommandCallback callback)
{

    //std::promise<bool> resultPromise; // Promise to hold the result (success/failure)
    //auto futureResult = resultPromise.get_future();

    auto connection_thread = [this, command, callback]() {

        boost::asio::io_service io_service;
        boost::asio::ip::udp::socket socket(io_service);
        boost::asio::ip::udp::endpoint remote_endpoint;

        socket.open(boost::asio::ip::udp::v4());

        remote_endpoint = boost::asio::ip::udp::endpoint(boost::asio::ip::address::from_string(this->ip), this->port);

        boost::system::error_code err;
        socket.send_to(boost::asio::buffer(command), remote_endpoint, 0, err);

        if (err)
        {
            spdlog::error("Error: {}", err.message());
            callback(0);
            // resultPromise.set_value(false); // Command failed
            return;  // Don't proceed if there was an error sending the command.
        }
        else
        {
            spdlog::info("Command sent!");
        }

        boost::asio::ip::udp::endpoint receive_endpoint(boost::asio::ip::address::from_string(this->ip), this->port);

        // Set a timeout for the receive operation (5 seconds in this example).
        boost::asio::deadline_timer timer(io_service);
        timer.expires_from_now(boost::posix_time::seconds(5));

        timer.async_wait([&](const boost::system::error_code& ec) {
            if (!ec)
            {
                // Timeout occurred, handle the timeout here.
                //spdlog::error("The command {} timed out because the drone did not respond, check the connection.", command);
                socket.cancel();  // Cancel the receive operation.
                //callback(0);
            }
            });

        socket.async_receive_from(boost::asio::buffer(this->buffer), receive_endpoint,
            [&](const boost::system::error_code& recv_error, std::size_t bytes_received) {
                timer.cancel();  // Cancel the timer since we received a response.
                if (!recv_error)
                {
                    spdlog::info("Response: {}", this->buffer);
                    //resultPromise.set_value(true); // Command succeeded
                    callback(1);

                }
                else
                {
                    spdlog::error("Error receiving response: {}", recv_error.message());
                    spdlog::error("Check the connection to the drone.");
                   // resultPromise.set_value(false); // Command failed
                    callback(0);
                }
            });

        io_service.run();  // Start the asynchronous operations.
    };

    std::thread thread(connection_thread);
    thread.detach();


    //futureResult.wait(); // Wait for the result (success/failure)

    //return futureResult.get() ? 0 : 1; // Return 0 for success, 1 for failure
}

void tello::takeoff(CommandCallback callback)
{
    this->send_command("takeoff", callback);
}

void tello::land(CommandCallback callback)
{
    this->send_command("land", callback);
}

void tello::emergency(CommandCallback callback)
{
    this->send_command("emergency", callback);
}

void tello::up(int x, CommandCallback callback)
{
    this->send_command("up " + std::to_string(x), callback);
}

void tello::down(int x, CommandCallback callback)
{
    this->send_command("down " + std::to_string(x), callback);
}

void tello::left(int x, CommandCallback callback)
{
    this->send_command("left " + std::to_string(x), callback);
}

void tello::right(int x, CommandCallback callback)
{
    this->send_command("right " + std::to_string(x), callback);
}

void tello::forward(int x, CommandCallback callback)
{
    this->send_command("forward " + std::to_string(x), callback);
}

void tello::back(int x, CommandCallback callback)
{
    this->send_command("back " + std::to_string(x), callback);
}

void tello::cw(int x, CommandCallback callback)
{
    this->send_command("cw " + std::to_string(x), callback);
}

void tello::ccw(int x, CommandCallback callback)
{
    this->send_command("ccw " + std::to_string(x), callback);
}

void tello::flip(std::string direction, CommandCallback callback)
{
    this->send_command("flip " + direction, callback);
}

void tello::go(int x, int y, int z, int speed, CommandCallback callback)
{
    this->send_command("go " + std::to_string(x) + " " + std::to_string(y) + " " + std::to_string(z) + " " + std::to_string(speed), callback);
}

void tello::stop(CommandCallback callback)
{
    this->send_command("stop", callback);
}

void tello::curve(int x1, int y1, int z1, int x2, int y2, int z2, int speed, CommandCallback callback)
{
    this->send_command("curve " + std::to_string(x1) + " " + std::to_string(y1) + " " + std::to_string(z1) + " " + std::to_string(x2) + " " + std::to_string(y2) + " " + std::to_string(z2) + " " + std::to_string(speed), callback);
}

void tello::setSpeed(int x, CommandCallback callback)
{
    this->send_command("speed " + std::to_string(x), callback);
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
            spdlog::error("Error: {}", err.message());
            return;  // Don't proceed if there was an error sending the command.
        }
        else
        {
            spdlog::info("Command sent!");
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
                    spdlog::error("Error receiving response: {}", recv_error.message());
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



