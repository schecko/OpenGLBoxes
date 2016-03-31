#pragma once




#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <string>

#include "eventlistener.h"
#include "typedefs.h"

struct WinInfo
{
    HWND windowHandle;
    HINSTANCE exeHandle;
    HGLRC renderingContext;
    EventListener* events;
    TRACKMOUSEEVENT trackMouse;
    char* fullExePath;
    ui32 exePathSize;
    ui32 exeFolderPathSize;
};

struct File
{
    ui32 size;
    void* contents;
};

class WindowsWrapper
{
public:
    WindowsWrapper();
    WindowsWrapper(const WindowsWrapper& other);
    ~WindowsWrapper();

    WindowsWrapper(EventListener* events);
    void Init(ui32 width, ui32 height, const char* name);
    void vSync(int frameRate);


    void PollEvents(EventListener* events);
    void WrapSwapBuffers();

    static LRESULT CALLBACK EventHandler(HWND hwnd,
                                         UINT msg,
                                         WPARAM wP,
                                         LPARAM lP);

    File ReadEntireFile(char* fileName);


private:
    void FindExeLocation();
    bool CreateRenderingContext(HWND* windowHandle, HGLRC* renderingContext);
    WinInfo winInfo;
};


