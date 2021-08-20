#include <Windows.h>
#include <iostream>
#include <chrono>
#include <thread>
#include "FryEngine/Game.h"
#include "FryEngine/ECS/ECS.h"
#include "FryEngine/Window/Window.h"

class MyGame : public FryEngine::Game
{
    public:
        MyGame(): m_win(L"FryTest") {};
        void OnInit()
        {
            m_win.Init();
        }

        void OnUpdate(TimeDuration DT_ms)
        {
            if(GetAsyncKeyState(VK_ESCAPE) || !m_win.ProcessMessage())
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
    private:
    TimeDuration m_currentSecond = 0;
    TimeDuration m_gameRuntime = 0;
    Window m_win;
};


int main()
{
    MyGame game;
    game.Run();
}