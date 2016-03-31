#include "clock.h"


#include <ctime>

Clock::Clock()
{
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

    double secondsSinceStart = (double) cyclesSinceStart / (double) CLOCKS_PER_SEC;

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
