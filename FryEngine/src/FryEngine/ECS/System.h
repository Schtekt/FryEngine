#pragma once
#include <vector>

class BaseSystem
{
    public:
    BaseSystem() {};
    virtual void UpdateComponents(float delta, void** Components);
    private:
    std::vector<size_t> m_ComponentTypes;
};