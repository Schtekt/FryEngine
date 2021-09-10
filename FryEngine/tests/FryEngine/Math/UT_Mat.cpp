#include <gtest/gtest.h>
#include "FryEngine/Math/Matrix.h"

TEST(Matrix, Addition)
{
    Matrix<4,4> matFirst
    {   
        1, 2, 3, 4,
        5, 6, 7, 8,
        9, 10, 11, 12,
        13, 14, 15, 16
    };

    Matrix<4, 4> matSec
    {
        -1, -2, -3, -4,
        -5, -6, -7, -8,
        -9, -10, -11, -12,
        -13, -14, -15, -16
    };

    Matrix<4,4> res = matFirst + matSec;

    Matrix<4,4> expected
    {
        0,0,0,0,
        0,0,0,0,
        0,0,0,0,
        0,0,0,0
    };
   EXPECT_TRUE(res == expected);
}

TEST(Matrix, Subtraction)
{
   Matrix<3,3> matFirst
   {   
       1, 2, 3, 
       4, 5, 6,
       7, 8, 9
   };

   matFirst -= matFirst;

   Matrix<3,3> expected
   {
       0,0,0,
       0,0,0,
       0,0,0
   };

   EXPECT_TRUE(matFirst == expected);
}

TEST(Matrix, Multiplication)
{
    Matrix<1, 3> matFirst
    {
        2, 2, 2
    };

    Matrix<3, 1> matSec
    {
        3,
        3,
        3,    
    };

    Matrix<1,1> res = matFirst * matSec;

    Matrix<1,1> expected
    {
        18
    };

    EXPECT_TRUE(res == expected);
}

TEST(Matrix, Transpose)
{
    Matrix<4,1> matFirst
    {
        1,2,3,4
    };

    Matrix<1,4> nExpected
    {
        1,
        2,
        3,
        4
    };

    EXPECT_FALSE(matFirst == matFirst.Transpose());
    EXPECT_TRUE(matFirst.Transpose() == nExpected);
}

TEST(Matrix, Rotation)
{
    double angle = 3.14159265359;

    // rotate in x 180 degrees in axis
    Matrix<4,4> rotX
    {
        1, 0, 0, 0,
        0, cos(angle), sin(angle), 0,
        0, -sin(angle), cos(angle), 0,
        0, 0, 0, 1
    };

    Vector<4> res{0,0,1,0};

    res = rotX * res;

    // Round the values.
    for(unsigned int i = 0; i < 4; ++i)
    {
       if(abs(res.nums[i]) < 0.0000000001)
       {
           res.nums[i] = 0.0;
       }
    }

    Vector<4> expected{0,0,-1,0};

    EXPECT_TRUE(res == expected);

    // Rotate 90 degrees in y axis

    Matrix<4,4> rotY
    {
        cos(angle / 2), 0, -sin(angle / 2), 0,
        0, 1, 0, 0,
        sin(angle / 2), 0, cos(angle / 2), 0,
        0, 0, 0, 1
    };
    res = rotY * res;
    expected = {1,0,0,0};

    // Round the values.
    for(unsigned int i = 0; i < 4; ++i)
    {
       if(abs(res.nums[i]) < 0.0000000001)
       {
           res.nums[i] = 0.0;
       }
    }
    EXPECT_TRUE(res == expected);

    // Rotate 270 degrees in z axis

    Matrix<4,4> rotZ
    {
        cos(angle * 3 / 2), -sin(angle * 3 / 2), 0, 0,
        sin(angle * 3 / 2), cos(angle * 3 / 2), 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1
    };

    res = rotZ * res;

    // Round the values.
    for(unsigned int i = 0; i < 4; ++i)
    {
       if(abs(res.nums[i]) < 0.0000000001)
       {
           res.nums[i] = 0.0;
       }
    }
    expected = {0, -1, 0, 0};

    EXPECT_TRUE(res == expected);
}