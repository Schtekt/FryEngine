#include <Windows.h>
#include <iostream>
#include <chrono>
#include <thread>
#include "FryEngine/Game.h"
#include "FryEngine/ECS/ECS.h"

class DeleteIntClass
{
public:
    DeleteIntClass(int* ptr): m_pNumber(ptr) {};
    DeleteIntClass(const DeleteIntClass& other): m_pNumber(&(*other.m_pNumber)) {};
    ~DeleteIntClass() { ++*m_pNumber; };
    int GetNumber() const { return *m_pNumber; };
private:
    int* m_pNumber;
};

class MyGame : public FryEngine::Game
{
    private:
        void OnInit()
        {
            ECS* ecs = new ECS();
            int myNumber = 5;
    
            size_t entHandle = ecs->CreateEntity();
            Entity* ent = ecs->GetEntity(entHandle);
            ent->AddComponent<DeleteIntClass>(&myNumber);

            Component<DeleteIntClass>* myComponent = ent->GetComponent<DeleteIntClass>();

            delete ecs;

        }

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