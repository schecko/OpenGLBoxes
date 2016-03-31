#pragma once

#define USING_WINDOWS true
#define USING_GLFW false
//#define USING_IOS false
//#define USING_LUNIX false


#if USING_WINDOWS == true
#include "windowswrapper.h"
#elif USING_GLFW == true
#include "glfwwrapper.h"
#endif


#include "typedefs.h"
#include "eventlistener.h"



struct WindowInfo
{
    int width;
    int	height;
    WindowState windowState;
};



class Platform
{
public:
    Platform();
    Platform(EventListener* events);
    Platform(const Platform& other);
    ~Platform();

    void Init(ui32 width,
              ui32 height,
              const char* name);

    void vSync(int frameRate);

    void PollEvents(EventListener* events);
    void SwapBuffers();

    WindowInfo* GetWindowInfo();
    void UpdateWindowInfo(WindowEvents windowEvents);

    File ReadEntireFile(char* fileName);

private:
    WindowInfo windowInfo;

#if USING_WINDOWS == true
    WindowsWrapper wrapper;
#elif USING_GLFW == true
    GLFWwrapper wrapper;
#endif


};