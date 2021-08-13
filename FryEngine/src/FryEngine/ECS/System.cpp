#include "System.h"

std::vector<std::pair<size_t, CompFlag>>& BaseSystem::GetCompTypes()
{
    return m_ComponentTypes;
}

void BaseSystem::addComponentType(const size_t typeId, const CompFlag flag)
{
    m_ComponentTypes.emplace_back(typeId, flag);
}