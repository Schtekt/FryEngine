#include <Windows.h>
#include <iostream>
#include <chrono>
#include <thread>
#include <crtdbg.h>
#include "FryEngine/Game.h"
#include "FryEngine/ECS/ECS.h"
#include "FryEngine/Rendering/Window.h"

class MyGame : public FryEngine::Game
{
    public:
        MyGame(): m_win("FryTest", 300, 300), m_RenderBuffs{{300, 300}, {300, 300}}
        {

        }

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
                std::cout << "Game has run for " << m_gameRuntime / 1000 << " Seconds!" << std::endl;
                std::cout << "FrameRate is: " << 1000 / DT_ms << " frames per second!" << std::endl << std::endl;
                m_currentSecond = 0;
            }
            

            m_RenderBuffs[m_buffCount].SetColor(red, green, blue);
            for (int i = 0; i < 30; i++)
            {
                m_RenderBuffs[m_buffCount].FillTri(-100, 0, 300 / 2, 300, 400, 0, (255 << 16));
                m_RenderBuffs[m_buffCount].FillTri(300 / 2, 0, 0, 300, 300, 300, 255);
            }
            m_win.Render(m_RenderBuffs[m_buffCount]);
            m_buffCount = (m_buffCount + 1) % 2;
        };

    private:
    TimeDuration m_currentSecond = 0;
    TimeDuration m_gameRuntime = 0;
    Window m_win;
    uint8_t blue = 0;
    uint8_t green = 255;
    uint8_t red = 0;
    RenderTarget m_RenderBuffs[2];
    bool m_buffCount = 0;
};


int main()
{
    _CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
    MyGame game;
    game.Run();
}