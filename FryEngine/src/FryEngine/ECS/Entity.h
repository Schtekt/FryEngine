#pragma once
#include <map>
#include "ComponentType.h"
class ECS;

class Entity
{
    public:

    ~Entity();
    template<typename T, typename ...Args>
    T* AddComponent(Args&&... args);
    
    template<typename T>
    bool RemoveComponent();
    bool RemoveComponent(size_t typeId);
    
    template<typename T>
    T* GetComponent();
    void* GetComponent(size_t typeId);

    private:
    Entity(ECS* parent);
    template<typename T>
    bool setCompId(const size_t id);
    bool setCompId(const size_t type, const size_t id);
    inline std::map<size_t, size_t>& getComponents()
    {
        return m_Components;
    };


    std::map<size_t, size_t> m_Components;
    ECS * m_pECSParent;

    friend class ECS;
};

template<typename T, typename ...Args>
T* Entity::AddComponent(Args&&... args)
{
    size_t id = m_pECSParent->addComponent<T>(this, std::forward<Args&&>(args)...);
    m_Components[ComponentType<T>::sId()] = id;
    return m_pECSParent->getComponent<T>(m_Components[ComponentType<T>::sId()]);
}

template<typename T>
bool Entity::RemoveComponent()
{
    return RemoveComponent(ComponentType<T>::sId());
};

template<typename T>
T* Entity::GetComponent()
{
    if(m_Components.count(ComponentType<T>::sId()) == 1)
    {
        return (T*)m_pECSParent->getComponent<T>(m_Components[ComponentType<T>::sId()]);
    }
    else
    {
        return nullptr;
    }
};


template<typename T>
bool Entity::setCompId(const size_t id)
{
    return setCompId(ComponentType<T>::sId(), id);
};