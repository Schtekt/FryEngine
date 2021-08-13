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

TEST(ECS, customSystemMultipleComponentTypes)
{
    ECS* ecs = new ECS();

    CreateDeleteIntClassSystem sys;

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
        entities[i]->AddComponent<CreateIntClass>(i);
        ASSERT_EQ(myNumbers[i], origNumbers[i]);
        ASSERT_EQ(*entities[i]->GetComponent<CreateIntClass>()->GetNumber(), i);
    }
    
    std::vector<BaseSystem*> systems;
    systems.push_back(&sys);
    
    ecs->UpdateSystems(0, systems);

    for(size_t i = 0; i < 20; i++)
    {
        ASSERT_EQ(*entities[i]->GetComponent<CreateIntClass>()->GetNumber() - myNumbers[i], 2);
    }


    delete ecs;

    for(size_t i = 0; i < 20; i++)
    {
        ASSERT_EQ(myNumbers[i], origNumbers[i]);
    }
}

TEST(ECS, customSystemMultipleComponentTypesOptional)
{
    ECS* ecs = new ECS();

    SubtractionSystem sys;

    std::vector<BaseSystem*> systems;
    systems.push_back(&sys);

    std::vector<Entity*> entities;

    for(int i = 0; i < 20; i++)
    {
        entities.push_back(ecs->CreateEntity());
    }

    for(int i = 0; i < 20; i++)
    {
        entities[i]->AddComponent<int>(i);

        ASSERT_EQ(*entities[i]->GetComponent<int>(), i);

        if(i % 2 == 0)
        {
            entities[i]->AddComponent<unsigned int>(i);
            ASSERT_EQ(*entities[i]->GetComponent<unsigned int>(), i);
        }
    }

    ecs->UpdateSystems(0, systems);

    for(int i = 0; i < 20; i++)
    {
        if(i % 2 == 0)
        {
            ASSERT_EQ(*entities[i]->GetComponent<int>(), 0);
        }
        else
        {
            ASSERT_EQ(*entities[i]->GetComponent<int>(), i);
        }
    }
    delete ecs;
}

TEST(ECS, multipleCustomSystems)
{
    ECS* ecs = new ECS();

    SubtractionSystem subSys;
    MultiplicationSystem mulSys;

    std::vector<Entity*> entities;
    
    for(int i = 0; i < 20; i++)
    {
        entities.push_back(ecs->CreateEntity());

        entities[i]->AddComponent<int>(i);
        entities[i]->AddComponent<unsigned int>(i);
    }

    // Order matters!
    std::vector<BaseSystem*> systems;
    systems.push_back(&mulSys);
    systems.push_back(&subSys);

    ecs->UpdateSystems(0, systems);

    for(int i = 0; i < 20; i++)
    {
        ASSERT_EQ(*entities[i]->GetComponent<int>(), i * i - i);
    }

    delete ecs;
}