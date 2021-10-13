#pragma once
#include <vector>
#include "ComponentType.h"

enum class CompFlag
{Required = 0, Optional = 1};

class BaseSystem
{
    public:
    BaseSystem() {};
    virtual void UpdateComponents(const long long delta, std::vector<void*>& components) = 0;
    std::vector<std::pair<size_t, CompFlag>>& GetCompTypes();

    protected:
    template<typename T>
    void addComponentType(const CompFlag flag = CompFlag::Required);
    void addComponentType(const size_t typeId, const CompFlag flag = CompFlag::Required);

    private:
    std::vector<std::pair<size_t, CompFlag>> m_ComponentTypes;
};

template<typename T>
void BaseSystem::addComponentType(const CompFlag flag)
{
    addComponentType(ComponentType<T>::sId(), flag);
}