#pragma once
#include <iostream>
#include <boost/asio.hpp>
#include <map>
#include <cassert>
#include <string>
class tello
{
private:
	std::string ip;
	int port;
	bool connected;
	char buffer[1024];
	std::string sn;
	std::string speed;
	std::string battery;
	std::string time;
	std::string wifi;
public:
	tello(std::string ip, int port);
	void testConnection();
	void refreshData();
	bool isConnected() const;
	int send_command(const std::string& command);
	void takeoff();
	std::string getSN() const;
	std::string getSpeed() const;
	std::string getBattery() const;
	std::string getTime() const;
	std::string getWifi() const;
};

