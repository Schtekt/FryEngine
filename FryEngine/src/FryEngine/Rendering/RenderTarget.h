#pragma once
#include <cinttypes>

class RenderTarget
{
    public:
    RenderTarget(size_t width, size_t height);
    RenderTarget(const RenderTarget& other);
    ~RenderTarget();
    RenderTarget& operator=(const RenderTarget& other) = delete;

    size_t GetWidth() const;
    size_t GetHeight() const;
    uint32_t GetColor(size_t x, size_t y) const;
    const uint32_t* GetPixels() const;

    bool SetPixelColor(size_t x, size_t y, uint32_t color);
    bool SetPixelColor(size_t x, size_t y, uint8_t red, uint8_t green, uint8_t blue);
    void SetColor(uint32_t color);
    void SetColor(uint8_t red, uint8_t green, uint8_t blue);

    private:
    size_t m_width;
    size_t m_height;
    uint32_t* m_pPixels;
};