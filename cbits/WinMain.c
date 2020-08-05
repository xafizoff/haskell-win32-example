#include <windows.h>
#include <strsafe.h>
#include "WinMain.h"

/* Global instance handle */
HINSTANCE g_hInstance = NULL;
HWND g_hWnd = NULL;

static LPCWSTR MainWndClass = L"Test app";

void ShowContextMenu(HWND hWnd) {
    HMENU hMenu = CreatePopupMenu();
    SetForegroundWindow(hWnd);
    UINT uFlags = TPM_RIGHTBUTTON;
    POINT pt; 
    GetCursorPos(&pt);
    AppendMenuW(hMenu, MF_STRING, IDM_EXIT, L"&Exit");
    TrackPopupMenuEx(hMenu, uFlags, pt.x, pt.y, hWnd, NULL);
    DestroyMenu(hMenu);
}

LRESULT CALLBACK MainWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
        case WM_COMMAND:
        {
            switch (LOWORD(wParam))
            {
                case IDM_EXIT:
                {
                    DestroyWindow(hWnd);
                    return 0;
                }
            }
            break;
        }

        case WM_CONTEXTMENU:
        {
            ShowContextMenu(hWnd);
            break;
        }

        case WM_DESTROY:
        {
            PostQuitMessage(0);
            return 0;
        }
    }

    return DefWindowProc(hWnd, msg, wParam, lParam);
}

BOOL RegisterMainWindowClass()
{
    WNDCLASSEXW wc;
    wc.cbSize        = sizeof(wc);
    wc.lpszClassName = MainWndClass;
    wc.style         = 0;
    wc.lpfnWndProc   = &MainWndProc;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hInstance     = g_hInstance;
    wc.hIcon         = (HICON)LoadImage(NULL, IDI_WINLOGO, IMAGE_ICON, 0, 0, LR_SHARED);
    wc.hCursor       = (HCURSOR)LoadImage(NULL, IDC_ARROW, IMAGE_CURSOR, 0, 0, LR_SHARED);
    wc.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
    wc.lpszMenuName  = NULL;
    wc.hIconSm       = (HICON)LoadImage(NULL, IDI_WINLOGO, IMAGE_ICON, 0, 0, LR_SHARED);
    return (RegisterClassExW(&wc)) ? TRUE : FALSE;
}

HWND CreateMainWindow()
{
    HWND hWnd = GetConsoleWindow();
    ShowWindow( hWnd, SW_HIDE );
    return CreateWindowExW(0, MainWndClass, MainWndClass, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 640, 480,
                            NULL, NULL, g_hInstance, NULL);
}

int WINAPI GuiInit(HINSTANCE hInstance)
{
    HWND hWnd;
    MSG msg;

    g_hInstance = hInstance;

    if (!RegisterMainWindowClass())
    {
        DWORD err = GetLastError();
        printf("error: %d\n", err);
        MessageBox(NULL, TEXT("Error registering main window class."), TEXT("Error"), MB_ICONERROR | MB_OK);
        return 0;
    }

    if (!(hWnd = CreateMainWindow()))
    {
        DWORD err = GetLastError();
        printf("error: %d\n", err);
        MessageBox(NULL, TEXT("Error creating main window."), TEXT("Error"), MB_ICONERROR | MB_OK);
        return 0;
    }

    g_hWnd = hWnd;

    ShowWindow(hWnd, SW_SHOWNORMAL);

    while (GetMessage(&msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}
