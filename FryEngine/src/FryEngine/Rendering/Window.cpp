#include "Window.h"
#include <iostream>
Window::Window(const char* windowDisplayName, size_t width, size_t height): m_windowDisplayName(windowDisplayName), m_width(width + 20), m_height(height + 43)
{
}

Window::~Window()
{
    Release();
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg , WPARAM wparam, LPARAM lparam)
{
    switch (msg)
    {
        case WM_CREATE:
        break;
        case WM_CLOSE:
            return DestroyWindow(hwnd);
        break;
        case WM_DESTROY:
            PostQuitMessage(0);
        break;
        default:
            return DefWindowProc(hwnd, msg, wparam, lparam);
        break;
    }

    return TRUE;
}

bool Window::Init()
{
    WNDCLASSEX windowClass;

    windowClass.cbClsExtra      = NULL;
    windowClass.cbSize          = sizeof(WNDCLASSEX);
    windowClass.cbWndExtra      = NULL;
    windowClass.hbrBackground   = (HBRUSH)COLOR_WINDOW;
    windowClass.hCursor         = LoadCursor(NULL, IDC_ARROW);
    windowClass.hIcon           = LoadIcon(NULL, IDI_APPLICATION);
    windowClass.hIconSm         = LoadIcon(NULL, IDI_APPLICATION);
    windowClass.hInstance       = NULL;
    windowClass.lpszClassName   = L"FryWindowClass";
    windowClass.lpszMenuName    = L"";
    windowClass.style           = NULL;
    windowClass.lpfnWndProc     = &WndProc;

    if(!RegisterClassEx(&windowClass))
    {
        return false;
    }

    m_windowHandle = CreateWindowExA(WS_EX_OVERLAPPEDWINDOW, "FryWindowClass", m_windowDisplayName.c_str(), WS_SYSMENU , 
    CW_USEDEFAULT, CW_USEDEFAULT, (int)m_width, (int)m_height, 
    NULL, NULL, NULL, NULL);

    if(!m_windowHandle)
    {
        return false;
    }

    ShowWindow(m_windowHandle, SW_SHOW);
    UpdateWindow(m_windowHandle);

    RECT rect;
    GetClientRect(m_windowHandle, &rect);
    m_width = rect.right - rect.left;
    m_height = rect.bottom - rect.top;

    m_bmInfo.bmiHeader.biSize = sizeof(m_bmInfo.bmiHeader);
    m_bmInfo.bmiHeader.biWidth = (LONG)m_width;
    m_bmInfo.bmiHeader.biHeight = (LONG)m_height;
    m_bmInfo.bmiHeader.biPlanes = 1;
    m_bmInfo.bmiHeader.biBitCount = 32;
    m_bmInfo.bmiHeader.biCompression = BI_RGB;
    m_hdc = GetDC(m_windowHandle);
    return true;
}

bool Window::Release()
{
    if(!DestroyWindow(m_windowHandle))
    {
        return false;
    }
    return true;
}

bool Window::ProcessMessage()
{
    MSG msg;

    while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    
    if(!IsWindow(m_windowHandle))
    {
        return false;
    }
    return true;
}

void Window::Render(const RenderTarget& target)
{

    StretchDIBits(m_hdc,
        0, 0,
        (int)target.GetWidth(),
        (int)target.GetHeight(),
        0, 0,
        (int)m_width,
        (int)m_height,
        target.GetPixels(),
        &m_bmInfo,
        DIB_RGB_COLORS,
        SRCCOPY
    );
}

void Window::SetWindowName(std::string& name)
{
    m_windowDisplayName = name;
    SetWindowTextA(m_windowHandle, m_windowDisplayName.c_str());
}

size_t Window::GetWidth() const
{
    return m_width;
}

size_t Window::GetHeight() const
{
    return m_height;
}