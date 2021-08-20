#include <Windows.h>
#include <string>

class Window
{
    public:
    Window(const wchar_t* windowDisplayName);
    ~Window();
    bool Init();
    bool Release();
    bool ProcessMessage();

    private:
    std::wstring m_windowDisplayName;
    HWND m_windowHandle;
    void* m_pPixels;
    BITMAPINFO m_bmInfo;
    size_t m_width;
    size_t m_height;
};