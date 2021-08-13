#include "ECS.h"
ECS::~ECS()
{
    for (int i = 0; i < m_Entities.size(); i++)
    {
        delete m_Entities[i];
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
        ent->removeComponent(compType);
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

    std::vector<char>& componentsOfInterest = m_Components[typeId];

    // Call destructor of the component.
    void* compToRemove = (void*)&componentsOfInterest[toRemoveIndex];
    m_ComponentTypes[typeId]->CallComponentDestructor(compToRemove);

    // Make a switch between the last component of the vector and the now removed component.
    size_t lastComponentIndex = componentsOfInterest.size() - sizeOfComponent;

    if(lastComponentIndex != toRemoveIndex)
    {
        memcpy(&componentsOfInterest[toRemoveIndex], &componentsOfInterest[lastComponentIndex], sizeOfComponent);
        
        // Make sure the parent points to the right id after move.
        m_ComponentParents[typeId][lastComponentIndex / sizeOfComponent]->setCompId(typeId, compId);
    }

    // Remove references to the now deleted component.
    componentsOfInterest.resize(lastComponentIndex);
    m_ComponentParents[typeId].pop_back();

    return true;
}

void ECS::UpdateSystems(long long dt, std::vector<BaseSystem*>&systems)
{
    for(auto system: systems)
    {
        const std::vector<std::pair<size_t, CompFlag>>& compTypes = system->GetCompTypes();
        
        // If the ECS does not have the required components, skip the system.
        bool componentsExistInEcs = true;
        for(auto compType : compTypes)
        {
            if(m_Components[compType.first].size() == 0
                && compType.second == CompFlag::Required)
            {
                componentsExistInEcs = false;
                break;
            }
        }

        if(!componentsExistInEcs)
        {
            continue;
        }
        
        std::vector<void*> componentsToSend;

        // If there is only one componenttype, we simply send that for update in the system.
        if(compTypes.size() == 1)
        {
            std::vector<char> compList = m_Components[compTypes[0].first];
            size_t sizeOfComponent = m_ComponentTypes[compTypes[0].first]->GetSizeOfComponent();
            size_t sizeOfVector = m_Components[compTypes[0].first].size();
            for(size_t i = 0; i < sizeOfVector; i += sizeOfComponent)
            {
                componentsToSend.push_back(&compList[i]);
                system->UpdateComponents(dt, componentsToSend);
                componentsToSend.clear();
            }
        }
        // If there are several component types, we need to check so that each entity has all required components.
        // (we also need to make sure to send the optional ones)
        else if(compTypes.size() > 1)
        {
            for(auto entityParent : m_ComponentParents[(*compTypes.begin()).first])
            {
                bool sendComponentSetForUpdate = true;
                for(auto compType = compTypes.begin(); compType != compTypes.end(); compType++)
                {
                    void* comp = entityParent->getComponent(compType->first);
                    if(comp == nullptr && compType->second == CompFlag::Required)
                    {
                        sendComponentSetForUpdate = false;
                        break;
                    }
                    componentsToSend.push_back(comp);
                }

                if(sendComponentSetForUpdate)
                {
                    system->UpdateComponents(dt, componentsToSend);
                }

                componentsToSend.clear();
            }
        }
    }
}

void* ECS::getComponent(size_t typeId, size_t compIdInList)
{
    size_t index = compIdInList * m_ComponentTypes[typeId]->GetSizeOfComponent();
    if(m_ComponentTypes.count(typeId) == 0 || m_Components[typeId].size() < index)
    {
        return nullptr;
    }
    return (void*)&m_Components[typeId][index];
}