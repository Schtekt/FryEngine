#include <gtest/gtest.h>
#include <iostream>
#include "helperClasses.h"

TEST(ECS, customSystemSingleComponent)
{
    ECS* ecs = new ECS();

    DeleteIntClassSystem sys;

    Entity* ent = ecs->CreateEntity();
    
    int myNumber = 5;
    ent->AddComponent<DeleteIntClass>(&myNumber);

    ASSERT_EQ(myNumber, 5);

    std::vector<BaseSystem*> systems;
    systems.push_back(&sys);
    ecs->UpdateSystems(0, systems);

    ASSERT_EQ(myNumber, 4);


    delete ecs;
    ASSERT_EQ(myNumber, 5);
}

TEST(ECS, customSystemSingleComponentType)
{
    ECS* ecs = new ECS();

    DeleteIntClassSystem sys;

    std::vector<int> myNumbers;
    std::vector<int> origNumbers;
    std::vector<Entity*> entities;

    for(int i = 0; i < 20; i++)
    {
        myNumbers.push_back(i);
        origNumbers.push_back(i);
        entities.push_back(ecs->CreateEntity());
    }

    for(int i = 0; i < 20; i++)
    {
        entities[i]->AddComponent<DeleteIntClass>(&myNumbers[i]);

        ASSERT_EQ(myNumbers[i], origNumbers[i]);
    }
    
    std::vector<BaseSystem*> systems;
    systems.push_back(&sys);
    
    ecs->UpdateSystems(0, systems);

    for(size_t i = 0; i < 20; i++)
    {
        ASSERT_EQ(myNumbers[i], origNumbers[i] - 1);
    }


    delete ecs;

    for(size_t i = 0; i < 20; i++)
    {
        ASSERT_EQ(myNumbers[i], origNumbers[i]);
    }
}