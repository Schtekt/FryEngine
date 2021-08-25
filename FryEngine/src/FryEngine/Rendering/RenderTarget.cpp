#include "RenderTarget.h"
#include <memory.h>
RenderTarget::RenderTarget(size_t width, size_t height) : m_width(width), m_height(height)
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

size_t RenderTarget::GetWidth() const
{
    return m_width;
}

size_t RenderTarget::GetHeight() const
{
    return m_height;
}

uint32_t RenderTarget::GetColor(size_t x, size_t y) const
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

bool RenderTarget::SetPixelColor(size_t x, size_t y, uint32_t color)
{
    if(x < m_width && y < m_height)
    {
        m_pPixels[y*m_width + x] = color;
        return true;
    }
    return false;
}

bool RenderTarget::SetPixelColor(size_t x, size_t y, uint8_t red, uint8_t green, uint8_t blue)
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