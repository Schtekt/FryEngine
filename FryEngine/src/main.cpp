#include <Windows.h>
#include <iostream>
#include <chrono>
#include <thread>
#include "FryEngine/Game.h"
#include "FryEngine/ECS/ECS.h"

// TODO fix linking...

class MyGame : public FryEngine::Game
{
    private:

        void OnUpdate(TimeDuration DT_ms)
        {
            if(GetAsyncKeyState(VK_ESCAPE))
            {
                CloseGame();
            }

            m_currentSecond += DT_ms;
            m_gameRuntime += DT_ms;

            if(m_currentSecond > 1000)
            {
                std::cout << "Game has run for " << m_gameRuntime / 1000 << " Seconds!" << std::endl;
                m_currentSecond = 0;
            }
            
            // Sleep to appear as working. TO BE REMOVED!
            Sleep(10);
        };
    TimeDuration m_currentSecond = 0;
    TimeDuration m_gameRuntime = 0;
};


int main()
{
    MyGame game;
    game.Run();
}