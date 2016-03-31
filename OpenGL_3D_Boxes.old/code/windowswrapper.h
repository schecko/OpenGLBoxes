#pragma once




#define WIN32_LEAN_AND_MEAN
#include <windows.h>


#include "eventlistener.h"
#include "typedefs.h"

class WindowsWrapper
{
public:
    WindowsWrapper();
    WindowsWrapper(const WindowsWrapper& other);
    ~WindowsWrapper();

    bool Init(EventListener* events,
              uint32 width = 800,
              uint32 height = 800,
              const char* name = "Engine");

    void Quit();

    void vSync(int frameRate);


    void PollEvents(EventListener* events);
    void WrapSwapBuffers();

    static LRESULT CALLBACK EventHandler(HWND hwnd,
                                         UINT msg,
                                         WPARAM wP,
                                         LPARAM lP);
private:
    bool CreateRenderingContext(HWND* windowHandle, HGLRC* renderingContext);

    HWND windowHandle;
    HGLRC renderingContext;
    EventListener* events;
};


