#pragma once
#include <Windows.h>
#include <string>
#include "RenderTarget.h"

class Window
{
    public:
    Window(const char* windowDisplayName, size_t width, size_t height);
    ~Window();
    bool Init();
    bool Release();
    bool ProcessMessage();
    void Render(const RenderTarget& target);
    void SetWindowName(std::string& name);
    size_t GetWidth() const;
    size_t GetHeight() const;

    private:
    std::string m_windowDisplayName;
    HWND m_windowHandle;
    BITMAPINFO m_bmInfo;
    size_t m_width;
    size_t m_height;
    HDC m_hdc;
};