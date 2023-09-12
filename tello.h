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
	using CommandCallback = std::function<void(int)>;
	tello(std::string ip, int port);
	void testConnection();
	void refreshData();
	bool isConnected() const;
	void send_command(const std::string& command, CommandCallback callback);
	void takeoff(CommandCallback callback);
	void land(CommandCallback callback);
	void emergency(CommandCallback callback);
	void up(int x, CommandCallback callback);
	void down(int x, CommandCallback callback);
	void left(int x, CommandCallback callback);
	void right(int x, CommandCallback callback);
	void forward(int x, CommandCallback callback);
	void back(int x, CommandCallback callback);
	void cw(int x, CommandCallback callback);
	void ccw(int x, CommandCallback callback);
	void flip(std::string direction, CommandCallback callback);
	void go(int x, int y, int z, int speed, CommandCallback callback);
	void stop(CommandCallback callback);
	void curve(int x1, int y1, int z1, int x2, int y2, int z2, int speed, CommandCallback callback);
	void setSpeed(int x, CommandCallback callback);
	std::string getSN() const;
	std::string getSpeed() const;
	std::string getBattery() const;
	std::string getTime() const;
	std::string getWifi() const;
};

