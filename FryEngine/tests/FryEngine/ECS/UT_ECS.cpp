#include <gtest/gtest.h>
#include <iostream>
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

TEST(ECS, RemoveUnownedEntity)
{
    ECS* ecs = new ECS();
    Entity* ent = ecs->CreateEntity();

    ASSERT_FALSE(ecs->RemoveEntity(nullptr));
    delete ecs;
}

TEST(ECS, RemoveEntity)
{
    ECS* ecs = new ECS();
    Entity* ent = ecs->CreateEntity();

    ASSERT_TRUE(ecs->RemoveEntity(ent));
    delete ecs;
}

TEST(ECS, ComponentDeletedWithECS)
{
    ECS* ecs = new ECS();
    int myNumber = 5;
    
    Entity* ent = ecs->CreateEntity();
    ent->AddComponent<DeleteIntClass>(&myNumber);

    DeleteIntClass* myComponent = ent->GetComponent<DeleteIntClass>();
    ASSERT_NE(myComponent, nullptr);
    ASSERT_EQ(myNumber, myComponent->GetNumber());
    delete ecs;
    ASSERT_NE(myNumber, 5);
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
}