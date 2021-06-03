// GdiPlusLab.cpp : Defines the entry point for the application.
//

#include "pch.h"
#include "framework.h"
#include "GdiPlusLab.h"
#include <Gdiplus.h>
#include <string>
using namespace std;

using namespace Gdiplus;

#define MAX_LOADSTRING 100

HRGN textRegion;

static wchar_t textArray[1024] = L"Text";
static wchar_t fontArray[1024] = L"Arial";
static int fontSize = 48;
static int borderRed = 128, borderGreen = 128, borderBlue = 255;
static int innerRed = 255, innerGreen = 255, innerBlue = 255;
static int backgroundRed = 255, backgroundGreen = 255, backgroundBlue = 0;
static int posX = 0, posY = 0;


// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);

INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    Choose(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.
    GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, nullptr);

    // Initialize global strings
    //LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_GDIPLUSLAB, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_GDIPLUSLAB));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    GdiplusShutdown(gdiplusToken);

    return (int) msg.wParam;
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

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_GDIPLUSLAB));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = nullptr;
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

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

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, NULL,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

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
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            case IDM_CHOOSE:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, Choose);
                InvalidateRect(hWnd, NULL, true);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_LBUTTONDOWN:
    {
        if (PtInRegion(textRegion, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)) != 0) {
            DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, Choose);
        }
        // redrawing window
        InvalidateRect(hWnd, NULL, TRUE);
    }
    break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            RECT rClient;

            GetClientRect(hWnd, &rClient);
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code that uses hdc here...
            {
                Graphics graphics(hdc);
                graphics.SetSmoothingMode(SmoothingModeHighQuality);
                graphics.SetInterpolationMode(InterpolationModeHighQualityBicubic);

                SolidBrush brushYellow(Color(backgroundRed, backgroundGreen, backgroundBlue));
                graphics.FillRectangle(&brushYellow, 0, 0, (int)rClient.right, (int)rClient.bottom);

                FontFamily fontFamily(fontArray);
                StringFormat strformat;
                
                GraphicsPath path;
                path.AddString(textArray, wcslen(textArray), &fontFamily,
                    FontStyleRegular, fontSize, Gdiplus::Point(posX, posY), &strformat);
                Pen pen(Color(borderRed, borderGreen, borderBlue), 3);
                pen.SetLineJoin(LineJoinRound);
                graphics.DrawPath(&pen, &path);
                SolidBrush brushWhite(Color(innerRed, innerGreen, innerBlue));
                graphics.FillPath(&brushWhite, &path);

                Region myRegion(&path);
                textRegion = myRegion.GetHRGN(&graphics);
            }
            EndPaint(hWnd, &ps);

            //SetWindowRgn(hWnd, textRegion, TRUE);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;

    }
    return (INT_PTR)FALSE;
}

INT_PTR CALLBACK Choose(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_COMMAND:
    {
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            BOOL bSuccess;
            TCHAR textArrayTemp[1024] = L"";
            TCHAR fontArrayTemp[1024] = L"";
            GetDlgItemText(hwnd, IDC_TEXT, textArrayTemp, 1000);
            GetDlgItemText(hwnd, IDC_FONT, fontArrayTemp, 1000);
            int fontSizeTemp = GetDlgItemInt(hwnd, IDC_FONTSIZE, &bSuccess, FALSE);
            int borderRedTemp = GetDlgItemInt(hwnd, IDC_BORDERR, &bSuccess, FALSE), borderGreenTemp = GetDlgItemInt(hwnd, IDC_BORDERG, &bSuccess, FALSE), borderBlueTemp = GetDlgItemInt(hwnd, IDC_BORDERB, &bSuccess, FALSE);
            int innerRedTemp = GetDlgItemInt(hwnd, IDC_INNERR, &bSuccess, FALSE), innerGreenTemp = GetDlgItemInt(hwnd, IDC_INNERG, &bSuccess, FALSE), innerBlueTemp = GetDlgItemInt(hwnd, IDC_INNERB, &bSuccess, FALSE);
            int backgroundRedTemp = GetDlgItemInt(hwnd, IDC_BGR, &bSuccess, FALSE), backgroundGreenTemp = GetDlgItemInt(hwnd, IDC_BGG, &bSuccess, FALSE), backgroundBlueTemp = GetDlgItemInt(hwnd, IDC_BGB, &bSuccess, FALSE);
            int posXTemp = GetDlgItemInt(hwnd, IDC_POSX, &bSuccess, FALSE), posYTemp = GetDlgItemInt(hwnd, IDC_POSY, &bSuccess, FALSE);


            wcscpy_s(textArray, textArrayTemp);
            wcscpy_s(fontArray, fontArrayTemp);


            if (fontSizeTemp > 0) {
                fontSize = fontSizeTemp;
            }
            if (borderRedTemp >= 0 && borderRedTemp <= 255) {
                borderRed = borderRedTemp;
            }
            if (borderBlueTemp >= 0 && borderBlueTemp <= 255) {
                borderBlue = borderBlueTemp;
            }
            if (borderGreenTemp >= 0 && borderGreenTemp <= 255) {
                borderGreen = borderGreenTemp;
            }
            if (innerRedTemp >= 0 && innerRedTemp <= 255) {
                innerRed = innerRedTemp;
            }
            if (innerBlueTemp >= 0 && innerBlueTemp <= 255) {
                innerBlue = innerBlueTemp;
            }
            if (innerGreenTemp >= 0 && innerGreenTemp <= 255) {
                innerGreen = innerGreenTemp;
            }
            if (backgroundRedTemp >= 0 && backgroundRedTemp <= 255) {
                backgroundRed = backgroundRedTemp;
            }
            if (backgroundBlueTemp >= 0 && backgroundBlueTemp <= 255) {
                backgroundBlue = backgroundBlueTemp;
            }
            if (backgroundGreenTemp >= 0 && backgroundGreenTemp <= 255) {
                backgroundGreen = backgroundGreenTemp;
            }
            if (posXTemp >= -1) {
                posX = posXTemp;
            }
            if (posYTemp >= -1) {
                posY = posYTemp;
            }

            EndDialog(hwnd, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
    }
    break;
    }

    return (INT_PTR)FALSE;
}