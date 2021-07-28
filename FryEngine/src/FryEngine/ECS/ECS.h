#pragma once

#include "Component.h"
#include <vector>
#include <map>
#include "../Core/Core.h"

class ECS;

struct ECSEntry
{
    size_t sizeOfComponent;
    std::vector<uint8_t> compList;
};

class Entity
{
    public:
    template<typename T, typename ...Args>
    void AddComponent(Args&& ... args);
    template<typename T>
    Component<T>* GetComponent();
    
    private:
    FRY_EXPORT Entity(ECS* ecs, size_t id);


    std::map<size_t, size_t> m_Components;
    ECS* m_pEcs;
    size_t m_Id;
    
    friend class ECS;
};

template<typename T, typename ...Args>
void Entity::AddComponent(Args&& ... args)
{
    size_t typeId = Component<T>::sGetId();
    size_t compId = m_pEcs->AddComponent<T>(m_Id, std::forward<Args...>(args)...);

    m_Components[typeId] = compId;
}

template<typename T>
Component<T>* Entity::GetComponent()
{
    return m_pEcs->GetComponent<T>(m_Components[Component<T>::sGetId()]);
}

class ECS
{
    public:
    FRY_EXPORT ECS();
    FRY_EXPORT ~ECS();

    size_t FRY_EXPORT CreateEntity();
    Entity FRY_EXPORT *GetEntity(size_t entityId);

    private:
    template<typename T, typename ...Args>
    size_t AddComponent(size_t entityId, Args&& ... args);
    template<typename T>
    Component<T>* GetComponent(size_t compId);

    std::map<size_t, ECSEntry> m_Components;
    std::vector<uint8_t> m_Entities;
    friend class Entity;
};

template<typename T, typename ...Args>
size_t ECS::AddComponent(size_t entityId, Args&&... args)
{
    size_t typeId = Component<T>::sGetId();
    size_t size = Component<T>::sGetSize();
    std::vector<uint8_t>& compList =  m_Components[typeId].compList;

    // If more infos are added, an if may be viable.
    m_Components[typeId].sizeOfComponent = size;

    size_t currSize = compList.size();
    compList.resize(currSize + size);
    new (&compList[currSize]) Component<T>(entityId, std::forward<Args...>(args)...);
    return currSize / size;
}

template<typename T>
Component<T>* ECS::GetComponent(size_t compId)
{
    return (Component<T>*)&m_Components[Component<T>::sGetId()].compList.at(compId);
}