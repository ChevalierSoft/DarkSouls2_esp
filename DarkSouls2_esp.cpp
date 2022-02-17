// DarkSouls2_esp.cpp : Defines the entry point for the application.
// github.com/ChevalierSoft

#include "framework.h"
#include "DarkSouls2_esp.h"
#include "Paint.h"
#include <windows.h>
#include <objidl.h>
#include <gdiplus.h>
#include <string>
#include <iostream>
#include "GameData.h"

// Global Variables:
HINSTANCE   hInst;                                  // current instance
WCHAR       szTitle[32] = L"I see you";                 // The title bar text
//LPCSTR      targetTitle = "DarkSoulsII.exe";
LPCSTR      targetTitle = "DARK SOULS II";
int         width, height;
HWND        overlayHWND, targetHWND;
Paint       paint;
int         cpt = 0;

GameData	g_data;

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    Gdiplus::GdiplusStartupInput    gdiplusStartupInput;
    ULONG_PTR                       gdiplusToken;

    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    MyRegisterClass(hInstance);

    targetHWND = FindWindowA(0, targetTitle);
    if (targetHWND)
    {
        RECT rect;
        GetWindowRect(targetHWND, &rect);
        width = rect.right - rect.left;
        height = rect.bottom - rect.top;

        //width = 800;
        //height = 599;
    }
    else
        return false;

    // Perform application initialization:
    if (!InitInstance(hInstance, nCmdShow))
        return FALSE;

    paint = Paint(overlayHWND, targetHWND, width, height);

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        RECT rect;
        TranslateMessage(&msg);
        DispatchMessage(&msg);
        GetWindowRect(targetHWND, &rect);
        width = rect.right - rect.left;
        height = rect.bottom - rect.top;

        //std::cout << width << std::endl;
        //std::cout << height << std::endl;

        MoveWindow(overlayHWND, rect.left, rect.top, width, height, true);
    }

    return (int)msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DARKSOULS2ESP));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = CreateSolidBrush(RGB(0, 0, 0));
    wcex.lpszMenuName = szTitle;
    wcex.lpszClassName = szTitle;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance; // Store instance handle in our global variable

    overlayHWND = CreateWindowExW(WS_EX_TOPMOST | WS_EX_TRANSPARENT | WS_EX_LAYERED, szTitle, szTitle, WS_POPUP,
        1, 1, width, height, nullptr, nullptr, hInstance, nullptr);

    if (!overlayHWND)
        return FALSE;

    SetLayeredWindowAttributes(overlayHWND, RGB(0, 0, 0), 0, LWA_COLORKEY);

    ShowWindow(overlayHWND, nCmdShow);

    AllocConsole();
    FILE* fp;
    freopen_s(&fp, "CONOUT$", "w", stdout);

    g_data.init();

    srand(time(NULL));

    return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HDC          hdc;
    PAINTSTRUCT  ps;

    switch (message)
    {
        case WM_PAINT:
        {
            paint.render(g_data);
            break;
        }
        case WM_KEYDOWN:
        {
            if (lParam == VK_TAB)
                g_data.visible = !g_data.visible;
            break;
        }
        case WM_DESTROY:
        {
            PostQuitMessage(0);
            FreeConsole();
            delete g_data.memory;
            g_data.memory = NULL;
            break;
        }
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
