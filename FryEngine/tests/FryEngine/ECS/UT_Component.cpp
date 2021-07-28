#include <string>
#include <gtest/gtest.h>

#include "FryEngine/ECS/Component.h"

class DummyClass
{
    public:
    inline DummyClass(const std::string& text, const float& number): m_Text(text),m_Number(number){};
    inline DummyClass(std::string && text, float && number): m_Text(std::forward<std::string>(text)), m_Number(number){};

    inline const std::string& GetText(){return m_Text;};
    inline const float& GetNumber(){return m_Number;};

    private:
    std::string m_Text;
    float m_Number;
};

TEST(ECS, ComponentRegisterID)
{
    Component<DummyClass> comp("Hello", 5.f);
    Component<DummyClass> compSec("Bye", 10.f);
    Component<float> compNumber(5.f);

    ASSERT_EQ(comp.sGetID(), compSec.sGetID());
    ASSERT_NE(comp.sGetID(), compNumber.sGetID());
};

TEST(ECS, ComponentGetObject)
{
    Component<DummyClass> comp("Hello", 5.f);

    ASSERT_EQ(comp.GetObject().GetText(), "Hello");
    ASSERT_EQ(comp.GetObject().GetNumber(), 5.f);
}