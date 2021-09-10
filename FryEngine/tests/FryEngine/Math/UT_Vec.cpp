#include <gtest/gtest.h>
#include "FryEngine/Math/Vector.h"

TEST(Vector, Addition)
{
    Vector<4> vecFirst{4,3,2,1};
    Vector<4> vecSec{1,2,3,4};
    Vector<4> vecThird(vecFirst + vecSec);
    Vector<4> expectedResult{5,5,5,5};

    ASSERT_EQ(vecThird, expectedResult);
}

TEST(Vector, Subtraction)
{
    Vector<3> vecFirst{0.0, -1.0, 1500};
    Vector<3> vecSec{0.0, -1.0, 1500};

    Vector<3> vecRes{vecFirst - vecSec};
    Vector<3> expectedResult{0,0,0};

    ASSERT_EQ(vecRes, expectedResult);
}

TEST(Vector, ScalarMult)
{
    Vector<5> vecFirst{5, 5, 5, 5, 5};
    vecFirst *= 5;

    Vector<5> expectedResult{25, 25, 25, 25, 25};

    ASSERT_EQ(vecFirst, expectedResult);
}

TEST(Vector, ScalarDiv)
{
    Vector<5> vecFirst{25, 25, 25, 25, 25};
    vecFirst /= 5;

    Vector<5> expectedResult{5, 5, 5, 5, 5};

    ASSERT_EQ(vecFirst, expectedResult);
}

TEST(Vector, DotProd)
{
    Vector<2> vec{1,2};
    double res = vec.Dot(vec);

    ASSERT_EQ(res, 5);

}

TEST(Vector, CrossProd)
{
    Vector<3> vecFirst{1,0,0};
    Vector<3> vecSec{0,1,0};
    Vector<3> vecThird{0,0,1};

    ASSERT_EQ(vecFirst.Cross(vecSec), vecThird);
    ASSERT_EQ(vecThird.Cross(vecFirst), vecSec);
    ASSERT_EQ(vecSec.Cross(vecThird), vecFirst);
}