#pragma once
#ifndef UNICODE
#define UNICODE
#endif 

#include <chrono>

class time
{
public:
private:
std::chrono::system_clock::time_point prevTime = std::chrono::system_clock::now();
std::chrono::system_clock::time_point currentTime = std::chrono::system_clock::now();
float deltaTime;
public:
time();
void calculateDeltaTime();
float getDeltaTime();
private:
};