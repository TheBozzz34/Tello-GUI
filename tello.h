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
	int takeoff();
	int land();
	int emergency();
	int up(int x);
	int down(int x);
	int left(int x);
	int right(int x);
	int forward(int x);
	int back(int x);
	int cw(int x);
	int ccw(int x);
	int flip(std::string direction);
	int go(int x, int y, int z, int speed);
	int stop();
	int curve(int x1, int y1, int z1, int x2, int y2, int z2, int speed);
	int setSpeed(int x);
	std::string getSN() const;
	std::string getSpeed() const;
	std::string getBattery() const;
	std::string getTime() const;
	std::string getWifi() const;
};

