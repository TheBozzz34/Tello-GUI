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
public:
	void testConnection();
	void sendTakeOff();
	void sendLand();
	void sendUp(int x);
	void sendDown(int x);
	void sendLeft(int x);
	void sendRight(int x);
	void sendForward(int x);
	void sendBack(int x);
	void sendRotateClockwise(int x);
	void sendRotateCounterClockwise(int x);
	void sendFlip(std::string direction);
	void sendGo(int x, int y, int z, int speed);
	void sendCurve(int x1, int y1, int z1, int x2, int y2, int z2, int speed);
	void sendSetSpeed(int x);
	void sendStop();
	void readResponse();
	void readSpeed();
	void readBattery();
	void readTime();
	void readWifi();
	void readSdk();
	void readSn();
	tello(std::string ip, int port);
};

