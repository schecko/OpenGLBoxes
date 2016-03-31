#include "eventlistener.h"

EventListener::EventListener() : events{Events()}
{

}

EventListener::~EventListener()
{
}

void EventListener::SetKey(KeyCode key, bool value)
{
    events[currentEvent].keys[key] = value;
}
bool EventListener::GetKey(KeyCode key)
{
    return events[currentEvent].keys[key];
}
void EventListener::SetMousePos(glm::vec2 pos)
{
    events[currentEvent].mouse.pos = pos;
}

glm::vec2 EventListener::GetMousePos()
{
    return events[currentEvent].mouse.pos;
}

glm::vec2 EventListener::GetLastMousePos()
{
    return events[currentEvent].mouse.lastPos;
}

int32 EventListener::GetMouseScroll()
{
    return events[currentEvent].mouse.scroll;
}

void EventListener::SetViewportNeedsUpdating(bool value, uint32 width, uint32 height)
{
    events[currentEvent].windowEvents.viewPortNeedsUpdating = true;
    events[currentEvent].windowEvents.newScreenHeight = height;
    events[currentEvent].windowEvents.newScreenWidth = width;
}

bool EventListener::GetViewportNeedsUpdating()
{
    return events[currentEvent].windowEvents.viewPortNeedsUpdating;
}

void EventListener::SetIsMouseInWindow(bool value)
{
    events[currentEvent].mouse.isMouseInWindow = value;
}

bool EventListener::GetIsMouseInWindow()
{
    return events[currentEvent].mouse.isMouseInWindow;
}

void EventListener::SetMouseEnteredScreen(bool value)
{
    events[currentEvent].mouse.mouseEnteredScreen = value;
}

bool EventListener::GetMouseEnteredScreen()
{
    return events[currentEvent].mouse.mouseEnteredScreen;
}

void EventListener::SetDidEventsHappenThisFrame(bool value)
{
    events[currentEvent].windowEvents.didEventsHappenThisFrame = value;
}

bool EventListener::GetDidEventsHappenThisFrame()
{
    return events[currentEvent].windowEvents.didEventsHappenThisFrame;
}

void EventListener::SwapEvents()
{
    events[currentEvent].mouse.lastPos = events[currentEvent].mouse.pos;
    // events[currentEvent] = events[currentEvent];
    //events[lastEvent] = {};
    //ubyte temp = currentEvent;
    //currentEvent = currentEvent;
    //currentEvent = temp;
}
/*
Events * EventListener::GetEvents(uint32 type)
{

	if (type == THIS_EVENT_FRAME)
	{
		return &events[currentEvent];
	}
	else
	{
		return &events[lastEvent];
	}

}*/

void EventListener::SetCloseGame()
{
    events[currentEvent].windowEvents.closeGame = true;
}

bool EventListener::GetCloseGame()
{
    return events[currentEvent].windowEvents.closeGame;
}

WindowEvents * EventListener::GetWindowEvents()
{
    return &events[currentEvent].windowEvents;
}
