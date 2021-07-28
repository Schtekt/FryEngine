#pragma once

#include <utility>
#include "../Core/Core.h"

class BaseComponent
{
    public:
    BaseComponent(size_t entityId):m_EntityId(entityId) {};
    virtual ~BaseComponent(){};
    private:
    inline static size_t m_sIDCount = 0;
    size_t m_EntityId;

    protected:
    inline static size_t assignID(){ return m_sIDCount++;};
};


template<typename T>
class Component : public BaseComponent
{
    public:
    template<typename ...Args>
    Component(size_t entityId, Args&& ... args);
    ~Component(){};

    T& GetObject();

    static size_t sGetId();
    static size_t sGetSize();
    private:
        static size_t m_sId;
        static size_t m_sSize;
        T m_Object;
};

template<typename T>
size_t Component<T>::m_sId(BaseComponent::assignID());

template<typename T>
size_t Component<T>::m_sSize(sizeof(Component<T>));

template<typename T>
template<typename ...Args>
Component<T>::Component(size_t entityId, Args&& ... args): BaseComponent(entityId), m_Object(std::forward<Args&&>(args)...)
{
};

template <typename T>
T& Component<T>::GetObject(){
    return m_Object;
};

template<typename T>
size_t Component<T>::sGetId()
{
    return m_sId;
}
template<typename T>
size_t Component<T>::sGetSize()
{
    return m_sSize;
}