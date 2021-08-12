#pragma once
#include "FryEngine/ECS/ECS.h"

class DeleteIntClass
{
public:
    DeleteIntClass(int* ptr): m_pNumber(ptr) {};
    DeleteIntClass(const DeleteIntClass& other): m_pNumber(&(*other.m_pNumber)) {};
    ~DeleteIntClass() { ++*m_pNumber; };
    int GetNumber() const { return *m_pNumber; };
    void SetNumber(int num) {*m_pNumber = num;}
private:
    int* m_pNumber;
};

class DeleteIntClassSystem : public BaseSystem
{
    public:
    DeleteIntClassSystem()
    {
        addComponentType<DeleteIntClass>();
    }

    inline void UpdateComponents(const long long delta, std::vector<void*>& components)
    {
        DeleteIntClass* comp = (DeleteIntClass*)components[0];

        comp->SetNumber(comp->GetNumber() - 1);
    };
};