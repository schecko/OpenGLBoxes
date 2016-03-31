
#include "windowswrapper.h"



#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <windowsx.h>
#include <gl/gl.h>
#include <algorithm>
#include <vector>

#include "debug.h"
#include "eventlistener.h"
#include "clock.h"
#include "utilities.h"



WindowsWrapper::WindowsWrapper()
{
    Debug::Assert(0, "Must pass in an eventlistener struct to WindowsWrapper");
}

WindowsWrapper::WindowsWrapper(const WindowsWrapper & other)
{
}

WindowsWrapper::WindowsWrapper(EventListener* events) : winInfo(WinInfo())
{
    winInfo.events = events;

}

WindowsWrapper::~WindowsWrapper()
{
    HDC deviceContext = GetDC(winInfo.windowHandle);
    wglMakeCurrent(deviceContext, nullptr);
    wglDeleteContext(winInfo.renderingContext);
    DestroyWindow(winInfo.windowHandle);
    delete(winInfo.fullExePath);
}


void WindowsWrapper::Init(ui32 width, ui32 height, const char * name)
{

    winInfo.exeHandle = GetModuleHandle(nullptr);

    //create window
    WNDCLASS windowDescription = { 0 };
    windowDescription.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
    windowDescription.lpfnWndProc = EventHandler;
    windowDescription.cbWndExtra = sizeof(void*);
    windowDescription.hInstance = winInfo.exeHandle;
    windowDescription.lpszMenuName = (LPCSTR)name;
    windowDescription.lpszClassName = (LPCSTR)name;

    ATOM error = RegisterClass(&windowDescription);
    Debug::Assert(error != false, "Failed to register windowclass.");

    RECT clientRect = {0, 0, (LONG)width, (LONG)height};
    AdjustWindowRect(&clientRect, WS_OVERLAPPEDWINDOW | WS_VISIBLE, false);

    ui32 windowWidth = abs(clientRect.right - clientRect.left);
    ui32 windowHeight = abs(clientRect.top - clientRect.bottom);

    winInfo.windowHandle = CreateWindowEx(0,
                                          windowDescription.lpszMenuName,
                                          windowDescription.lpszMenuName,
                                          WS_OVERLAPPEDWINDOW | WS_VISIBLE,
                                          CW_USEDEFAULT,
                                          CW_USEDEFAULT,
                                          windowWidth,
                                          windowHeight,
                                          nullptr,
                                          nullptr,
                                          winInfo.exeHandle,
                                          this);

    Debug::Assert(winInfo.windowHandle != nullptr, "Could not create window.");

    ShowWindow(winInfo.windowHandle, SW_SHOW);

    //create false rendering context for extensions
    CreateRenderingContext(&winInfo.windowHandle, &winInfo.renderingContext);

    //fill in mousetracking info struct
    winInfo.trackMouse.cbSize = sizeof(TRACKMOUSEEVENT);
    winInfo.trackMouse.dwFlags = TME_LEAVE | TME_HOVER;
    winInfo.trackMouse.hwndTrack = winInfo.windowHandle;
    winInfo.trackMouse.dwHoverTime = HOVER_DEFAULT;

    FindExeLocation();

}

void WindowsWrapper::vSync(int frameRate)
{
}

void WindowsWrapper::PollEvents(EventListener* events)
{
    MSG msg;
    LARGE_INTEGER freq;

    QueryPerformanceFrequency(&freq);

    Debug::Message(std::to_string(freq.QuadPart));
    while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    } //peekmessage
    // messageCount = 0;
}

void WindowsWrapper::WrapSwapBuffers()
{
    HDC deviceContext = GetDC(winInfo.windowHandle);
    SwapBuffers(deviceContext);
}


LRESULT WindowsWrapper::EventHandler(HWND hwnd, UINT msg, WPARAM wP, LPARAM lP)
{
    WindowsWrapper* wrapper = nullptr;
    wrapper = (WindowsWrapper*)GetWindowLongPtr(hwnd, 0);

    switch (msg)
    {
        case WM_MOUSEMOVE:
        {
            if (wrapper != nullptr)
            {
                MouseEvents* mouseEvents = wrapper->winInfo.events->GetMouseEvents();
                if (mouseEvents->isMouseInWindow == false)
                {
                    mouseEvents->mouseEnteredWindow = true;
                }
                else
                {
                    mouseEvents->mouseEnteredWindow = false;
                }

                BOOL tracked = TrackMouseEvent(&wrapper->winInfo.trackMouse);
                if (!tracked)
                {
                    memset(mouseEvents->mouseButtons, 0, sizeof(mouseEvents->mouseButtons));
                }

                mouseEvents->isMouseInWindow = (bool)tracked;
                i32 x = GET_X_LPARAM(lP);
                i32 y = GET_Y_LPARAM(lP);

                glm::vec2 pos = glm::vec2((float)x, (float)y);
                mouseEvents->pos = pos;
            }

            //Debug::Message(std::to_string(pos.x) + " " + std::to_string(pos.y));
        }
        break;
        case WM_NCMOUSELEAVE:
        case WM_MOUSELEAVE:
        {
            if (wrapper != nullptr)
            {
                MouseEvents* mouseEvents = wrapper->winInfo.events->GetMouseEvents();
                mouseEvents->isMouseInWindow = false;
                memset(mouseEvents->mouseButtons, 0, sizeof(mouseEvents->mouseButtons));
            }

        }
        break;
        //case WM_SETCURSOR:
        //{
        //    //Debug::Message("WM_SETCURSOR");
        //}
        //break;
        case WM_MOUSEWHEEL:
        {
            if (wrapper != nullptr)
            {
                MouseEvents* mouseEvents = wrapper->winInfo.events->GetMouseEvents();
                i32 offset = GET_Y_LPARAM(wP);
                mouseEvents->scrollMoved = true;
                mouseEvents->scroll = offset / WHEEL_DELTA;
            }
        }
        break;
        case WM_LBUTTONDOWN:
        {
            if (wrapper != nullptr)
            {


                MouseEvents* mouseEvents = wrapper->winInfo.events->GetMouseEvents();
                mouseEvents->mouseLeft = true;
            }

        }
        break;
        case WM_LBUTTONUP:
        {

            if (wrapper != nullptr)
            {
                MouseEvents* mouseEvents = wrapper->winInfo.events->GetMouseEvents();

                mouseEvents->mouseLeft = false;

            }
        }
        break;
        case WM_SYSKEYDOWN:
        case WM_KEYDOWN:
        {
            if (wrapper != nullptr)
            {
                EventListener* events = wrapper->winInfo.events;

                switch (wP)
                {
                    case 'W':
                    case 'w':
                    {
                        events->SetKey(KEY_W, true);
                    }
                    break;

                    case 'A':
                    case 'a':
                    {
                        events->SetKey(KEY_A, true);
                    }
                    break;

                    case 'S':
                    case 's':
                    {
                        events->SetKey(KEY_S, true);
                    }
                    break;

                    case 'D':
                    case 'd':
                    {
                        events->SetKey(KEY_D, true);
                    }
                    break;

                    default:
                    {
                        Debug::SoftAssert(0, "key hit but not used: " + wP);
                    }


                }
            }
        }
        break;
        case WM_KEYUP:
        case WM_SYSKEYUP:
        {
            if (wrapper != nullptr)
            {
                EventListener* events = wrapper->winInfo.events;
                switch (wP)
                {
                    case 'W':
                    case 'w':
                    {
                        events->SetKey(KEY_W, false);
                    }
                    break;
                    case 'A':
                    case 'a':
                    {
                        events->SetKey(KEY_A, false);
                    }
                    break;

                    case 'S':
                    case 's':
                    {
                        events->SetKey(KEY_S, false);
                    }
                    break;
                    case 'D':
                    case 'd':
                    {
                        events->SetKey(KEY_D, false);
                    }
                    break;

                    default:
                    {
                        Debug::SoftAssert(0, "key hit but not used: " + wP);
                    }


                }
            }
        }
        break;
        case WM_CREATE:
        {
            CREATESTRUCTW* cs = (CREATESTRUCTW*) lP;
            SetWindowLongPtrW(hwnd, 0, (LONG_PTR)cs->lpCreateParams);
        }
        break;
        case WM_SIZE:
        {
            //TODO FIX MINIMIZING
            if (wrapper != nullptr)
            {
                WindowEvents* windowEvents = wrapper->winInfo.events->GetWindowEvents();

                windowEvents->viewPortNeedsUpdating = true;
                windowEvents->newClientWidth = GET_X_LPARAM(lP);
                windowEvents->newClientHeight = GET_Y_LPARAM(lP);
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
        }
        break;
        case WM_DESTROY:
        case WM_CLOSE:
        {
            if (wrapper != nullptr)
            {
                EventListener* events = wrapper->winInfo.events;
                events->SetCloseGame();
            }
        }
        break;
        //never use this flag huge amounts of spam messages come in
        //case WM_SETCURSOR:
        //{
        //    Debug::Message("SETCURSOR THROUGH WNDPROC.....annoying");
        //}
        //break;
        case WM_ACTIVATEAPP:
        {
            if (wrapper != nullptr)
            {
                EventListener* events = wrapper->winInfo.events;
                //if activated wP == true, deactivated means wP == false
                //TODO are there any exceptions to this logic?
                //events->SetIsMouseInWindow((bool)wP);
                //TODO possible reason for glitches here maybe

                //Debug::Message("Activate App Message");
                //events->SetMouseEnteredWindow(!events->GetMouseEnteredWindow() | !(bool)wP);
            }
        }
        break;
        default:
        {
            return DefWindowProc(hwnd, msg, wP, lP);
        }
        break;

    }
    return 0;
}

File WindowsWrapper::ReadEntireFile(char * fileName)
{


    File result = {};
    ui32 fileNameSize = CharStarSize(fileName);
    ui32 fullPathSize = winInfo.exeFolderPathSize + fileNameSize;
    char* fullPath = new char[fullPathSize];
    CatChars(winInfo.fullExePath, winInfo.exeFolderPathSize,
             fileName, fileNameSize,
             fullPath);

    HANDLE fileHandle = CreateFile(fullPath,
                                   GENERIC_READ,
                                   FILE_SHARE_READ,
                                   nullptr,
                                   OPEN_EXISTING,
                                   0,
                                   0);

    Debug::Assert(fileHandle != INVALID_HANDLE_VALUE,
                  "could not open file " + std::string(fullPath, fullPathSize));
    if (fileHandle != INVALID_HANDLE_VALUE)
    {
        LARGE_INTEGER fileSize;
        if (GetFileSizeEx(fileHandle, &fileSize))
        {
            Debug::Assert(fileSize.QuadPart < 0xFFFFFFFF,
                          "file size is too big for ui32" + std::string(fullPath,
                                  fullPathSize));

            ui32 fileSize32 = (ui32)fileSize.QuadPart;
            result.contents = HeapAlloc(GetProcessHeap(),
                                        HEAP_ZERO_MEMORY,
                                        fileSize32);
            if (result.contents)
            {
                DWORD bytesRead = 0;
                if (ReadFile(fileHandle,
                             result.contents,
                             fileSize32,
                             &bytesRead,
                             0) && fileSize32 == bytesRead)
                {
                    result.size = fileSize32;
                }
                else
                {
                    VirtualFree(result.contents, 0, MEM_RELEASE);
                    Debug::Assert(0, "file bytes read not equal to file bytes" + std::string(fullPath,
                                  fullPathSize));
                    result.contents = nullptr;
                }
            }

        }

        CloseHandle(fileHandle);
    }
    return result;
}


void WindowsWrapper::FindExeLocation()
{
    DWORD guessExePathSize = MAX_PATH;
    winInfo.fullExePath = new char[guessExePathSize];

    DWORD trueSizeOfFileName = GetModuleFileNameA(nullptr,
                               (LPSTR)winInfo.fullExePath,
                               guessExePathSize);
    Debug::Assert(trueSizeOfFileName != 0, "failed to determine exe file path");

    //if guess and truesizeoffile name are equal, then that means the filename was truncated
    //might be overkill....
    while (guessExePathSize == trueSizeOfFileName)
    {
        delete(winInfo.fullExePath);
        guessExePathSize *= 2;
        winInfo.fullExePath = new char[guessExePathSize];
        trueSizeOfFileName = GetModuleFileName(nullptr,
                                               (LPSTR)winInfo.fullExePath,
                                               sizeof(winInfo.fullExePath));
    }

    char* lastSlash = winInfo.fullExePath + trueSizeOfFileName;
    DWORD sizeOfFolderPath = trueSizeOfFileName;

    while (*lastSlash != '\\')
    {
        lastSlash--;
        sizeOfFolderPath--;
    }

    this->winInfo.exePathSize = trueSizeOfFileName;
    this->winInfo.exeFolderPathSize = sizeOfFolderPath + 1;


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

    ui32 suggestedPixelFormatIndex = ChoosePixelFormat(deviceContext, &prefferedPixelFormat);

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

    i32 Attributes[] = { WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
                         WGL_CONTEXT_MINOR_VERSION_ARB, 3,
                         WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_DEBUG_BIT_ARB,
                         WGL_CONTEXT_PROFILE_MASK_ARB,
                         WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
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
