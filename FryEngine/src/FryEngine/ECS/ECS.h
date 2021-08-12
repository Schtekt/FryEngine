#pragma once

#include <vector>
#include "ComponentType.h"
#include "Entity.h"
#include "System.h"

class ECS
{
    public:
    ECS(){};
    ~ECS();
    Entity* CreateEntity();
    bool  RemoveEntity(Entity* ent);
    void  UpdateSystems(std::vector<BaseSystem*>& systems);

    private:
    template<typename T, typename ...Args>
    size_t addComponent(Entity* entityHandle, Args&& ... args);
    template<typename T>
    bool removeComponent(size_t componentId);
    bool removeComponent(size_t typeId, size_t compId);
    template<typename T>
    T* getComponent(size_t compIdInList);

    private:
    std::map<size_t, BaseComponentType*> m_ComponentTypes;
    std::map<size_t, std::vector<char>> m_Components;
    std::map<size_t, std::vector<Entity*>> m_ComponentParents;
    std::vector<Entity*> m_Entities;

    friend class Entity;
};


template<typename T, typename ...Args>
size_t ECS::addComponent(Entity* entityHandle, Args&& ... args)
{
    if(m_ComponentTypes.count(ComponentType<T>::sId()) == 0)
    {
        m_ComponentTypes[ComponentType<T>::sId()] = new ComponentType<T>;
    }

    size_t CompListCurrSize = m_Components.size();
    m_Components[ComponentType<T>::sId()].resize(CompListCurrSize + ComponentType<T>::sSizeOfComponent());
    new (&m_Components[ComponentType<T>::sId()].at(CompListCurrSize)) T(std::forward<Args>(args)...);
    int* test (std::forward<Args>(args)...);

    m_ComponentParents[ComponentType<T>::sId()].push_back(entityHandle);

    return m_ComponentParents.size() - 1;
}

template<typename T>
bool ECS::removeComponent(size_t componentId)
{
    // Validate the id.
    if(m_Components.count(ComponentType<T>::sId()) == 0)
    {
        return false;
    }

    size_t toRemoveIndex = componentId * ComponentType<T>::sSizeOfComponent();

    if(m_Components[ComponentType<T>::sId()].size() < toRemoveIndex)
    {
        return false;
    }

    // Remove and free the component
    T* compToRemove = (T*)&m_Components[toRemoveIndex];
    compToRemove->~T();

    // Move the component in the end of the vector to the now free memory and resize the vector
    size_t toMoveIndex = (T*)m_Components.size() - ComponentType<T>::sSizeOfComponent();
    if(toMoveIndex == toRemoveIndex)
    {
        m_Components.resize(toMoveIndex);
        return true;
    }

    T* compToMove = (T*)&m_Components[toMoveIndex];
    memcpy(toRemoveIndex, toMoveIndex, ComponentType<T>::sSizeOfComponent());

    m_ComponentParents[ComponentType<T>::sId][componentId]->setCompId<T>(componentId);
    m_ComponentParents[ComponentType<T>::sId].pop_back();

    return true;
}

template<typename T>
T* ECS::getComponent(size_t compIdInList)
{
    size_t index = compIdInList * ComponentType<T>::sSizeOfComponent();
    if(m_ComponentTypes.count(ComponentType<T>::sId()) == 0 || m_Components[ComponentType<T>::sId()].size() < index)
    {
        return nullptr;
    }
    return (T*)&m_Components[ComponentType<T>::sId()][index];
}