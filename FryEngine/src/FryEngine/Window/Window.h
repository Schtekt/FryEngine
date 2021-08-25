#include <Windows.h>
#include <string>

class Window
{
    public:
    Window(const char* windowDisplayName);
    ~Window();
    bool Init();
    bool Release();
    bool ProcessMessage();
    bool SetPixelColor(size_t x, size_t y, uint32_t color);
    bool SetPixelColor(size_t x, size_t y, uint8_t red, uint8_t green, uint8_t blue);
    void SetColor(uint32_t color);
    void SetColor(uint8_t red, uint8_t green, uint8_t blue);
    void Render();
    void SetWindowName(std::string& name);
    size_t GetWidth() const;
    size_t GetHeight() const;

    private:
    std::string m_windowDisplayName;
    HWND m_windowHandle;
    void* m_pPixels;
    BITMAPINFO m_bmInfo;
    size_t m_width;
    size_t m_height;
    HDC m_hdc;
};