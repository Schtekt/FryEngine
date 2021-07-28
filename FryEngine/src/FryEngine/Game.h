#pragma once
namespace FryEngine
{
#define TimeDuration long long
    class Game
    {
        public:
            Game(){};
            virtual ~Game(){};
            void Run();
            void CloseGame();
            virtual void OnInit(){};
            virtual void OnUpdate(TimeDuration DT_ms){};
            virtual void OnRender(){};


        private:
            bool m_isRunning = true;
    };
}