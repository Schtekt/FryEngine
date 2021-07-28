#pragma once

#include "Component.h"
#include <vector>
#include <map>

struct ECSEntry
{
    size_t sizeOfComponent;
    std::vector<uint8_t> compList;
};

class ECS
{
    public:
    ECS() = default;
    ~ECS();
    template<typename T, typename ...Args>
    Component<T>* AddComponent(Args&& ... args);

    private:
    std::map<size_t, ECSEntry> m_Components;
};

ECS::~ECS()
{
    for(auto pair : m_Components)
    {
        size_t sizeOfComponent = pair.second.sizeOfComponent;
        std::vector<uint8_t>& compList = pair.second.compList;
        size_t sizeOfList = compList.size();

        for(size_t i = 0; i < sizeOfList; i += sizeOfComponent)
        {
            ((BaseComponent*)&compList[i])->~BaseComponent();
        }

        compList.resize(0);
    }
};

template<typename T, typename ...Args>
Component<T>* ECS::AddComponent(Args&&... args)
{
    size_t id = Component<T>::sGetID();
    size_t size = Component<T>::sGetSize();
    std::vector<uint8_t>& compList =  m_Components[id].compList;

    // If more infos are added, an if may be viable.
    m_Components[id].sizeOfComponent = size;

    size_t currSize = compList.size();
    compList.resize(currSize + size);
    return new (&compList[currSize]) Component<T>(std::forward<Args...>(args)...);
}