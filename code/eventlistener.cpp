#include "eventlistener.h"

EventListener::EventListener() :events(Events())
{
}

EventListener::~EventListener()
{
}

void EventListener::SetKey(KeyCode key, bool value)
{
    events.keys[key] = value;
}
bool EventListener::GetKey(KeyCode key)
{
    return events.keys[key];
}
void EventListener::SetMousePos(glm::vec2 pos)
{
    events.mouseEvents.pos = pos;
}

glm::vec2 EventListener::GetMousePos()
{
    return events.mouseEvents.pos;
}

glm::vec2 EventListener::GetLastMousePos()
{
    return events.mouseEvents.lastPos;
}

i32 EventListener::GetMouseScroll()
{
    return events.mouseEvents.scroll;
}

void EventListener::SetViewportNeedsUpdating(bool value, ui32 width, ui32 height)
{
    events.windowEvents.viewPortNeedsUpdating = true;
    events.windowEvents.newClientHeight = height;
    events.windowEvents.newClientWidth = width;
}

bool EventListener::GetViewportNeedsUpdating()
{
    return events.windowEvents.viewPortNeedsUpdating;
}

void EventListener::SetIsMouseInWindow(bool value)
{
    events.mouseEvents.isMouseInWindow = value;
}

bool EventListener::GetIsMouseInWindow()
{
    return events.mouseEvents.isMouseInWindow;
}

void EventListener::SetMouseEnteredWindow(bool value)
{
    events.mouseEvents.mouseEnteredWindow = value;
}

bool EventListener::GetMouseEnteredWindow()
{
    return events.mouseEvents.mouseEnteredWindow;
}


void EventListener::SwapEvents()
{
    events.mouseEvents.lastPos = events.mouseEvents.pos;
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
    events.windowEvents.closeGame = true;
}

bool EventListener::GetCloseGame()
{
    return events.windowEvents.closeGame;
}

WindowEvents * EventListener::GetWindowEvents()
{
    return &events.windowEvents;
}

MouseEvents * EventListener::GetMouseEvents()
{
    return &events.mouseEvents;
}
