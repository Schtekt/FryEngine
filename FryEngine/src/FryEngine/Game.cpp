#include "Game.h"
#include <chrono>

namespace FryEngine
{
    void Game::Run()
    {
        auto CurrentFrameTimePoint = std::chrono::system_clock::now();
        auto LastFrameTimePoint = CurrentFrameTimePoint;
        
        this->OnInit();
        while(m_isRunning)
        {
            CurrentFrameTimePoint = std::chrono::system_clock::now();
            OnUpdate(std::chrono::duration_cast<std::chrono::milliseconds>(CurrentFrameTimePoint - LastFrameTimePoint).count());
            OnRender();
            LastFrameTimePoint = CurrentFrameTimePoint;
        }
    }

    void Game::CloseGame()
    {
        m_isRunning = false;
    }
}