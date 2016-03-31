#pragma once

#include <GLM\glm.hpp>
#include <GLM\gtc\matrix_transform.hpp>
#include <GLM\gtc\type_ptr.hpp>

#include "typedefs.h"

enum KeyCode
{
    //DO NOT CHANGE
    KEY_FIRST_ENUM,


    KEY_W,
    KEY_A,
    KEY_S,
    KEY_D,


    //DO NOT CHANGE
    KEY_LAST_ENUM
};

enum WindowState
{
    NORMAL = 0,
    CLOSE_WINDOW,
    MINIMZED,
    MAXIMIZED,


};


struct MouseEvents
{
    glm::vec2 pos;
    glm::vec2 lastPos;
    union
    {
        struct
        {
            bool mouseControl;
            bool mouseLeft;
            bool mouseMiddle;
            bool mouseRight;
            bool mouseShift;
            bool mouseX1;
            bool mouseX2;
        };
        bool mouseButtons[7];
    };

    bool isMouseInWindow; // == true if app
    //is active and the cursor is hovering over the window itself
    bool mouseEnteredWindow; //is only true on the event of the mouse passing over
    //the boundary between another app/os and this app's window.
    bool scrollMoved;
    i32 scroll; //positive value indicates the wheel scrolled away from user.

};

struct WindowEvents
{
    bool viewPortNeedsUpdating;
    WindowState windowState;
    bool closeGame;
    ui32 newClientWidth;
    ui32 newClientHeight;

};


struct Events
{
    bool keys[KEY_LAST_ENUM + 1];
    MouseEvents mouseEvents;
    WindowEvents windowEvents;
};

class EventListener
{
public:
    EventListener();
    ~EventListener();

    void SetKey(KeyCode key, bool value);
    bool GetKey(KeyCode key);


    void SetMousePos(glm::vec2 pos);
    glm::vec2 GetMousePos();

    glm::vec2 GetLastMousePos();

    i32 GetMouseScroll();

    void SetViewportNeedsUpdating(bool value, ui32 width, ui32 height);
    bool GetViewportNeedsUpdating();

    void SetIsMouseInWindow(bool value);
    bool GetIsMouseInWindow();

    void SetMouseEnteredWindow(bool value);
    bool GetMouseEnteredWindow();

    void SetCloseGame();
    bool GetCloseGame();

    WindowEvents* GetWindowEvents();
    MouseEvents* GetMouseEvents();

    void SwapEvents();
    //Events* GetEvents(uint32 type);


private:
    //the currentEvent is the index of the event being written
    //the lastevent is the index of the eventstruct being read(reading happens
    //the frame after the frame of writing)
    Events events;
    //ubyte currentEvent = 1;


};