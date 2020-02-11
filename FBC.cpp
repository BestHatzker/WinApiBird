#include "stdafx.h"
#include "resource.h"
#include "cmath"
#include <vector>
#include <thread>
#include <string>
#include <time.h>
#include <iostream>
#include <map>

#include "Block.h"
#include "Bird.h"
#include "Timer.h"
#include "CommonGlobal.h"



Bird* Bird::m_pSingleton = nullptr;
Blocks* Blocks::m_pSingleton = nullptr;

DWORD g_mainWindowThreadId{};
HWND g_mainHwnd{};
HWND hButtonS, hButtonH, hButtonE;


HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK MainWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);


BOOL Line(HDC hdc, int x1, int y1, int x2, int y2)
{
	MoveToEx(hdc, x1, y1, NULL); //сделать текущими координаты x1, y1
	return LineTo(hdc, x2, y2);
}

void paintBlocks(HDC hdc) {
	for (auto curBlock : gBlocks()->getBlocks())
		Rectangle(hdc, curBlock.getX(), curBlock.getY(), curBlock.getX()+curBlock.getXsize(), curBlock.getY() + curBlock.getYsize());
}

void paintBird(HDC hdc) {
	Ellipse(hdc, gBird()->getX(), gBird()->getY(), gBird()->getX() + gBird()->getSize(), gBird()->getY() + gBird()->getSize());
	Line(hdc, gBird()->getX() + gBird()->getSize() / 2, gBird()->getY() + gBird()->getSize() / 2,
			  gBird()->getX() + gBird()->getSize() / 2 + gBird()->getSize() / 2 * sin(gBird()->getAngle()*PI / 180),
			  gBird()->getY() + gBird()->getSize() / 2 + gBird()->getSize() / 2 * cos(gBird()->getAngle()*PI / 180));
}


void newGame() {
	if (!firstStart) {
		gBird()->DelInstance();
		gBlocks()->clearBlocks();
	}
	firstStart = false;

	higthscore = 0;

	gBird()->GetInstance();
	gBird()->initBird( fieldSizeY / 10);

	gBlocks()->GetInstance();
	gBlocks()->generateBlocks();

	startingGame = false;
}


void mainGame(HWND hwnd) {
	if (startingGame)
		newGame();

	if (!gBird()->checkAlive())// проверка пересечения со всеми объектами vector Blocks
		gBird()->killBird();

	gBird()->fly();

	gBlocks()->moveBlocks();
	::PostThreadMessage(g_mainWindowThreadId, FBC_UPDATEMODEL, 0, 0);
}


void menuForm(HWND hwnd) {

	MSG msg;
	static HWND hButton;

	hButtonS = CreateWindow(L"button", L"Играть",
		WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
		fieldSizeX/3, fieldSizeY/3,
		fieldSizeX/10, fieldSizeY/10,
		hwnd, (HMENU)BUTTON_START,
		hInst, NULL);

	hButtonH = CreateWindow(L"button", L"Рекорды",
		WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
		fieldSizeX / 3 + fieldSizeX / 10, fieldSizeY / 3 + fieldSizeY / 10,
		fieldSizeX / 10, fieldSizeY / 10,
		hwnd, (HMENU)BUTTON_HIGH,
		hInst, NULL);

	hButtonE = CreateWindow(L"button", L"Выход",
		WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
		fieldSizeX / 3 + 2 * fieldSizeX / 10, fieldSizeY / 3 + 2 * fieldSizeY / 10,
		fieldSizeX / 10, fieldSizeY / 10,
		hwnd, (HMENU)BUTTON_EXIT,
		hInst, NULL);
}



int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

	g_mainWindowThreadId = ::GetCurrentThreadId();

    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_FBC, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    if (!InitInstance (hInstance, nCmdShow))
        return FALSE;

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_FBC));
    MSG msg;

    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
			if (msg.message == FBC_UPDATEMODEL)
				msg.hwnd = g_mainHwnd;

            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}


ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = MainWndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_FBC));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_FBC);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}


BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   g_mainHwnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!g_mainHwnd)
      return FALSE;
   return TRUE;
}


LRESULT CALLBACK MainWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;          // handle to device context (DC) 
	hdc = GetWindowDC(GetDesktopWindow());
	PAINTSTRUCT ps;   // paint data for Begin/EndPaint  
	POINT ptClientUL; // client area upper left corner  
	POINT ptClientLR; // client area lower right corner  
	static HDC hdcCompat; // handle to DC for bitmap  
	static POINT pt;      // x- and y-coordinates of cursor  

	static Timer t;

	srand(time(NULL));

	switch (uMsg)
	{

	case WM_COMMAND:

		if (LOWORD(wParam) == BUTTON_START) {

			DestroyWindow(hButtonS);
			DestroyWindow(hButtonH);
			DestroyWindow(hButtonE);

			t.setInterval([&] {
				mainGame(hwnd);
			}, 60);
		}

		if (LOWORD(wParam) == BUTTON_HIGH) {

			RECT rc;
			GetClientRect(hwnd, &rc);

			SelectObject(hdc, GetStockObject(DEFAULT_GUI_FONT));
			SetBkMode(hdc, TRANSPARENT);
			SetTextColor(hdc, RGB(255, 255, 255));

			std::map <int, std::string> rec;

			std::string line;
			std::ifstream in("higthscore"); // окрываем файл для чтения
			if (in.is_open())
			{
				int tmpHigthscore;
				std::string tmpStr;
				std::string::size_type sz;
				while (getline(in, line))
				//while (in >> tmpHigthscore >> line)
				{ 
					tmpStr = line.substr(line.find("-") + 2);
					tmpHigthscore = std::stoi(line.substr(0, line.find("-")));
					rec.insert(std::pair<int, std::string>(tmpHigthscore, tmpStr));
				}
			}
			
			int i = 1;
			for (auto itr = rec.begin(); itr != rec.end() && i < 10; itr++) {
				std::wstring wstr(itr->second.begin(), itr->second.end());
				std::wstring wh(std::to_wstring(itr->first));
				rc.left = fieldSizeX * 2 / 3;
				rc.top  = fieldSizeY * 1 / 3 + i * fieldSizeY / 10;
				DrawText(hdc, wh.c_str(), wstr.length(), &rc, 0);
				rc.left = fieldSizeX * 2 / 3 + fieldSizeX * 1 / 10;
				DrawText(hdc, wstr.c_str(), wstr.length(), &rc, 0);
				i++;
			}

		}

		if (LOWORD(wParam) == BUTTON_EXIT) {

			exit(0);
		}
		break;		

	case FBC_UPDATEMODEL:
		InvalidateRect(hwnd, NULL, TRUE);
		break;

	case WM_CREATE:

		WINDOWPLACEMENT wp;
		GetWindowPlacement(hwnd, &wp);
		if (wp.showCmd != SW_MAXIMIZE)
		{
			SetWindowLong(hwnd, GWL_STYLE, GetWindowLong(hwnd, GWL_STYLE) & (~WS_CAPTION));
			ShowWindow(hwnd, SW_MAXIMIZE);
		}
		UpdateWindow(hwnd);

		hdc = GetWindowDC(GetDesktopWindow());
		fieldSizeX = GetDeviceCaps(hdc, HORZRES); //взять разрешение по горизонтали
		fieldSizeY = GetDeviceCaps(hdc, VERTRES); //взять разрешение по вертикали 

		blockMin = fieldSizeX / 20;

		menuForm(hwnd);

		return 0;

	case WM_PAINT:

		BeginPaint(hwnd, &ps);

		RECT rect;
		GetClientRect(hwnd, &rect);
		FillRect(ps.hdc, &rect, (HBRUSH)(COLOR_WINDOW + 1));

		SelectObject(ps.hdc, GetStockObject(GRAY_BRUSH));

		paintBlocks(ps.hdc);
		paintBird(ps.hdc);

		EndPaint(hwnd, &ps);

		break;

	case WM_DESTROY:

		t.stop();

		DeleteDC(hdcCompat);
		PostQuitMessage(0);
		break;
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
			exit(0);
		else if (wParam == VK_SPACE)
			gBird()->flyUp();
		break;

	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
	return (LRESULT)NULL;
}