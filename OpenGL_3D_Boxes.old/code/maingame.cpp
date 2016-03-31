
#include "maingame.h"

#define GLEW_STATIC
#include <GLEW\glew.h>
#include <GLM\glm.hpp>
#include <GLM\gtc\matrix_transform.hpp>
#include <GLM\gtc\type_ptr.hpp>
#include <string>
#include <iostream>

#include "debug.h"
#include "shaders.h"
#include "eventlistener.h"

MainGame::MainGame()
{
}

MainGame::~MainGame()
{
}

int MainGame::Init()
{
    cEO.masterClock.Tick();
    if (!cEO.platform.Init(&cEO.eventListener,
                           1300,
                           800,
                           "Game"))
    {
        return Debug::PLATFORM_FAILED;
    }
    int time = (int)cEO.masterClock.Tock();
    std::cout << "time to init window: " +  time << std::endl;

    cEO.masterClock.Tick();
    if (!cEO.graphicsController.Init(cEO.platform.GetWindowInfo()))
    {
        return Debug::GRAPHICS_FAILED;
    }
    std::cout << "time to init graphicscontroller: " + (int)cEO.masterClock.Tock() << std::endl;
    return 0;

}

int MainGame::Run()
{
    //main game loop
    while (!cEO.eventListener.GetCloseGame())
    {
        cEO.masterClock.UpdateFrameTimes();
        cEO.platform.PollEvents(&cEO.eventListener);
        //react to events of last frame
        EventAction();
        //render
        cEO.graphicsController.Render(cEO.masterClock.GetTimeSinceStart());


        //swap buffers
        cEO.platform.SwapBuffers();


        //store events from message que to event struct
        cEO.eventListener.SwapEvents();
    }

    return 0;
}

int MainGame::Quit()
{
    cEO.graphicsController.Quit();
    cEO.platform.Quit();

    return 0;
}

void MainGame::EventAction()
{
    if (cEO.eventListener.GetDidEventsHappenThisFrame())
    {
        if (cEO.eventListener.GetKey(KEY_W))
        {
            cEO.graphicsController.graphicsObjects.camera.MoveCameraPosition(Camera::FORWARD,
                    (GLfloat)cEO.masterClock.GetDeltaFrameTime());
        }
        if (cEO.eventListener.GetKey(KEY_A))
        {
            cEO.graphicsController.graphicsObjects.camera.MoveCameraPosition(Camera::LEFT,
                    (GLfloat)cEO.masterClock.GetDeltaFrameTime());
        }
        if (cEO.eventListener.GetKey(KEY_S))
        {
            cEO.graphicsController.graphicsObjects.camera.MoveCameraPosition(Camera::BACKWARD,
                    (GLfloat)cEO.masterClock.GetDeltaFrameTime());
        }
        if (cEO.eventListener.GetKey(KEY_D))
        {
            cEO.graphicsController.graphicsObjects.camera.MoveCameraPosition(Camera::RIGHT,
                    (GLfloat)cEO.masterClock.GetDeltaFrameTime());
        }

        if (cEO.eventListener.GetViewportNeedsUpdating())
        {
            cEO.platform.GetWindowInfo()->width = cEO.eventListener.GetWindowEvents()->newScreenWidth;
            cEO.platform.GetWindowInfo()->height = cEO.eventListener.GetWindowEvents()->newScreenHeight;
            cEO.graphicsController.UpdateViewport();
        }
        if (cEO.eventListener.GetIsMouseInWindow())
        {


            //view angle calculation
            glm::vec2 mousePos = cEO.eventListener.GetMousePos();
            glm::vec2 lastMousePos = cEO.eventListener.GetLastMousePos();

            if (cEO.eventListener.GetMouseEnteredScreen())
            {
                lastMousePos = mousePos;
            }

            float yViewOffset = lastMousePos.y - mousePos.y;
            float xViewOffset = mousePos.x - lastMousePos.x;

            yViewOffset *= cEO.masterClock.GetDeltaFrameTime();
            xViewOffset *= cEO.masterClock.GetDeltaFrameTime();

            cEO.graphicsController.graphicsObjects.camera.ChangeViewAngle(xViewOffset, yViewOffset);

            //zoom calculation
            int32 scroll = cEO.eventListener.GetMouseScroll();
            cEO.graphicsController.graphicsObjects.camera.ChangeZoom(scroll);
        }

    }
}





