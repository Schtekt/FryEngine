#include "Window.h"
#include <iostream>
Window::Window(const char* windowDisplayName): m_windowDisplayName(windowDisplayName)
{
}

Window::~Window()
{
    delete m_pPixels;
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
        //case WM_PAINT:
        //    PAINTSTRUCT ps;
        //    HDC hdc;
        //    hdc = BeginPaint(hwnd, &ps);

        //    FillRect(hdc, &ps.rcPaint, (HBRUSH)(0));
        //    EndPaint(hwnd, &ps);
        //    std::cout << "PAINTING" << std::endl;
        //break;
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

    m_windowHandle = CreateWindowExA(WS_EX_OVERLAPPEDWINDOW, "FryWindowClass", m_windowDisplayName.c_str(), WS_OVERLAPPEDWINDOW, 
    CW_USEDEFAULT, CW_USEDEFAULT, 1080, 720, 
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

    m_pPixels = new uint32_t[m_width*m_height];

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

bool Window::SetPixelColor(size_t x, size_t y, uint32_t color)
{
    if(x < m_width && y < m_height)
    {
        ((uint32_t*)m_pPixels)[y*m_width + x] = color;

        return true;
    }
    return false;
}

bool Window::SetPixelColor(size_t x, size_t y, uint8_t red, uint8_t green, uint8_t blue)
{
    return SetPixelColor(x, y, (red << 16) | (green << 8) | blue );
}

void Window::SetColor(uint32_t color)
{
    size_t numPixels = m_width * m_height;
    for(size_t n = 0; n < numPixels; n++)
    {
        ((uint32_t*)m_pPixels)[n] = color;
    }
}

void Window::SetColor(uint8_t red, uint8_t green, uint8_t blue)
{
    SetColor((red << 16) | (green << 8) | blue);
}

void Window::Render()
{
    StretchDIBits(m_hdc,
        0, 0,
        (int)m_width,
        (int)m_height,
        0, 0,
        (int)m_width,
        (int)m_height,
        m_pPixels,
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