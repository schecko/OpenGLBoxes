
#include "glfwwrapper.h"

#define GLEW_STATIC
#include <GLEW\glew.h>
#include <GLFW\glfw3.h>
#include <iostream>

#include "debug.h"
#include "eventlistener.h"

GLFWwrapper::GLFWwrapper()
{

}

GLFWwrapper::~GLFWwrapper()
{

}

bool GLFWwrapper::Init(EventListener* events, int width, int height, const char* name)
{
    if (!glfwInit())
    {
        Debug::Assert(0, "failed to initialize glfw");
        return false;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

    //all game "windows" will be within the main window
    this->GLFWhandle = glfwCreateWindow(width, height, name, nullptr, nullptr);

    //check if window was made
    Debug::Assert(GLFWhandle != nullptr, "failed to create window");

    glfwSetWindowUserPointer(GLFWhandle, events);

    glfwSetKeyCallback(GLFWhandle, KeyHandler);
    glfwSetCursorPosCallback(GLFWhandle, MouseHandler);
    glfwSetScrollCallback(GLFWhandle, ScrollHandler);

    glfwMakeContextCurrent(GLFWhandle);

    return true;
}

void GLFWwrapper::Quit()
{
    glfwTerminate();
}


void GLFWwrapper::vSync(int frameRate)
{
    glfwSwapInterval(frameRate);
}


void GLFWwrapper::PollEvents(EventListener* events)
{
    this->events = events;
    glfwPollEvents();
}


void GLFWwrapper::WrapSwapBuffers()
{
    glfwSwapBuffers(GLFWhandle);
}


void GLFWwrapper::KeyHandler(GLFWwindow* platform, int key, int scancode, int action, int mode)
{
    EventListener* events = (EventListener*) glfwGetWindowUserPointer(platform);


    switch (action)
    {
        case GLFW_PRESS:
        {
            if (key == GLFW_KEY_W)
            {
                events->SetKey(KEY_W, true);
            }
            if (key == GLFW_KEY_A)
            {
                events->SetKey(KEY_A, true);
            }
            if (key == GLFW_KEY_S)
            {
                events->SetKey(KEY_S, true);
            }
            if (key == GLFW_KEY_D)
            {
                events->SetKey(KEY_D, true);
            }

            if (key == GLFW_KEY_ESCAPE)
            {
                events->SetCloseGame();
            }
        }
        break;
        case GLFW_RELEASE:
        {
            if (key == GLFW_KEY_W)
            {
                events->SetKey(KEY_W, false);
            }
            if (key == GLFW_KEY_A)
            {
                events->SetKey(KEY_A, false);
            }
            if (key == GLFW_KEY_S)
            {
                events->SetKey(KEY_S, false);
            }
            if (key == GLFW_KEY_D)
            {
                events->SetKey(KEY_D, false);
            }

            if (key == GLFW_KEY_ESCAPE)
            {
                events->SetCloseGame();
            }
        }
        break;

    }
}

void GLFWwrapper::ScrollHandler(GLFWwindow * platform, double xOffset, double yOffset)
{
    EventListener* events = (EventListener*)glfwGetWindowUserPointer(platform);

    //windowPtr->cameraPtr->ChangeZoom((GLfloat)yOffset);
}

void GLFWwrapper::MouseHandler(GLFWwindow * platform, double xPos, double yPos)
{
    EventListener* events = (EventListener*)glfwGetWindowUserPointer(platform);

    if (!events->GetIsMouseInWindow())
    {
        glm::vec2 pos = glm::vec2(xPos, yPos);
        events->SetMousePos(pos);
        events->SetIsMouseInWindow(true);
    }


    //double xOffset = xPos - wrapper->mouse.lastX;
    //double yOffset = wrapper->mouse.lastY - yPos; //reversed because y coords go from bot to top



    //wrapper->cameraPtr->ChangeViewAngle(xOffset, yOffset);
}


