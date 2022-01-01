#pragma once
#include <cinttypes>

class RenderTarget
{
    public:
    RenderTarget(unsigned int width, unsigned int height);
    RenderTarget(const RenderTarget& other);
    ~RenderTarget();
    RenderTarget& operator=(const RenderTarget& other) = delete;

    unsigned int GetWidth() const;
    unsigned int GetHeight() const;
    uint32_t GetColor(unsigned int x, unsigned int y) const;
    const uint32_t* GetPixels() const;

    bool SetPixelColor(unsigned int x, unsigned int y, uint32_t color);
    bool SetPixelColor(unsigned int x, unsigned int y, uint8_t red, uint8_t green, uint8_t blue);
    void SetColor(uint32_t color);
    void SetColor(uint8_t red, uint8_t green, uint8_t blue);

    void DrawLine(int x1, int y1, int x2, int y2, uint32_t color);
    void DrawTri(int x1, int y1, int x2, int y2, int x3, int y3, uint32_t color);
    void FillTri(int x1, int y1, int x2, int y2, int x3, int y3, uint32_t color);

    void FillTexturedTri(int x1, int y1, float u1, float v1,
                         int x2, int y2, float u2, float v2,
                         int x3, int y3, float u3, float v3,
                         unsigned char* image, unsigned int imageWidth, unsigned int imageHeight);

    private:
    void drawLineBresenham(int x1, int y1, int x2, int y2, uint32_t color);
    void fillTriInternal(int x1, int y1, int x2, int y2, int x3, int y3, uint32_t color);
    void textureTriInternal(int x1, int y1, float u1, float v1,
                            int x2, int y2, float u2, float v2,
                            int x3, int y3, float u3, float v3,
                            unsigned char* image, unsigned int imageWidth, unsigned int imageHeight);
    void TexturedTriangle(int x1, int y1, float u1, float v1, //float w1,
        int x2, int y2, float u2, float v2, //float w2,
        int x3, int y3, float u3, float v3, //float w3,
        unsigned char* image, unsigned int imageWidth, unsigned int imageHeight);
    void Comp(int x1, int y1, float u1, float v1,
        int x2, int y2, float u2, float v2,
        int x3, int y3, float u3, float v3);

    private:
    unsigned int m_width;
    unsigned int m_height;
    uint32_t* m_pPixels;
};