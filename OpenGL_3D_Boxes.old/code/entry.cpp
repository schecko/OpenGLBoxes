#include "maingame.h"





int main()
{

    MainGame mainGame;
    int errorCode = 0;
    errorCode = mainGame.Init();
    if (errorCode)
    {
        return errorCode;
    }
    errorCode = mainGame.Run();
    if (errorCode)
    {
        return errorCode;
    }
    errorCode = mainGame.Quit();
    if (errorCode)
    {
        return errorCode;
    }
    return 0;

}

