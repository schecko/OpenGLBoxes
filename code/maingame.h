#pragma once

#define GLEW_STATIC
#include <GLEW\glew.h>
#include <string>
#include <GLM\glm.hpp>
#include <GLM\gtc\matrix_transform.hpp>
#include <GLM\gtc\type_ptr.hpp>

#include "graphicscontroller.h"
#include "platform.h"
#include "clock.h"
#include "eventlistener.h"
#include "resourcemanager.h"

struct CoreEngineObjects
{
    GraphicsController* graphicsController;
    Platform* platform;
    EventListener* eventListener;
    Clock* masterClock;
    ResourceManager* resourceManager;

};

const ui32 numBoxes = 10000;

struct Box
{
    TextureInfo texture;
    glm::vec3 position;

};
class MainGame
{


public:
    MainGame();
    ~MainGame();

    void Run();

protected:

private:

    void EventAction();

    CoreEngineObjects cEO;
    Box boxes[numBoxes];

};

