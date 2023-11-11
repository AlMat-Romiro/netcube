#pragma once

#include <iostream>
#include <string>
#include <ctime>
#include <iomanip>
#include <chrono>

void Log(std::string message)
{
    auto now = std::chrono::system_clock::now();
    time_t now_in_tt = std::chrono::system_clock::to_time_t(now);

    std::cout << std::put_time(std::localtime(&now_in_tt), "[%X]\t") << message << std::endl;
}
