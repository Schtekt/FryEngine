#include <gtest/gtest.h>
#include "FryEngine/Rendering/RenderTarget.h"

TEST(RenderTarget, FillTarget)
{
    int width = 100;
    int height = 100;

    RenderTarget target(width, height);

    int nrOfPixels = width * height;
    
    // set black background
    target.SetColor(0,255,0);

    const uint32_t* pixels = target.GetPixels();

    // Check that it is green everywhere!
    for(int i = 0; i < nrOfPixels; ++i)
    {
        ASSERT_EQ(pixels[i], 255 << 8);
    }
}

TEST(RenderTarget, DrawLine)
{
    // Important that width and height is equal for this test!
    int width = 100;
    int height = 100;
    int nrOfPixels = width * height;
    RenderTarget target(width, height);

    uint32_t redColor = 255 << 16;
    target.DrawLine(0, 0, width, height, redColor);

    const uint32_t* pixels = target.GetPixels();

    for(int i = 0; i < nrOfPixels; ++i)
    {

        if(i % target.GetWidth() == i / target.GetWidth())
        {
            if(pixels[i] != redColor)
                std::cout << i << std::endl;
            ASSERT_EQ(pixels[i], redColor);
        }
        else
        {
            ASSERT_EQ(pixels[i], 0);
        }
    }
}