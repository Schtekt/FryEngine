#pragma once

class BaseComponentType
{
    public:
    virtual size_t GetTypeId() const = 0;
    virtual size_t GetSizeOfComponent() const = 0;

    virtual void CallComponentDestructor(void* ptr) = 0;

    protected:
    inline static size_t assignId()
    {
        return m_sIdCount++;
    };
    private:
    static size_t m_sIdCount;
};

template<typename T>
class ComponentType : public BaseComponentType
{
    public:
    size_t GetTypeId() const;
    size_t GetSizeOfComponent() const;

    static size_t sId();
    static size_t sSizeOfComponent();

    void CallComponentDestructor(void* ptr);

    private:
    ComponentType() {};
    static size_t m_sId;
    static size_t m_sSize;

    friend class ECS;
};

template<typename T>
size_t ComponentType<T>::m_sId(BaseComponentType::assignId());

template<typename T>
size_t ComponentType<T>::m_sSize(sizeof(T));

template<typename T>
size_t ComponentType<T>::GetTypeId() const
{
    return m_sId;
}

template<typename T>
size_t ComponentType<T>::GetSizeOfComponent() const
{
    return m_sSize;
}

template<typename T>
size_t ComponentType<T>::sId()
{
    return m_sId;
}

template<typename T>
size_t ComponentType<T>::sSizeOfComponent()
{
    return m_sSize;
}

template<typename T>
void ComponentType<T>::CallComponentDestructor(void* ptr)
{
    ((T*)ptr)->~T();
}