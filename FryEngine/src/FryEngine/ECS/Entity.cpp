#include "Entity.h"
#include "Entity.h"
#include "ECS.h"

Entity::~Entity()
{
    if (m_Components.size() > 0)
    {
        for (auto comp : m_Components)
        {
            m_pECSParent->removeComponent(comp.first, comp.second);
        }
    }
}

bool Entity::RemoveComponent(size_t typeId)
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