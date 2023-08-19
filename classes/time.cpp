#include "../headers/time.h"

Time::Time()
    :prevTime(std::chrono::system_clock::now()), currentTime(std::chrono::system_clock::now()), deltaTime(((std::chrono::duration<float>)(currentTime - prevTime)).count())
{
    
}

void Time::calculateDeltaTime()
{
	currentTime = std::chrono::system_clock::now();
	deltaTime = ((std::chrono::duration<float>)(currentTime - prevTime)).count();
    prevTime = currentTime;
}

float Time::getDeltaTime()
{
    return deltaTime;
}

void Time::resetTime()
{
    currentTime = std::chrono::system_clock::now();
    prevTime = std::chrono::system_clock::now();
    deltaTime = 0.0;
}