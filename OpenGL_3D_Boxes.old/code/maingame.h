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

struct CoreEngineObjects
{
    GraphicsController graphicsController = GraphicsController();
    Platform platform = Platform();
    EventListener eventListener = EventListener();
    Clock masterClock = Clock();
};

class MainGame
{


public:
    MainGame();
    ~MainGame();

    int Init();
    int Run();
    int Quit();

protected:

private:

    void EventAction();

    CoreEngineObjects cEO;

};

