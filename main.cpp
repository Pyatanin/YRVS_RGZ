#define _CRT_SECURE_NO_WARNINGS
#define UNICODE
#include <windows.h>
#include <string>

WORD year; // общие данные между дочерним и главным потоком


LRESULT CALLBACK window_handler(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT paint_context;
	HDC hdc; // устройство
	TCHAR buff[256];
	switch (message)
	{
	case WM_CREATE: // событие создания окна
		
		HANDLE t; // дочерний поток
		DWORD t_id;
		t = CreateThread(NULL, 0, thread_handler, NULL, 0, &t_id); // создаем дочерний поток который будет обрабатывать функцию thread_handler
		WaitForSingleObject(t, INFINITE); // ждем окончания потока
		CloseHandle(t); // закрываем поток
		break;
	case WM_PAINT: // событие отрисовки окна
		hdc = BeginPaint(hWnd, &paint_context);
		wsprintf(buff, L"Year : %d", year);
		TextOut(hdc,10, 20, buff, wcslen(buff)); // вывод информации на экран
		EndPaint(hWnd, &paint_context);
		break;

	case WM_DESTROY:
		PostQuitMessage(0); // закрытие приложения
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// функция, которая обрабатывается дочерним потоком
DWORD WINAPI thread_handler(void *)
{
	typedef WORD (*handler)(); // функция обратного вызова
	HINSTANCE library = LoadLibrary(TEXT("get_year.dll")); // загрузка библиотеки
	handler dynamic_lib = (handler)GetProcAddress(library, "get_year"); // извлечение функции из библиотеки
	year = dynamic_lib(); // вызов функции
	return 1;
}


int WINAPI WinMain(HINSTANCE hThisInst, HINSTANCE hPrevInst,
				   LPSTR str, int nWinMode)
{

	WNDCLASS wcl;
	HWND hWnd;
	LPCWSTR szClassName = L"Class";
	LPCWSTR szTitle = L"BOORLAKOV PM-93";
	// создание окна
	wcl.hInstance = hThisInst;
	wcl.lpszClassName = szClassName;
	wcl.lpfnWndProc = window_handler;
	wcl.style = CS_HREDRAW;
	wcl.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wcl.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcl.lpszMenuName = NULL;
	wcl.cbClsExtra = 0;
	wcl.cbWndExtra = 0;
	wcl.hbrBackground = (HBRUSH)GetStockObject(DEFAULT_PALETTE);
	
	RegisterClass(&wcl);
	int window_width = 200;
	int window_height = 100;

	int screen_width = GetSystemMetrics(SM_CXSCREEN );
	int screen_height = GetSystemMetrics(SM_CYSCREEN );
	
	// поцизионирование окна по центру экрана
	hWnd = CreateWindow(szClassName, szTitle, WS_OVERLAPPEDWINDOW |
						WS_CLIPCHILDREN | WS_CLIPSIBLINGS, (screen_width-window_width)/2, (screen_height-window_height)/2, window_width, window_height,
						HWND_DESKTOP, NULL, hThisInst, NULL);
	
	
	// показ и обновление экрана
	ShowWindow(hWnd, nWinMode);
	UpdateWindow(hWnd);
	
	MSG message;
	while (GetMessage(&message, NULL, 0, 0))
		DispatchMessage(&message);
	return message.wParam;
}