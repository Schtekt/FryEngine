#include "RenderTarget.h"
#include <memory.h>
#include <math.h>
#include <algorithm>

RenderTarget::RenderTarget( unsigned int width, unsigned int height) : m_width(width), m_height(height)
{
    m_pPixels = new uint32_t[m_width * m_height];
}

RenderTarget::RenderTarget(const RenderTarget& other) : m_width(other.m_width), m_height(other.m_height)
{
    m_pPixels = new uint32_t[m_width * m_height];
    memcpy(m_pPixels, other.m_pPixels, m_width * m_height * sizeof(uint32_t));
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
    size_t numPixels = m_width * m_height;
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
    // To be changed!
    x1 = std::max(0, std::min(x1, (int)m_width - 1));
    x2 = std::max(0, std::min(x2, (int)m_width - 1));
    y1 = std::max(0, std::min(y1, (int)m_height - 1));
    y2 = std::max(0, std::min(y2, (int)m_height - 1));
    
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
    // To be changed!
    x1 = std::max(0, std::min(x1, (int)m_width - 1));
    x2 = std::max(0, std::min(x2, (int)m_width - 1));
    x3 = std::max(0, std::min(x3, (int)m_width - 1));

    y1 = std::max(0, std::min(y1, (int)m_height - 1));
    y2 = std::max(0, std::min(y2, (int)m_height - 1));
    y3 = std::max(0, std::min(y3, (int)m_height - 1));

    // Sort the vertices in ascending order
    if (y2 > y1)
    {
        std::swap(y2, y1);
        std::swap(x2, x1);
        
    }

    if (y3 > y1)
    {
        std::swap(y3, y1);
        std::swap(x3, x1);
    }

    if (y3 > y2)
    {
        std::swap(y3, y2);
        std::swap(x3, x2);
    }

    // calculate fourth vertice
    int x4 = x1 + ((y2 - y1)/(y3 - y1))*(x3 - x1);
    int y4 = y2;

    fillTriInternal(x1, y1, x2, y2, x4, y4, color);
    fillTriInternal(x3, y3, x2, y2, x4, y4, color);
}

void RenderTarget::drawLineBresenham(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2, uint32_t color)
{
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

    float e = 2 * dy - dx;

    for (int i = 1; i <= dx; i++)
    {
        m_pPixels[y * m_width + x] = color;
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
    int x;
    int y;
    int dx;
    int dy;
    int signX = 0;
    int signY = 0;
    bool changed = false;
    float e;
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
    LineInfo firstLine;
    LineInfo secondLine;

    SetupLineInfo(firstLine, x1, y1, x2, y2);
    SetupLineInfo(secondLine, x1, y1, x3, y3);

    int i = 1;
    int j = 1;

    while (i <= firstLine.dx && j <= secondLine.dx)
    {
        int firstLineY = firstLine.y;
        while (firstLineY == firstLine.y)
        {
            while (firstLine.e >= 0 && firstLineY == firstLine.y)
            {
                if (firstLine.changed)
                {
                    firstLine.x += firstLine.signX;
                }
                else
                {
                    firstLine.y += firstLine.signY;
                }
                firstLine.e -= 2 * firstLine.dx;
            }

            if (firstLine.changed)
            {
                firstLine.y += firstLine.signY;
            }
            else
            {
                firstLine.x += firstLine.signX;
            }
            firstLine.e += 2 * firstLine.dy;
            ++i;
        }

        int secondLineY = secondLine.y;
        while (secondLineY == secondLine.y)
        {
            while (secondLine.e >= 0 && secondLineY == secondLine.y)
            {
                if (secondLine.changed)
                {
                    secondLine.x += secondLine.signX;
                }
                else
                {
                    secondLine.y += secondLine.signY;
                }
                secondLine.e -= 2 * secondLine.dx;
            }

            if (secondLine.changed)
            {
                secondLine.y += secondLine.signY;
            }
            else
            {
                secondLine.x += secondLine.signX;
            }
            secondLine.e += 2 * secondLine.dy;
            ++j;
        }

        DrawLine(firstLine.x, firstLine.y, secondLine.x, secondLine.y, color);
    }
}
