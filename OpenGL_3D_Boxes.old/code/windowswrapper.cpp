
#include "windowswrapper.h"



#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <gl/gl.h>

#include "debug.h"
#include "eventlistener.h"
#include "clock.h"


WindowsWrapper::WindowsWrapper() : windowHandle(nullptr),
    events(nullptr)
{
}

WindowsWrapper::WindowsWrapper(const WindowsWrapper & other)
{
}

WindowsWrapper::~WindowsWrapper()
{
}

bool WindowsWrapper::Init(EventListener* events, uint32 width, uint32 height, const char * name)
{

    //create window
    WNDCLASS windowDescription = {0};
    windowDescription.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
    windowDescription.lpfnWndProc = EventHandler;
    windowDescription.cbWndExtra = sizeof(void*);
    windowDescription.hInstance = GetModuleHandle(nullptr);
    windowDescription.lpszMenuName = (LPCSTR) name;
    windowDescription.lpszClassName = (LPCSTR) name;

    if (!RegisterClass(&windowDescription))
    {
        return false;
    }

    uint32 x, y;
    x = y = CW_USEDEFAULT;

    windowHandle = CreateWindowEx(0,
                                  windowDescription.lpszMenuName,
                                  windowDescription.lpszMenuName,
                                  WS_OVERLAPPEDWINDOW | WS_VISIBLE,
                                  x,
                                  y,
                                  width,
                                  height,
                                  nullptr,
                                  nullptr,
                                  windowDescription.hInstance,
                                  events);

    if (!windowHandle)
    {
        return false;
    }
    ShowWindow(windowHandle, SW_SHOW);

    //create false rendering context for extensions
    CreateRenderingContext(&windowHandle, &renderingContext);


    return true;
}

void WindowsWrapper::Quit()
{
    HDC deviceContext = GetDC(windowHandle);
    wglMakeCurrent(deviceContext, nullptr);
    wglDeleteContext(renderingContext);
}

void WindowsWrapper::vSync(int frameRate)
{
}

void WindowsWrapper::PollEvents(EventListener* events)
{

    //static int messageCount;

    MSG msg;
    events->SetDidEventsHappenThisFrame(PeekMessage(&msg, nullptr, 0, 0, PM_NOREMOVE));
    while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
    {
#
        //Debug::Message(std::to_string(messageCount++));
        switch (msg.message)
        {
            case WM_MOUSEMOVE:
            {
                if (!events->GetMouseEnteredScreen())
                {
                    events->SetMouseEnteredScreen(true);
                }
                else
                {
                    events->SetMouseEnteredScreen(false);
                }
                uint32 x = msg.lParam & 0xFFFF;
                uint32 y = (msg.lParam >> (sizeof(msg.lParam) * 8 / 2)) & 0xFFFF;

                glm::vec2 pos = glm::vec2(x, y);
                events->SetMousePos(pos);
                Debug::Message(std::to_string(pos.x) + " " + std::to_string(pos.y));
            }
            case WM_MOUSELEAVE:
            {
                events->SetIsMouseInWindow(false);

            }
            break;
            //case WM_SETCURSOR:
            //{
            //    //Debug::Message("WM_SETCURSOR");
            //}
            //break;
            case WM_SYSKEYDOWN:
            case WM_SYSKEYUP:
            case WM_KEYDOWN:
            case WM_KEYUP:
            {
                uint32 key = (uint32)msg.wParam;
                bool wasDown = ((msg.lParam) & (1 << 30)) != 1;
                bool isDown = ((msg.lParam) & (1 << 31)) == 1;
                if (wasDown != isDown)
                {
                    switch (key)
                    {
                        case 'W':
                        {
                            events->SetKey(KEY_W, isDown);
                        }
                        break;

                        case 'A':
                        {
                            events->SetKey(KEY_A, isDown);
                        }
                        break;

                        case 'S':
                        {
                            events->SetKey(KEY_S, isDown);
                        }
                        break;

                        case 'D':
                        {
                            events->SetKey(KEY_D, isDown);
                        }
                        break;

                        default:
                        {
                            Debug::SoftAssert(0, "key hit but not used: " + key);
                        }

                    }
                }
            }
            break;
            case WM_SIZE:
            {

            }
            break;
            break;
            default:
            {
                //eventually translate and dispatch message will go in here

            }
        }
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    } //peekmessage
    // messageCount = 0;
}

void WindowsWrapper::WrapSwapBuffers()
{
    HDC deviceContext = GetDC(windowHandle);
    SwapBuffers(deviceContext);
}


LRESULT WindowsWrapper::EventHandler(HWND hwnd, UINT msg, WPARAM wP, LPARAM lP)
{
    EventListener* events = (EventListener*)GetWindowLongPtr(hwnd, 0);
    switch (msg)
    {
        case WM_CREATE:
        {
            CREATESTRUCTW* cs = (CREATESTRUCTW*) lP;
            SetWindowLongPtrW(hwnd, 0, (LONG_PTR)cs->lpCreateParams);
        }
        break;
        case WM_SIZE:
        {
            //TODO FIX MINIMIZING
            events->SetDidEventsHappenThisFrame(true);
            Debug::SoftAssert(0, "size");
            WindowEvents* windowEvents = events->GetWindowEvents();
            windowEvents->viewPortNeedsUpdating = true;
            windowEvents->newScreenWidth = lP & 0xFFFF;
            windowEvents->newScreenHeight = lP >> (sizeof(lP) * 8 / 2) & 0xFFFF;
            switch (wP)
            {
                case SIZE_MAXIMIZED:
                {
                    windowEvents->windowState = MAXIMIZED;
                }
                break;
                case SIZE_MINIMIZED:
                {
                    windowEvents->windowState = MINIMZED;
                }
                break;
                case SIZE_RESTORED:
                {
                    windowEvents->windowState = NORMAL;
                }
                break;
            }
        }
        break;
        case WM_DESTROY:
        case WM_CLOSE:
        {
            events->SetDidEventsHappenThisFrame(true);
            events->SetCloseGame();
        }
        break;
            //never use this flag huge amounts of spam messages come in
            //case WM_SETCURSOR:
            //{
            //    Debug::Message("SETCURSOR THROUGH WNDPROC.....annoying");
            //}
        break;
        case WM_ACTIVATEAPP:
        {
            //if activated wP == true, deactivated means wP == false
            //TODO are there any exceptions to this logic?
            events->SetDidEventsHappenThisFrame(true);
            events->SetIsMouseInWindow((bool)wP);
            //TODO possible reason for glitches here maybe

            Debug::Message("isMouseinWindow: " + events->GetIsMouseInWindow());
            events->SetMouseEnteredScreen(!events->GetMouseEnteredScreen() | !(bool)wP);
        }
        break;

    }
    return DefWindowProc(hwnd, msg, wP, lP);
}

bool WindowsWrapper::CreateRenderingContext(HWND* windowHandle, HGLRC* renderingContext)
{


    HDC deviceContext = GetDC(*windowHandle);

    PIXELFORMATDESCRIPTOR prefferedPixelFormat = {0};

    prefferedPixelFormat.nSize = sizeof(PIXELFORMATDESCRIPTOR);
    prefferedPixelFormat.nVersion = 1;
    prefferedPixelFormat.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    prefferedPixelFormat.iPixelType = PFD_TYPE_RGBA;
    prefferedPixelFormat.cColorBits = 32;
    prefferedPixelFormat.cDepthBits = 24;
    prefferedPixelFormat.cStencilBits = 8;

    uint32 suggestedPixelFormatIndex = ChoosePixelFormat(deviceContext, &prefferedPixelFormat);

    Debug::Assert(suggestedPixelFormatIndex != 0, "could not find a suitable pixelformat");

    PIXELFORMATDESCRIPTOR suggestedPixelFormat;
    DescribePixelFormat(deviceContext,
                        suggestedPixelFormatIndex,
                        sizeof(suggestedPixelFormat),
                        &suggestedPixelFormat);

    SetPixelFormat(deviceContext, suggestedPixelFormatIndex, &suggestedPixelFormat);

    *renderingContext = wglCreateContext(deviceContext);

    bool result = false;
    if (wglMakeCurrent(deviceContext, *renderingContext))
    {
        result = true;
    }


    typedef BOOL WINAPI wglSwapIntervalExt(int interval);
    //static wglSwapIntervalExt *wglSwapInterval;

    typedef HGLRC WINAPI wglCreateContextAttribsArb(HDC hDC, HGLRC hShareContext,
            const int *attribList);



    wglSwapIntervalExt* wglSwapInterval = nullptr;
    wglSwapInterval = (wglSwapIntervalExt*)wglGetProcAddress("wglSwapIntervalEXT");

    wglCreateContextAttribsArb* wglCreateContextAttribs = nullptr;
    wglCreateContextAttribs = (wglCreateContextAttribsArb*)wglGetProcAddress("wglCreateContextAttribsARB");

    Debug::ReleaseAssert(wglCreateContextAttribs != nullptr,
                         "could not find function pointer to wglcreatecontextattribs");

#define WGL_CONTEXT_MAJOR_VERSION_ARB           0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB           0x2092
#define WGL_CONTEXT_LAYER_PLANE_ARB             0x2093
#define WGL_CONTEXT_FLAGS_ARB                   0x2094
#define	WGL_CONTEXT_PROFILE_MASK_ARB            0x9126
#define WGL_CONTEXT_DEBUG_BIT_ARB               0x0001
#define WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB  0x0002

    int32 Attributes[] = { WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
                           WGL_CONTEXT_MINOR_VERSION_ARB, 3,
                           WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_DEBUG_BIT_ARB,
                           WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
                           0

                         };

    HGLRC modernRenderingContext = nullptr;
    modernRenderingContext = wglCreateContextAttribs(deviceContext, nullptr, Attributes);

    Debug::ReleaseAssert(modernRenderingContext != nullptr, "could not create a modern context");

    BOOL isCurrent =  wglMakeCurrent(deviceContext, modernRenderingContext);

    Debug::ReleaseAssert(isCurrent != 0, "could not make modern rendering context current");
    wglDeleteContext(*renderingContext);
    *renderingContext = modernRenderingContext;

    ReleaseDC(*windowHandle, deviceContext);
    return result;

}
