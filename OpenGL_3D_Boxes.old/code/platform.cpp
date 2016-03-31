
#include "platform.h"
#include "eventlistener.h"

Platform::Platform()
{
}

Platform::Platform(const Platform & other)
{
}

Platform::~Platform()
{
}

bool Platform::Init(EventListener* events,
                    uint32 width,
                    uint32 height,
                    const char* name)
{
    windowInfo.width = width;
    windowInfo.height = height;
    return wrapper.Init(events, width, height, name);
}

void Platform::Quit()
{
    wrapper.Quit();
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
