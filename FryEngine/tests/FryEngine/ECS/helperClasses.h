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

class CreateIntClass
{
    public:
    CreateIntClass(int number): m_pNumber(new int(number)) {};
    ~CreateIntClass() {delete m_pNumber; m_pNumber = nullptr;};
    int* GetNumber() {return m_pNumber;};
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

class CreateDeleteIntClassSystem : public BaseSystem
{
    public:
    CreateDeleteIntClassSystem()
    {
        addComponentType<DeleteIntClass>();
        addComponentType<CreateIntClass>();
    }

    inline void UpdateComponents(const long long delta, std::vector<void*>& components)
    {
        DeleteIntClass* delComp = (DeleteIntClass*)components[0];
        CreateIntClass* crComp = (CreateIntClass*)components[1];
        *crComp->GetNumber() += 1;
        delComp->SetNumber(delComp->GetNumber() - 1);
    };
};

class SubtractionSystem : public BaseSystem
{
    public:
    SubtractionSystem()
    {
        addComponentType<int>();
        addComponentType<unsigned int>(CompFlag::Optional);
    }

    inline void UpdateComponents(const long long delta, std::vector<void*>& components)
    {
        int* firstComp = (int*)components[0];
        unsigned int* secondComp = (unsigned int*)components[1];

        if(secondComp)
        {
            *firstComp -= (int)*secondComp;
        }
    };
};

class MultiplicationSystem : public BaseSystem
{
        public:
    MultiplicationSystem()
    {
        addComponentType<int>();
        addComponentType<unsigned int>();
    }

    inline void UpdateComponents(const long long delta, std::vector<void*>& components)
    {
        int* firstComp = (int*)components[0];
        unsigned int* secondComp = (unsigned int*)components[1];

        *firstComp *= *secondComp;
    };
};