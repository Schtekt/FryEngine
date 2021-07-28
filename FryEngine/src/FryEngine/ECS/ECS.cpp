#include "ECS.h"

Entity::Entity(ECS* ecs, size_t id) : m_pEcs(ecs), m_Id(id){};

ECS::ECS(){};

size_t ECS::CreateEntity()
{
    size_t entSize = sizeof(Entity);
    size_t id = m_Entities.size() / entSize;
    size_t currSize = m_Entities.size();
    m_Entities.resize(currSize + entSize);
    new (&m_Entities[currSize]) Entity(this, id);
    //m_Entities.emplace_back(this, id);
    return id;
}

Entity* ECS::GetEntity(size_t entityId)
{
    return (Entity*)&m_Entities.at(entityId * sizeof(Entity));
}

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