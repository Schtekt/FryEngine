#include "ECS.h"
ECS::~ECS()
{
    for(auto ent : m_Entities)
    {
        delete ent;
    }
    m_Entities.clear();
}

Entity* ECS::CreateEntity()
{
    m_Entities.push_back(new Entity(this));
    return m_Entities[m_Entities.size() - 1];
}

bool ECS::RemoveEntity(Entity* ent)
{
    // Make sure that this ecs holds that entity.
    if (std::find(m_Entities.begin(), m_Entities.end(), ent) == m_Entities.end())
    {
        return false;
    }

    std::map<size_t, size_t>& comps = ent->getComponents();
    std::vector<size_t> compTypes;
    for(auto comp: comps)
    {
        compTypes.push_back(comp.first);
    }
    for (auto compType : compTypes)
    {
        ent->RemoveComponent(compType);
    }
    return true;
}

bool ECS::removeComponent(size_t typeId, size_t compId)
{
    // Validate the id.
    if(m_Components.count(typeId) == 0)
    {
        return false;
    }
    size_t sizeOfComponent = m_ComponentTypes[typeId]->GetSizeOfComponent();
    size_t toRemoveIndex = compId * sizeOfComponent;

    if(m_Components[typeId].size() < toRemoveIndex)
    {
        return false;
    }

    std::vector<char>& ComponentsOfInterest = m_Components[typeId];

    // Call destructor of the component.
    void* compToRemove = (void*)&ComponentsOfInterest[toRemoveIndex];
    m_ComponentTypes[typeId]->CallComponentDestructor(compToRemove);

    // Make a switch between the last component of the vector and the now removed component.
    size_t lastComponentIndex = ComponentsOfInterest.size() - sizeOfComponent;

    if(lastComponentIndex != toRemoveIndex)
    {
        memcpy(&ComponentsOfInterest[toRemoveIndex], &ComponentsOfInterest[lastComponentIndex], sizeOfComponent);
        
        // Make sure the parent points to the right id after move.
        m_ComponentParents[typeId][compId]->setCompId(typeId, compId);
    }

    // Remove references to the now deleted component.
    ComponentsOfInterest.resize(lastComponentIndex);
    m_ComponentParents[typeId].pop_back();

    return true;
}