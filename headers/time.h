#pragma once
#ifndef UNICODE
#define UNICODE
#endif 

#include <chrono>

class Time
{
public:
private:
std::chrono::system_clock::time_point prevTime = std::chrono::system_clock::now();
std::chrono::system_clock::time_point currentTime = std::chrono::system_clock::now();
float deltaTime;
public:
Time();
void calculateDeltaTime();
float getDeltaTime();
void resetTime();
private:
};