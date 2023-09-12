#pragma once
#include <iostream>
#include <string>
#include <atlstr.h>

class util
{
public:
    std::string exec(const char* cmd);
    void AppendToBuffer(const char* str, char* inputBuffer);
};
