#pragma once

#include <ctime>

class Clock
{
public:
    Clock();
    Clock(const Clock& other);
    ~Clock();

    void UpdateFrameTimes();

    double GetTimeSinceStart();
    double GetDeltaFrameTime();

    //tick stores a custom frame time, and tock will
    //calcuate the difference between the times of tick and tock then return it.
    //USING THIS FUNCTION WILL OVERRIDE THE LAST USE
    void Tick();
    double Tock();
private:
    double lastFrameTime;
    double thisFrameTime;
    double deltaFrameTime;

    double tickTime;
};