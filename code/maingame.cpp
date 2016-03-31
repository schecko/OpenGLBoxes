
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
#include "graphicscontroller.h"
#include "typedefs.h"





MainGame::MainGame()
{


    cEO.masterClock = new Clock();
    cEO.eventListener =  new EventListener();
    cEO.masterClock->Tick();
    cEO.platform = new Platform(cEO.eventListener);
    cEO.platform->Init(1300, 800, "Game");
    cEO.resourceManager = new ResourceManager(cEO.platform);
    std::cout << "time to init window: " + std::to_string(cEO.masterClock->Tock()) << std::endl;
    cEO.masterClock->Tick();
    cEO.graphicsController = new GraphicsController(*cEO.platform->GetWindowInfo());
    std::cout << "time to init graphicscontroller: " + std::to_string(cEO.masterClock->Tock()) << std::endl;


    i32 range = numBoxes / 10;
    for (int i = 0; i < numBoxes; i++)
    {
        if (i % 1)
        {
            boxes[i].texture = cEO.resourceManager->GetTexture("data\\assets\\textures\\awesomeface.bmp");
        }
        else
        {
            boxes[i].texture = cEO.resourceManager->GetTexture("data\\assets\\textures\\container.bmp");
        }
        boxes[i].position = glm::vec3((float)((rand() % range) - (range / 2)),
                                      (float)((rand() % range) - (range / 2)),
                                      (float)((rand() % range) - (range / 2)));

    }
    for (int i = 0; i < 2; i++)
    {
        cEO.graphicsController->BindTexture(&boxes[i].texture);
    }




}


void MainGame::Run()
{
    //main game loop
    while (!cEO.eventListener->GetCloseGame())
    {
        cEO.masterClock->Tick();
        cEO.masterClock->UpdateFrameTimes();
        cEO.platform->PollEvents(cEO.eventListener);
        //react to events of last frame
        EventAction();
        //render
        cEO.graphicsController->Render(cEO.masterClock->GetTimeSinceStart(),
                                       boxes);


        //swap buffers
        cEO.platform->SwapBuffers();


        //store events from message que to event struct
        cEO.eventListener->SwapEvents();
        Debug::Message(std::to_string(cEO.masterClock->Tock()));
    }

}

MainGame::~MainGame()
{
    delete(cEO.masterClock);
    delete(cEO.graphicsController);
    delete(cEO.platform);
    delete(cEO.eventListener);
}

void MainGame::EventAction()
{

    if (cEO.eventListener->GetKey(KEY_W))
    {
        cEO.graphicsController->graphicsObjects.camera->MoveCameraPosition(Camera::FORWARD,
                (GLfloat)cEO.masterClock->GetDeltaFrameTime());
    }
    if (cEO.eventListener->GetKey(KEY_A))
    {
        cEO.graphicsController->graphicsObjects.camera->MoveCameraPosition(Camera::LEFT,
                (GLfloat)cEO.masterClock->GetDeltaFrameTime());
    }
    if (cEO.eventListener->GetKey(KEY_S))
    {
        cEO.graphicsController->graphicsObjects.camera->MoveCameraPosition(Camera::BACKWARD,
                (GLfloat)cEO.masterClock->GetDeltaFrameTime());
    }
    if (cEO.eventListener->GetKey(KEY_D))
    {
        cEO.graphicsController->graphicsObjects.camera->MoveCameraPosition(Camera::RIGHT,
                (GLfloat)cEO.masterClock->GetDeltaFrameTime());
    }

    if (cEO.eventListener->GetViewportNeedsUpdating())
    {
        WindowEvents* windowEvents = cEO.eventListener->GetWindowEvents();
        cEO.platform->UpdateWindowInfo(*windowEvents);
        cEO.graphicsController->UpdateViewport(windowEvents->newClientWidth, windowEvents->newClientHeight);
    }

    MouseEvents* mouseEvents = cEO.eventListener->GetMouseEvents();
    if (cEO.eventListener->GetIsMouseInWindow())
    {

        if (mouseEvents->scrollMoved)
        {
            //zoom calculation
            GLfloat scroll = (GLfloat)cEO.eventListener->GetMouseScroll();
            cEO.graphicsController->graphicsObjects.camera->ChangeZoom(scroll);
            mouseEvents->scrollMoved = false;
        }

        if (mouseEvents->mouseLeft)
        {
            //view angle calculation
            glm::vec2 mousePos = cEO.eventListener->GetMousePos();
            glm::vec2 lastMousePos = cEO.eventListener->GetLastMousePos();

            if (cEO.eventListener->GetMouseEnteredWindow())
            {
                lastMousePos = mousePos;
            }

            double yViewOffset = (lastMousePos.y - mousePos.y);
            double xViewOffset = (mousePos.x - lastMousePos.x);

            //Debug::Message(std::to_string(xViewOffset));

            yViewOffset *= cEO.masterClock->GetDeltaFrameTime();
            xViewOffset *= cEO.masterClock->GetDeltaFrameTime();

            cEO.graphicsController->graphicsObjects.camera->ChangeViewAngle((GLfloat)xViewOffset, (GLfloat)yViewOffset);
        }


    }


}





