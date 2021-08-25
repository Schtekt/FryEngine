#include <gtest/gtest.h>
#include <iostream>
#include "helperClasses.h"

TEST(ECS, DummyTest)
{
    ASSERT_EQ(0, 0);
}


TEST(ECS, CreationAndDestruction)
{
    ECS* ecs = new ECS();
    delete ecs;
}

TEST(ECS, RemoveUnownedEntity)
{
    ECS* ecs = new ECS();
    Entity* ent = ecs->CreateEntity();

    ASSERT_FALSE(ecs->RemoveEntity(nullptr));
    delete ecs;
}

TEST(ECS, RemoveEntity)
{
    ECS ecs;
    Entity* ent = ecs.CreateEntity();

    ASSERT_TRUE(ecs.RemoveEntity(ent));
}

TEST(ECS, ComponentDeletedWithECS)
{
    ECS* ecs = new ECS();
    int myNumber = 5;
    
    Entity* ent = ecs->CreateEntity();
    ent->AddComponent<DeleteIntClass>(&myNumber);

    //DeleteIntClass* myComponent = ent->GetComponent<DeleteIntClass>();
    //ASSERT_NE(myComponent, nullptr);
    //ASSERT_EQ(myNumber, myComponent->GetNumber());
    delete ecs;
    //ASSERT_NE(myNumber, 5);
}


TEST(ECS, ComponentDeletedWithEntity)
{
    ECS* ecs = new ECS();
    int myNumber = 5;
    
    Entity* ent = ecs->CreateEntity();
    ent->AddComponent<DeleteIntClass>(&myNumber);

    DeleteIntClass* myComponent = ent->GetComponent<DeleteIntClass>();

    ASSERT_EQ(myNumber, myComponent->GetNumber());

    ecs->RemoveEntity(ent);

    ASSERT_NE(myNumber, 5);
    
    delete ecs;

    ASSERT_NE(myNumber, 5);
}

TEST(ECS, ComponentDeletedFromEntity)
{
    ECS* ecs = new ECS();
    int myNumber = 5;
    
    Entity* ent = ecs->CreateEntity();
    ent->AddComponent<DeleteIntClass>(&myNumber);

    DeleteIntClass* myComponent = ent->GetComponent<DeleteIntClass>();

    ASSERT_EQ(myNumber, myComponent->GetNumber());

    ent->RemoveComponent<DeleteIntClass>();

    ASSERT_NE(myNumber, 5);

    delete ecs;

    ASSERT_NE(myNumber, 5);
}