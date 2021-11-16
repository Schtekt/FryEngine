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

class MyGame : public FryEngine::Game
{
    public:
        MyGame(): m_win("FryTest", 300, 300), m_RenderBuffs{{300, 300}, {300, 300}}, m_cam({0.0, 0.0, 0.0}, 0.0, 0.0), m_renderSys(m_cam)
        {
            m_systems.push_back(&m_renderSys);
        }

        void OnInit()
        {
            m_win.Init();

            
            Entity* CubeEnt = m_entities.emplace_back(m_ecs.CreateEntity());

            Mesh* mesh = CubeEnt->AddComponent<Mesh>();
            mesh->ReadFromObj("../Resources/Cube.obj");
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
                //std::cout << "FrameRate is: " << 1000 / DT_ms << " frames per second!" << std::endl << std::endl;
                m_currentSecond = 0;
            }

            Matrix<4, 4> trans
            {
                1, 0, 0, 0,
                0, 1, 0, 0,
                0, 0, 1, 0,
                0, 0, 0, 1
            };

            double rotSpeed = 0.001;
            Matrix<4, 4> rotX
            {
                1, 0, 0, 0,
                0, cos(m_gameRuntime * rotSpeed), sin(m_gameRuntime * rotSpeed), 0,
                0, -sin(m_gameRuntime * rotSpeed), cos(m_gameRuntime * rotSpeed), 0,
                0, 0, 0, 1
            };

            Matrix<4, 4> rotZ
            {
                cos(m_gameRuntime * rotSpeed / 2), -sin(m_gameRuntime * rotSpeed / 2), 0, 0,
                sin(m_gameRuntime * rotSpeed / 2), cos(m_gameRuntime * rotSpeed / 2), 0, 0,
                0, 0, 1, 0,
                0, 0, 0, 1
            };
            m_entities[0]->GetComponent<Mesh>()->SetModelMatrix(rotZ*rotX);

            m_ecs.UpdateSystems(DT_ms, m_systems);
            m_renderSys.Draw(m_RenderBuffs[m_buffCount]);
            m_win.Render(m_RenderBuffs[m_buffCount]);

            m_buffCount = (m_buffCount + 1) % 2;
            m_RenderBuffs[m_buffCount].SetColor(red, green, blue);
            Sleep(1);
        };

    private:
    TimeDuration m_currentSecond = 0;
    TimeDuration m_gameRuntime = 0;
    Window m_win;
    uint8_t blue = 0;
    uint8_t green = 0;
    uint8_t red = 0;
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
    MyGame game;
    game.Run();
    return 0; 
}