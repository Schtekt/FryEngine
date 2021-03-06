#include "Entity.h"
#include "Entity.h"
#include "ECS.h"

Entity::~Entity()
{
    for (auto comp : m_Components)
    {
        m_pECSParent->removeComponent(comp.first, comp.second);
    }
}

bool Entity::removeComponent(size_t typeId)
{
    if (m_Components.count(typeId) == 1)
    {
        if (m_pECSParent->removeComponent(typeId, m_Components[typeId]))
        {
            m_Components.erase(typeId);
            return true;
        }
    }
    return false;
}

void* Entity::getComponent(size_t typeId)
{
    if(m_Components.count(typeId) == 1)
    {
        return (void*)m_pECSParent->getComponent(typeId, m_Components[typeId]);
    }
    else
    {
        return nullptr;
    }
}

bool Entity::setCompId(const size_t compType, const size_t id)
{
    if(m_Components.count(compType) == 0)
    {
        return false;
    }
    m_Components[compType] = id;
    return true;
}

Entity::Entity(ECS* parent) : m_pECSParent(parent) 
{
}