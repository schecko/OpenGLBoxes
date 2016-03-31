#include "debug.h"
#include <string>
#include <iostream>


void Debug::Assert(bool test, const std::string errorNotes)
{
#if (DEBUGGINGLEVEL >= 3)
    if (!test)
    {
        std::cout << errorNotes << std::endl;
        //abort(); //todo make a less extreme replacement of this line
    }

#endif
}
void Debug::SoftAssert(bool test, const std::string errorNotes)
{
#if (DEBUGGINGLEVEL >= 1)
    if (!test)
    {
        std::cout << errorNotes << std::endl;
    }

#endif
}

void Debug::Message(std::string message)
{
#if (DEBUGGINGLEVEL >= 1)
    std::cout << message << std::endl;
#endif
}

void Debug::ReleaseAssert(bool test, const std::string errorNotes)
{
    if (!test)
    {
        std::cout << errorNotes << std::endl;
        // abort(); //todo make a less extreme replacement of this line
    }

}

