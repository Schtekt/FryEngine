#include <Windows.h>
#include <iostream>
#include <chrono>
#include <thread>
#include "FryEngine/Game.h"
#include "FryEngine/ECS/ECS.h"
#include "FryEngine/Window/Window.h"
#include <crtdbg.h>

class MyGame : public FryEngine::Game
{
    public:
        MyGame(): m_win("FryTest") {};
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
            if(DT_ms != 0)
                m_win.SetWindowName("FryTest" + std::to_string(1000/DT_ms));

            if(m_currentSecond > 1000 && DT_ms != 0)
            {
                //std::cout << "Game has run for " << m_gameRuntime / 1000 << " Seconds!" << std::endl;
                m_currentSecond = 0;
            }
            
            // Sleep to appear as working. TO BE REMOVED!
            Sleep(1);

            //for (int y = 0; y < 720; y++)
            //{
            //    for (int x = 0; x < 1080; x++)
            //    {
            //        m_win.SetPixelColor(x, y, red, green, blue);
            //    }
            //}
            m_win.SetColor(red, green, blue);

            m_win.Render();
        };
    private:
    TimeDuration m_currentSecond = 0;
    TimeDuration m_gameRuntime = 0;
    uint8_t blue = 0;
    uint8_t green = 255;
    uint8_t red = 0;
    Window m_win;
};


int main()
{
    _CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
    MyGame game;
    game.Run();
}