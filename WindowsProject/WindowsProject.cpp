// WindowsProject.cpp : Определяет точку входа для приложения.
//

#include "framework.h"
#include "WindowsProject.h"

#include "commdlg.h"
#include "windows.h"

#define MAX_LOADSTRING 100

// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна

// Отправить объявления функций, включенных в этот модуль кода:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: Разместите код здесь.

	// Инициализация глобальных строк
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_WINDOWSPROJECT, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Выполнить инициализацию приложения:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINDOWSPROJECT));

	MSG msg;

	// Цикл основного сообщения:
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}



//
//  ФУНКЦИЯ: MyRegisterClass()
//
//  ЦЕЛЬ: Регистрирует класс окна.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINDOWSPROJECT));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_WINDOWSPROJECT);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

//
//   ФУНКЦИЯ: InitInstance(HINSTANCE, int)
//
//   ЦЕЛЬ: Сохраняет маркер экземпляра и создает главное окно
//
//   КОММЕНТАРИИ:
//
//        В этой функции маркер экземпляра сохраняется в глобальной переменной, а также
//        создается и выводится главное окно программы.
//

#define IDC_CALENDAR 100
#define IDC_EDIT 101
#define IDC_LISTBOX 102
#define IDC_SPIN 103

#define WC_MONTHCAL L"SysMonthCal32"
#define WC_EDITCAL L"Edit"
#define WC_LISTBOXCAL L"ListBox"
#define WC_UPDOWNCAL L"msctls_updown32"

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // Сохранить маркер экземпляра в глобальной переменной

	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}

	HWND hWndCalendar = CreateWindowEx(0, WC_MONTHCAL, L"",
		WS_CHILD | WS_VISIBLE,
		20, 0, 160, 200,
		hWnd, (HMENU)IDC_CALENDAR,
		GetModuleHandle(NULL), NULL);

	if (!hWndCalendar)
	{
		return FALSE;
	}

	HWND hWndEdit = CreateWindowEx(0, WC_EDITCAL, L"",
		WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
		20, 200, 150, 30,
		hWnd, (HMENU)IDC_EDIT,
		GetModuleHandle(NULL), NULL);

	if (!hWndEdit)
	{
		return FALSE;
	}

	HWND hWndListbox = CreateWindowEx(0, WC_LISTBOXCAL, L"",
		WS_CHILD | WS_VISIBLE | WS_BORDER | LBS_COMBOBOX,
		20, 250, 150, 50,
		hWnd, (HMENU)IDC_LISTBOX,
		GetModuleHandle(NULL), NULL);

	if (!hWndListbox)
	{
		return FALSE;
	}

	SendMessage(hWndListbox, LB_ADDSTRING, 0, (LPARAM)L"Первый элемент");
	SendMessage(hWndListbox, LB_ADDSTRING, 0, (LPARAM)L"Второй элемент");

	HWND hWndSpin = CreateWindowEx(0, WC_UPDOWNCAL, L"",
		WS_CHILD | WS_VISIBLE | WS_BORDER,
		20, 320, 150, 50,
		hWnd, (HMENU)IDC_SPIN,
		GetModuleHandle(NULL), NULL);

	if (!hWndSpin)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

//
//  ФУНКЦИЯ: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ЦЕЛЬ: Обрабатывает сообщения в главном окне.
//
//  WM_COMMAND  - обработать меню приложения
//  WM_PAINT    - Отрисовка главного окна
//  WM_DESTROY  - отправить сообщение о выходе и вернуться
//
//

#define MAX_PATH 260
wchar_t szFileName[MAX_PATH];
wchar_t szFindWhat[MAX_PATH];
wchar_t szReplaceWith[MAX_PATH];

FINDREPLACE fr;
OPENFILENAME ofn;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);

		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		case IDM_REPLACE:

			ZeroMemory(&fr, sizeof(fr));

			fr.lStructSize = sizeof(fr);
			fr.hwndOwner = hWnd;
			fr.wFindWhatLen = sizeof(szFindWhat);
			fr.wReplaceWithLen = sizeof(szReplaceWith);
			fr.Flags = FR_DOWN;
			fr.lpstrFindWhat = szFindWhat;
			fr.lpstrReplaceWith = szReplaceWith;
			if (ReplaceText(&fr) != FALSE) {

			}

			break;
		case IDM_SAVE:

			ZeroMemory(&ofn, sizeof(ofn));

			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = hWnd;
			ofn.lpstrFilter = L"Текстовые файлы (*.txt)\0*.txt\0Все файлы (*.*)\0*.*\0";
			ofn.lpstrFile = szFileName;
			ofn.nMaxFile = MAX_PATH;
			ofn.Flags = OFN_EXPLORER | OFN_OVERWRITEPROMPT;

			if (GetSaveFileName(&ofn)) {

			}
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;

	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		// TODO: Добавьте сюда любой код прорисовки, использующий HDC...
		EndPaint(hWnd, &ps);
	}
	break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_LBUTTONDBLCLK: {
		MessageBox(hWnd, L"Событие 1 - двойной щелчок левой кнопкой мыши", L"WM_MBUTTONDBLCLK Message", MB_OK | MB_ICONINFORMATION);
		break;
	}

	case WM_LBUTTONDOWN: {
		MessageBox(hWnd, L"Событие 2 - нажатие левой кнопки мыши", L"WM_LBUTTONDOWN Message", MB_OK | MB_ICONINFORMATION);
		break;
	}

	case WM_LBUTTONUP: {
		MessageBox(hWnd, L"Событие 3 - отпускание левой кнопки мыши", L"WM_LBUTTONUP Message", MB_OK | MB_ICONINFORMATION);
		break;
	}

	case WM_MOUSEWHEEL: {
		MessageBox(hWnd, L"Событие 4 - прокрутка колесика мыши", L"WM_MOUSEWHEEL Message", MB_OK | MB_ICONINFORMATION);
		break;
	}

	case WM_SIZING: {
		MessageBox(hWnd, L"Событие 5 - изменение размера окна", L"WM_SIZING Message", MB_OK | MB_ICONINFORMATION);
		break;
	}

	case WM_SYSCHAR: {
		MessageBox(hWnd, L"Событие 6 - обработка системного символа (Alt + буква)", L"WM_SYSCHAR Message", MB_OK | MB_ICONINFORMATION);
		break;
	}

	case WM_SYSKEYUP: {
		MessageBox(hWnd, L"Событие 7 - обработка отпускания системной клавиши", L"WM_SYSCHAR Message", MB_OK | MB_ICONINFORMATION);
		break;
	}

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Обработчик сообщений для окна "О программе".
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
