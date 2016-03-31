#include "clock.h"


#include <ctime>

#include <cstdlib>

Clock::Clock() : lastFrameTime(0.0f),
    thisFrameTime(0.0f),
    deltaFrameTime(0.0f),
    tickTime(0.0f)
{
    srand(time(nullptr));
}

Clock::Clock(const Clock & other)
{
}

Clock::~Clock()
{
}

void Clock::UpdateFrameTimes()
{
    lastFrameTime = thisFrameTime;
    thisFrameTime = GetTimeSinceStart();
    deltaFrameTime = thisFrameTime - lastFrameTime;

}


double Clock::GetTimeSinceStart()
{
    clock_t cyclesSinceStart = clock();

    double secondsSinceStart = (double) cyclesSinceStart / (double) (CLOCKS_PER_SEC * 1000);

    return secondsSinceStart;
}

double Clock::GetDeltaFrameTime()
{
    return deltaFrameTime;
}

void Clock::Tick()
{
    tickTime = GetTimeSinceStart();
}

double Clock::Tock()
{
    return GetTimeSinceStart() - tickTime;
}
