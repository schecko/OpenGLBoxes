
#include "platform.h"
#include "eventlistener.h"
#include "debug.h"

Platform::Platform()
{
    Debug::Assert(0, "Must supply an eventHandler pointer to platform class.");
}

Platform::Platform(EventListener* events) :
#if USING_WINDOWS
    wrapper(WindowsWrapper(events))
#elif USING_GLFW
    wrapper(GLFWwrapper(events))
#endif
{
}

Platform::Platform(const Platform & other)
{
    Debug::Assert(0, "Must supply an eventHandler pointer to platform class.");
}

Platform::~Platform()
{
}

void Platform::Init(ui32 width, ui32 height, const char * name)
{
    wrapper.Init(width, height, name);
}

void Platform::vSync(int frameRate)
{
}

void Platform::PollEvents(EventListener* events)
{
    wrapper.PollEvents(events);
}

void Platform::SwapBuffers()
{
    wrapper.WrapSwapBuffers();
}


WindowInfo* Platform::GetWindowInfo()
{
    return &windowInfo;
}

void Platform::UpdateWindowInfo(WindowEvents windowEvents)
{
    windowInfo.width = windowEvents.newClientWidth;
    windowInfo.height = windowEvents.newClientHeight;
    windowInfo.windowState = windowEvents.windowState;

}

File Platform::ReadEntireFile(char * fileName)
{
    return wrapper.ReadEntireFile(fileName);
}
