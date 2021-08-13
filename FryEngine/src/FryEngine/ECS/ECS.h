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
    void  UpdateSystems(long long dt, std::vector<BaseSystem*>& systems);

    private:
    template<typename T, typename ...Args>
    size_t addComponent(Entity* entityHandle, Args&& ... args);
    bool removeComponent(size_t typeId, size_t compId);
    template<typename T>
    T* getComponent(size_t compIdInList);
    void* getComponent(size_t typeId, size_t compIdInList);

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

    size_t CompListCurrSize = m_Components[ComponentType<T>::sId()].size();
    m_Components[ComponentType<T>::sId()].resize(CompListCurrSize + ComponentType<T>::sSizeOfComponent());
    new (&m_Components[ComponentType<T>::sId()].at(CompListCurrSize)) T(std::forward<Args>(args)...);

    m_ComponentParents[ComponentType<T>::sId()].push_back(entityHandle);

    return m_ComponentParents[ComponentType<T>::sId()].size() - 1;
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