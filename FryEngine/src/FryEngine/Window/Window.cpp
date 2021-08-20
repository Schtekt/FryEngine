#include "Window.h"
#include <iostream>
Window::Window(const wchar_t* windowDisplayName): m_windowDisplayName(windowDisplayName)
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
        case WM_PAINT:
            PAINTSTRUCT ps;
            HDC hdc;
            hdc = BeginPaint(hwnd, &ps);

            FillRect(hdc, &ps.rcPaint, (HBRUSH)(0));
            EndPaint(hwnd, &ps);
            std::cout << "PAINTING" << std::endl;
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

    m_windowHandle = CreateWindowEx(WS_EX_OVERLAPPEDWINDOW, L"FryWindowClass", m_windowDisplayName.c_str(), WS_OVERLAPPEDWINDOW, 
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
    m_bmInfo.bmiHeader.biWidth = m_width;
    m_bmInfo.bmiHeader.biHeight = m_height;
    m_bmInfo.bmiHeader.biPlanes = 1;
    m_bmInfo.bmiHeader.biBitCount = 32;
    m_bmInfo.bmiHeader.biCompression = BI_RGB;

    HDC hdc = GetDC(m_windowHandle);

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