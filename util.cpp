#include "util.h"
#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>

std::string util::exec(const char* cmd) {
    std::array<char, 128> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&_pclose)> pipe(_popen(cmd, "r"), _pclose);
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    return result;
}

void util::AppendToBuffer(const char* str, char* inputBuffer)
{
	// Get the current time
	time_t currentTime;
	time(&currentTime);

	// Declare a structure to hold the local time
	struct tm timeInfo;
	localtime_s(&timeInfo, &currentTime);

	// Format the time as a string
	char timeStr[12]; // Adjust the size as needed for the time format
	strftime(timeStr, sizeof(timeStr), "[%H:%M:%S]", &timeInfo);

	// Append the formatted time to the buffer
	int len = strlen(timeStr);
	memcpy(&inputBuffer[strlen(inputBuffer)], timeStr, len);
	inputBuffer[strlen(inputBuffer)] = ' '; // Add a space between time and the original string

	// Append the original string
	len = strlen(str);
	memcpy(&inputBuffer[strlen(inputBuffer)], str, len);
	inputBuffer[strlen(inputBuffer)] = '\n';
	inputBuffer[strlen(inputBuffer)] = '\0';
}