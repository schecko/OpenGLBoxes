#pragma once

#include "camera.h"

#define GLEW_STATIC
#include <GLEW\glew.h>
#include <GLFW\glfw3.h>

#include "eventlistener.h"


class GLFWwrapper
{
public:

    GLFWwrapper();
    ~GLFWwrapper();

    bool Init(EventListener* events,
              int width,
              int height,
              const char* name);
    void Quit();

    void vSync(int frameRate);

    void PollEvents(EventListener* events);
    void WrapSwapBuffers();

    static void KeyHandler(GLFWwindow* platform, int key, int scancode, int action, int mode);
    static void ScrollHandler(GLFWwindow* platform, double xOffset, double yOffset);
    static void MouseHandler(GLFWwindow* platform, double xPos, double yPos);


private:
    EventListener* events;
    GLFWwindow* GLFWhandle;
};





