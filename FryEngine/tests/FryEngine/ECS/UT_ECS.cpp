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
    Component<DeleteIntClass>* myComponent = ecs->AddComponent<DeleteIntClass>(&myNumber);

    ASSERT_EQ(myNumber, myComponent->GetObject().GetNumber());

    delete ecs;

    ASSERT_NE(myNumber, 5);
}