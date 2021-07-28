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

TEST(ECS, ComponentDeletedWithECS)
{
    ECS* ecs = new ECS();
    int myNumber = 5;
    
    size_t entHandle = ecs->CreateEntity();
    Entity* ent = ecs->GetEntity(entHandle);
    ent->AddComponent<DeleteIntClass>(&myNumber);

    Component<DeleteIntClass>* myComponent = ent->GetComponent<DeleteIntClass>();

    ASSERT_EQ(myNumber, myComponent->GetObject().GetNumber());

    delete ecs;

    ASSERT_NE(myNumber, 5);
}