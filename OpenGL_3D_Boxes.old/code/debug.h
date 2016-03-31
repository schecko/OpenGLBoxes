#pragma once

#include <string>


//set the debugging lebel, 0 is no debugging and 3 is the highest debugging level
#define DEBUGGINGLEVEL 3

#define DEBUGGING_GRAPHICS 0x1
#define DEBUGGING_PLATFORM 0x2

//The debugging class is designed to not be used in the release version, and all functions have
//preprocessor checks for the DEBUGGING flag above. If DEBUGGING is undefined, the functions become
//empty bodies and the optimizer *should* (needs further checking) inline them and in effect erase them.

//At the time of writing, all functions are static but this may change later.

class Debug
{
public:

    enum
    {
        GLFW_FAILED,
        GLEW_FAILED,
        PLATFORM_FAILED,
        GRAPHICS_FAILED,
        VERTEXSHADER_FAILED,
        FRAGMENTSHADER_FAILED,
        SHADERPROGRAM_FAILED

    };

    static void Message(std::string message);
    //assert that the value holds true, if false then program aborts or may pause
    static void Assert(bool test, const std::string errorNotes);
    //asserts that the test holds true, gives a warning if false
    //called "softassert" instead of "warning" to promote proper use of the test
    static void SoftAssert(bool test, const std::string errorNotes);
    //this function will be called independant of debugging level
    //and is reserved for asserts that must be true for the game to continue
    static void ReleaseAssert(bool test, const std::string errorNotes);

};
