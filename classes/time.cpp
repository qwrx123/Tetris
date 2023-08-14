#include "../headers/time.h"

time::time()
    :prevTime(std::chrono::system_clock::now()), currentTime(std::chrono::system_clock::now()), deltaTime(((std::chrono::duration<float>)(currentTime - prevTime)).count())
{
    
}

void time::calculateDeltaTime()
{
	currentTime = std::chrono::system_clock::now();
	deltaTime = ((std::chrono::duration<float>)(currentTime - prevTime)).count();
    prevTime = currentTime;
}

float time::getDeltaTime()
{
    return deltaTime;
}