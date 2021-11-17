#include "RenderTarget.h"
#include <memory.h>
#include <math.h>
#include <algorithm>

RenderTarget::RenderTarget( unsigned int width, unsigned int height) : m_width(width), m_height(height)
{
    m_pPixels = new uint32_t[(size_t)m_width * m_height];
    memset(m_pPixels, 0, sizeof(uint32_t) * m_width * m_height);
}

RenderTarget::RenderTarget(const RenderTarget& other) : m_width(other.m_width), m_height(other.m_height)
{
    m_pPixels = new uint32_t[(size_t)m_width * m_height];
    memcpy(m_pPixels, other.m_pPixels, (size_t)m_width * m_height * sizeof(uint32_t));
}

RenderTarget::~RenderTarget()
{
    delete m_pPixels;
}

unsigned int RenderTarget::GetWidth() const
{
    return m_width;
}

unsigned int RenderTarget::GetHeight() const
{
    return m_height;
}

uint32_t RenderTarget::GetColor( unsigned int x, unsigned int y) const
{
    if(x < m_width && y < m_height)
    {
        return m_pPixels[y*m_width + x];
    }
    return 0;
}

const uint32_t* RenderTarget::GetPixels() const
{
    return m_pPixels;
}

bool RenderTarget::SetPixelColor( unsigned int x, unsigned int y, uint32_t color)
{
    if(x < m_width && y < m_height)
    {
        m_pPixels[y*m_width + x] = color;
        return true;
    }
    return false;
}

bool RenderTarget::SetPixelColor( unsigned int x, unsigned int y, uint8_t red, uint8_t green, uint8_t blue)
{
    return SetPixelColor(x, y, (red << 16) | (green << 8) | blue);
}

void RenderTarget::SetColor(uint32_t color)
{
    size_t numPixels = (size_t)m_width * m_height;
    for(size_t n = 0; n < numPixels; ++n)
    {
        m_pPixels[n] = color;
    }
}

void RenderTarget::SetColor(uint8_t red, uint8_t green, uint8_t blue)
{
    SetColor((red << 16) | (green << 8) | blue);
}

void RenderTarget::DrawLine( int x1, int y1, int x2, int y2, uint32_t color)
{   
    drawLineBresenham(x1, y1, x2, y2, color);
}

void RenderTarget::DrawTri(int x1, int y1, int x2, int y2, int x3, int y3, uint32_t color)
{
    DrawLine(x1, y1, x2, y2, color);
    DrawLine(x1, y1, x3, y3, color);
    DrawLine(x2, y2, x3, y3, color);
}

void RenderTarget::FillTri(int x1, int y1, int x2, int y2, int x3, int y3, uint32_t color)
{
    if(y1 > y3)
    {
        std::swap(y1, y3);
        std::swap(x1, x3);
    }

    if(y1 > y2)
    {
        std::swap(y1, y2);
        std::swap(x1, x2);
    }

    if(y2 > y3)
    {
        std::swap(y2, y3);
        std::swap(x2, x3);
    }
    
    fillTriInternal(x1,y1,x2,y2,x3,y3,color);
}

void RenderTarget::drawLineBresenham(int x1, int y1, int x2, int y2, uint32_t color)
{
    // Make sure that we stay within the rendertarget.
    float intersectDeltaLeft;
    float intersectDeltaTop;
    
    float intersectDeltaRight;
    float intersectDeltaBot;
    
    // The line is not paralell to either the x- nor y-axes.
    if (x1 != x2 && y1 != y2)
    {
        intersectDeltaLeft = (float)-x1 / (x2 - x1);
        intersectDeltaBot = (float)-y1 / (y2 - y1);
    
        intersectDeltaRight = (float)(m_width - x1) / (x2 - x1);
        intersectDeltaTop = (float)(m_height - y1) / (y2 - y1);
    
        int xTemp1 = std::min(std::max(x1, (int)(x1 + intersectDeltaLeft * (x2 - x1))), (int)(x1 + intersectDeltaRight * (x2 - x1)));
        int xTemp2 = std::min(std::max(x2, (int)(x1 + intersectDeltaLeft * (x2 - x1))), (int)(x1 + intersectDeltaRight * (x2 - x1)));
        int yTemp1 = std::min(std::max(y1, (int)(y1 + intersectDeltaBot * (y2 - y1))), (int)(y1 + intersectDeltaTop * (y2 - y1)));
        int yTemp2 = std::min(std::max(y2, (int)(y1 + intersectDeltaBot * (y2 - y1))), (int)(y1 + intersectDeltaTop * (y2 - y1)));
        
        x1 = xTemp1;
        x2 = xTemp2;
        y1 = yTemp1;
        y2 = yTemp2;

        if (x1 == x2 || y1 == y2)
        {
            return;
        }
    }
    // The line is paralell to the y axis.
    else if (x1 != x2)
    {
        if (y1 < 0 || y1 > (int)m_height)
        {
            return;
        }

        intersectDeltaLeft =  (float)-x1 / (x2 - x1);
        intersectDeltaRight = (float)(m_width - x1) / (x2 - x1);
    
        int xTemp1 = std::min(std::max(x1, (int)(x1 + intersectDeltaLeft * (x2 - x1))), (int)(x1 + intersectDeltaRight * (x2 - x1)));
        int xTemp2 = std::min(std::max(x2, (int)(x1 + intersectDeltaLeft * (x2 - x1))), (int)(x1 + intersectDeltaRight * (x2 - x1)));
        x1 = xTemp1;
        x2 = xTemp2;

        if (x1 == x2)
        {
            return;
        }
    }
    // The line is paralell to the x axis.
    else if (y1 != y2)
    {
        if (x1 < 0 || x1 > (int)m_width)
        {
            return;
        }

        intersectDeltaBot = (float)-y1 / (y2 - y1);
        intersectDeltaTop = (float)(m_height - y1) / (y2 - y1);

        int yTemp1 = std::min(std::max(y1, (int)(y1 + intersectDeltaBot * (y2 - y1))), (int)(y1 + intersectDeltaTop * (y2 - y1)));
        int yTemp2 = std::min(std::max(y2, (int)(y1 + intersectDeltaBot * (y2 - y1))), (int)(y1 + intersectDeltaTop * (y2 - y1)));
        y1 = yTemp1;
        y2 = yTemp2;

        if (y1 == y2)
        {
            return;
        }
    }
    else
    {
        SetPixelColor(x1, y1, color);
        return;
    }


    // Perform Breshenhams algorithm!
    bool changed = false;
    int x = x1;
    int y = y1;

    int dx = abs((int)x2 - (int)x1);
    int dy = abs((int)y2 - (int)y1);

    int signX = 0;
    if (dx != 0)
    {
        signX = (int)(x2 - x1) / dx;
    }

    int signY = 0;
    if (dy != 0)
    {
        signY = (int)(y2 - y1) / dy;
    }

    if (dy > dx)
    {
        std::swap(dx, dy);
        changed = true;
    }

    int e = 2 * dy - dx;

    for (int i = 1; i <= dx; i++)
    {
        SetPixelColor(x,y,color);
        while (e >= 0)
        {
            if (changed)
            {
                x += signX;
            }
            else
            {
                y += signY;
            }
            e -= 2 * dx;
        }
        if (changed)
        {
            y += signY;
        }
        else
        {
            x += signX;
        }
        e = e + 2 * dy;
    }
}

struct LineInfo
{
    int x = 0;
    int y = 0;
    int dx = 0;
    int dy = 0;
    int signX = 0;
    int signY = 0;
    bool changed = false;
    int e = 0;
};

void SetupLineInfo(LineInfo & lineInfo, int x1, int y1, int x2, int y2)
{
    lineInfo.x = x1;
    lineInfo.y = y1;

    lineInfo.dx = abs(x2 - x1);
    lineInfo.dy = abs(y2 - y1);

    if (lineInfo.dx != 0)
    {
        lineInfo.signX = (x2 - x1) / lineInfo.dx;
    }

    if (lineInfo.dy != 0)
    {
        lineInfo.signY = (y2 - y1) / lineInfo.dy;
    }

    if (lineInfo.dy > lineInfo.dx)
    {
        std::swap(lineInfo.dy, lineInfo.dx);
        lineInfo.changed = true;
    }

    lineInfo.e = 2 * lineInfo.dy - lineInfo.dx;
}

void RenderTarget::fillTriInternal(int x1, int y1, int x2, int y2, int x3, int y3, uint32_t color)
{
    // Assume points are sorted such that y1 <= y2 <= y3

    int dx1 = x2 - x1;
    int dx2 = x3 - x1;
    int dy1 = y2 - y1;
    int dy2 = y3 - y1;

    int ax, bx;

    float axStep = 0;
    float bxStep = 0;

    if (dy1)
    {
        axStep = (float)dx1 / dy1;
    }
    if (dy2)
    {
        bxStep = (float)dx2 / dy2;
    }

    // limit y values
    unsigned int yLow = std::min(std::max(0, y1), (int)m_height);
    unsigned int yHigh = std::min(std::max(0, y2), (int)m_height);

    for (int y = yLow; y < yHigh; y++)
    {
        ax = x1 + axStep * (y - y1);
        bx = x1 + bxStep * (y - y1);

        ax = std::min(std::max(0, ax), (int)m_width);
        bx = std::min(std::max(0, bx), (int)m_width);
        if (ax > bx)
            std::swap(ax, bx);

        for (int x = ax; x < bx; x++)
        {
            SetPixelColor(x, y, color);
        }
    }

    dx1 = x1 - x3;
    dx2 = x2 - x3;
    dy1 = y1 - y3;
    dy2 = y2 - y3;

    axStep = 0;
    bxStep = 0;

    if (dy1)
    {
        axStep = (float)dx1 / dy1;
    }
    if (dy2)
    {
        bxStep = (float)dx2 / dy2;
    }

    yLow = std::min(std::max(0, y2), (int)m_height);
    yHigh = std::min(std::max(0, y3), (int)m_height);

    for (int y = yLow; y <= yHigh; y++)
    {
        ax = x3 + axStep * (y - y3);
        bx = x3 + bxStep * (y - y3);

        ax = std::min(std::max(0, ax), (int)m_width);
        bx = std::min(std::max(0, bx), (int)m_width);

        if (ax > bx)
            std::swap(ax, bx);

        for (int x = ax; x < bx; x++)
        {
            SetPixelColor(x, y, color);
        }
    }
}

void RenderTarget::textureTriInternal(int x1, int y1, int u1, int v1, 
                                      int x2, int y2, int u2, int v2, 
                                      int x3, int y3, int u3, int v3, 
                                      unsigned char* image, unsigned int imageWidth, unsigned int imageHeight)
{
    // Assume points are sorted such that y1 <= y2 <= y3

    int dx1 = x2 - x1;
    int dx2 = x3 - x1;
    int dy1 = y2 - y1;
    int dy2 = y3 - y1;
    int du1 = u2 - u1;
    int du2 = u3 - u1;
    int dv1 = u2 - u1;
    int dv2 = u3 - u1;

    int ax, bx;

    float invertedDy1 = 0;
    float invertedDy2 = 0;

    if (dy1)
    {
        invertedDy1 = 1.f / dy1;
    }
    if (dy2)
    {
        invertedDy2 = 1.f / dy2;
    }

    // limit y values
    unsigned int yLow = std::min(std::max(0, y1), (int)m_height);
    unsigned int yHigh = std::min(std::max(0, y2), (int)m_height);

    for (int y = yLow; y < yHigh; y++)
    {
        float t1 = (y - y1) * invertedDy1;
        float t2 = (y - y1) * invertedDy2;

        ax = x1 + dx1 * t1;
        bx = x1 + dx2 * t2;
        
        float startU = u1 + du1 * t1;
        float startV = v1 + dv1 * t1;
        float endU = u1 + du2 * t2;
        float endV = v1 + dv2 * t2;


        if (ax > bx)
        {
            std::swap(ax, bx);
            std::swap(startU, endU);
            std::swap(startV, endV);
        }

        int limitedAx = std::min(std::max(0, ax), (int)m_width);
        int limitedBx = std::min(std::max(0, bx), (int)m_width);
        float t_step = 1.0f / (float)(bx - ax);
        float t = (limitedAx - ax) * t_step;

        float tex_u, tex_v;
        for (int x = limitedAx; x < limitedBx; x++)
        {
            tex_u = startU + t * endU;
            tex_v = startV + t * endV;
            unsigned char color = image[(int)tex_u + (int)tex_v * imageHeight];
            SetPixelColor(x, y, color);
            t += t_step;
        }
    }

    dx1 = x1 - x3;
    dx2 = x2 - x3;
    dy1 = y1 - y3;
    dy2 = y2 - y3;

    invertedDy1 = 0;
    invertedDy2 = 0;

    if (dy1)
    {
        invertedDy1 = 1.f / dy1;
    }
    if (dy2)
    {
        invertedDy2 = 1.f / dy2;
    }

    yLow = std::min(std::max(0, y2), (int)m_height);
    yHigh = std::min(std::max(0, y3), (int)m_height);

    for (int y = yLow; y <= yHigh; y++)
    {
        float t1 = (y - y3) * invertedDy1;
        float t2 = (y - y3) * invertedDy2;

        ax = x3 + dx1 * t1;
        bx = x3 + dx2 * t2;

        ax = std::min(std::max(0, ax), (int)m_width);
        bx = std::min(std::max(0, bx), (int)m_width);

        float startU = u1 + du1 * t1;
        float startV = v1 + dv1 * t1;
        float endU = u1 + du2 * t2;
        float endV = v1 + dv2 * t2;

        if (ax > bx)
        {
            std::swap(ax, bx);
            std::swap(startU, endU);
            std::swap(startV, endV);
        }

        int limitedAx = std::min(std::max(0, ax), (int)m_width);
        int limitedBx = std::min(std::max(0, bx), (int)m_width);
        float t_step = 1.0f / (float)(bx - ax);
        float t = (limitedAx - ax) * t_step;

        float tex_u, tex_v;
        for (int x = limitedAx; x < limitedBx; x++)
        {
            tex_u = startU + t * endU;
            tex_v = startV + t * endV;
            unsigned char color = image[(int)tex_u + (int)tex_v * imageHeight];
            SetPixelColor(x, y, color);
            t += t_step;
        }
    }
}
