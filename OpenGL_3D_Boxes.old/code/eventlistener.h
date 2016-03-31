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
    NORMAL,
    CLOSE_WINDOW,
    MINIMZED,
    MAXIMIZED,


};


struct MouseEvents
{
    glm::vec2 pos;
    glm::vec2 lastPos;
    bool isMouseInWindow = false;
    bool mouseEnteredScreen = false;
    int32 scroll; //positive value indicates the wheel scrolled away from user.

};

struct WindowEvents
{
    bool viewPortNeedsUpdating = false;
    WindowState windowState = NORMAL;
    bool didEventsHappenThisFrame = false;
    bool closeGame = false;
    uint32 newScreenWidth;
    uint32 newScreenHeight;

};


struct Events
{
    bool keys[KEY_LAST_ENUM + 1] = {false};
    MouseEvents mouse = {};
    WindowEvents windowEvents = {};
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

    int32 GetMouseScroll();

    void SetViewportNeedsUpdating(bool value, uint32 width, uint32 height);
    bool GetViewportNeedsUpdating();

    void SetIsMouseInWindow(bool value);
    bool GetIsMouseInWindow();

    void SetMouseEnteredScreen(bool value);
    bool GetMouseEnteredScreen();

    void SetDidEventsHappenThisFrame(bool value);
    bool GetDidEventsHappenThisFrame();

    void SetCloseGame();
    bool GetCloseGame();

    WindowEvents* GetWindowEvents();

    void SwapEvents();
    //Events* GetEvents(uint32 type);


private:
    //the currentEvent is the index of the event being written
    //the lastevent is the index of the eventstruct being read(reading happens
    //the frame after the frame of writing)
    Events events[2];
    ubyte currentEvent = 0;
    //ubyte currentEvent = 1;


};