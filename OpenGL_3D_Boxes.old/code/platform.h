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
    int width = 800;
    int	height = 800;
    bool fullscreen = false;


};



class Platform
{
public:
    Platform();
    Platform(const Platform& other);
    ~Platform();

    bool Init(EventListener* events,
              uint32 width = 800,
              uint32 height = 800,
              const char* name = "Engine");

    void Quit();

    void vSync(int frameRate);


    void PollEvents(EventListener* events);
    void SwapBuffers();

    WindowInfo* GetWindowInfo();

private:
    WindowInfo windowInfo = WindowInfo();

#if USING_WINDOWS == true
    WindowsWrapper wrapper = WindowsWrapper();
#elif USING_GLFW == true
    GLFWwrapper wrapper = GLFWwrapper();
#endif


};