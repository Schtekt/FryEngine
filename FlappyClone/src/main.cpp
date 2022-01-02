#include <Windows.h>
#include <iostream>
#include <chrono>
#include <thread>
#include <crtdbg.h>
#include "FryEngine/Game.h"
#include "FryEngine/ECS/ECS.h"
#include "FryEngine/Rendering/Window.h"
#include "FryEngine/Rendering/Camera.h"
#include "FryEngine/Rendering/TriCollector.h"
#include "FryEngine/ECS/RenderSystem.h"
#include "FryEngine/Input/KeyboardState.h"

class FlappyClone : public FryEngine::Game
{
    public:
        FlappyClone(): m_win("FlappyClone", 300, 300), m_RenderBuffs{{300, 300}, {300, 300}}, m_cam({0.0, 0.0, 0.0}, 0.0, 0.0), m_renderSys(m_cam)
        {
        }

        void OnInit()
        {
            m_win.Init();
        }

        void OnUpdate(TimeDuration DT_ms)
        {
            if(m_win.GetKeyboard().GetKeyState(VK_ESCAPE) || !m_win.ProcessMessage())
            {
                CloseGame();
            }

            m_currentSecond += DT_ms;
            m_gameRuntime += DT_ms;
            if(DT_ms != 0)
                m_win.SetWindowName("FryTest" + std::to_string(1000/DT_ms));

            if(m_currentSecond > 1000 && DT_ms != 0)
            {
                m_currentSecond = 0;
            }

            m_ecs.UpdateSystems(DT_ms, m_systems);
            m_renderSys.Draw(m_RenderBuffs[m_buffCount]);
            m_win.Render(m_RenderBuffs[m_buffCount]);

            m_buffCount = (m_buffCount + 1) % 2;
            m_RenderBuffs[m_buffCount].SetColor(0, 0, 0);
        };

    private:
    TimeDuration m_currentSecond = 0;
    TimeDuration m_gameRuntime = 0;
    Window m_win;
    RenderTarget m_RenderBuffs[2];
    bool m_buffCount = 0;
    FPSCamera m_cam;
    ECS m_ecs;
    std::vector<Entity*> m_entities;

    std::vector<BaseSystem*> m_systems;
    RenderSystem m_renderSys;
};


int main()
{
    _CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
    FlappyClone game;
    game.Run();
    return 0; 
}