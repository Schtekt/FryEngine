#include <Windows.h>
#include <iostream>
#include <chrono>
#include <thread>
#include <crtdbg.h>
#include "FryEngine/Game.h"
#include "FryEngine/ECS/ECS.h"
#include "FryEngine/Rendering/Window.h"
#include "FryEngine/Rendering/Camera.h"
#include "FryEngine/Rendering/Mesh.h"

class MyGame : public FryEngine::Game
{
    public:
        MyGame(): m_win("FryTest", 300, 300), m_RenderBuffs{{300, 300}, {300, 300}}, m_cam({0.0, 0.0, 0.0}, 0.0, 0.0)
        {

        }

        void OnInit()
        {
            m_win.Init();
            std::vector<Vertex> vertices;
            // Box.
            vertices.push_back({-0.5, -0.5, -0.5, 1.0});
            vertices.push_back({-0.5, 0.5, -0.5, 1.0});
            vertices.push_back({0.5,  0.5, -0.5, 1.0});
            vertices.push_back({0.5, -0.5, -0.5, 1.0});
            vertices.push_back({0.5,  0.5, 0.5, 1.0});
            vertices.push_back({0.5, -0.5, 0.5, 1.0});
            vertices.push_back({-0.5, 0.5, 0.5, 1.0});
            vertices.push_back({-0.5, -0.5, 0.5, 1.0});

            std::vector<unsigned int> vertexIndices;

            // South
            vertexIndices.push_back(0);
            vertexIndices.push_back(1);
            vertexIndices.push_back(2);
            
            vertexIndices.push_back(0);
            vertexIndices.push_back(2);
            vertexIndices.push_back(3);
            
            // East
            vertexIndices.push_back(3);
            vertexIndices.push_back(2);
            vertexIndices.push_back(4);
            
            vertexIndices.push_back(3);
            vertexIndices.push_back(4);
            vertexIndices.push_back(5);
            
            // North
            vertexIndices.push_back(5);
            vertexIndices.push_back(4);
            vertexIndices.push_back(6);
            
            vertexIndices.push_back(5);
            vertexIndices.push_back(6);
            vertexIndices.push_back(7);
            
            // West
            vertexIndices.push_back(7);
            vertexIndices.push_back(6);
            vertexIndices.push_back(1);
            
            vertexIndices.push_back(7);
            vertexIndices.push_back(1);
            vertexIndices.push_back(0);
            
            // Top
            vertexIndices.push_back(1);
            vertexIndices.push_back(6);
            vertexIndices.push_back(4);
            
            vertexIndices.push_back(1);
            vertexIndices.push_back(4);
            vertexIndices.push_back(2);
            
            // Bottom
            vertexIndices.push_back(5);
            vertexIndices.push_back(7);
            vertexIndices.push_back(0);
            
            vertexIndices.push_back(5);
            vertexIndices.push_back(0);
            vertexIndices.push_back(3);

            m_Mesh.SetVertices(vertices, vertexIndices);
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

            Matrix<4, 4> rotX
            {
                1, 0, 0, 0,
                0, cos(m_gameRuntime * 0.005), sin(m_gameRuntime * 0.005), 0,
                0, -sin(m_gameRuntime * 0.005), cos(m_gameRuntime * 0.005), 0,
                0, 0, 0, 1
            };

            Matrix<4, 4> rotZ
            {
                cos(m_gameRuntime * 0.005 / 2), -sin(m_gameRuntime * 0.005 / 2), 0, 0,
                sin(m_gameRuntime * 0.005 / 2), cos(m_gameRuntime * 0.005 / 2), 0, 0,
                0, 0, 1, 0,
                0, 0, 0, 1
            };

            m_Mesh.SetModelMatrix(trans * rotX /** rotZ*/);
            

            m_RenderBuffs[m_buffCount].SetColor(red, green, blue);
            m_Mesh.Draw(m_cam.GetViewMat(), m_cam.GetProjectionMatrix(), m_RenderBuffs[m_buffCount]);
            
            m_win.Render(m_RenderBuffs[m_buffCount]);
            m_buffCount = (m_buffCount + 1) % 2;
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
    Mesh m_Mesh;
};


int main()
{
    _CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
    MyGame game;
    game.Run();
}