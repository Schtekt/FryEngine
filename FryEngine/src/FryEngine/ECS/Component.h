#pragma once

#include <utility>
class BaseComponent
{
    public:
    virtual ~BaseComponent(){};
    private:
    inline static size_t m_sIDCount = 0;

    protected:
    inline static size_t assignID(){ return m_sIDCount++;};
};


template<typename T>
class Component : public BaseComponent
{
    public:
    template<typename ...Args>
    Component(Args&& ... args);
    ~Component(){};

    T& GetObject();

    static size_t sGetID();
    static size_t sGetSize();
    private:
        static size_t m_sID;
        static size_t m_sSize;
        T m_Object;
};

template<typename T>
size_t Component<T>::m_sID(BaseComponent::assignID());

template<typename T>
size_t Component<T>::m_sSize(sizeof(Component<T>));

template<typename T>
template<typename ...Args>
Component<T>::Component(Args&& ... args): m_Object(std::forward<Args&&>(args)...)
{
};

template <typename T>
T& Component<T>::GetObject(){
    return m_Object;
};

template<typename T>
size_t Component<T>::sGetID()
{
    return m_sID;
}
template<typename T>
size_t Component<T>::sGetSize()
{
    return m_sSize;
}