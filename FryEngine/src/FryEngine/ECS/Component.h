#include <utility>
class BaseComponent
{
    public:
    inline size_t GetId() {return m_ID;};
    private:
    static size_t m_sIDCount;

    protected:
    size_t m_ID;
    inline static size_t assignID(){ return m_sIDCount++;};
};

size_t BaseComponent::m_sIDCount = 0;

template<typename T>
class Component : public BaseComponent
{
    public:
    template<typename ...Args>
    Component(Args&& ... args);
    ~Component(){};

    T& GetObject();
    private:
        static size_t m_sID;
        T m_Object;
};

template<typename T>
size_t Component<T>::m_sID(BaseComponent::assignID());

template<typename T>
template<typename ...Args>
Component<T>::Component(Args&& ... args): m_Object(std::forward<Args&&>(args)...){m_ID = m_sID;};

template <typename T>
T& Component<T>::GetObject(){
    return m_Object;
};