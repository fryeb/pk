#if _WIN32

#include "stdafx.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#pragma comment(lib, "user32.lib")

static bool quit = false;

static DWORD errorWin32(const char *msg)
{
	MessageBoxA(NULL, msg, "Windows Error", MB_OK);
	return GetLastError();
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg) {
	case WM_CLOSE: {
		quit = true;
		return 0;
	} break;
	default: {
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	} break;
	}
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
		   LPSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	WNDCLASSEXA wcex = {
	    .cbSize = sizeof(wcex),
	    .style = CS_HREDRAW | CS_VREDRAW,
	    .lpfnWndProc = WindowProc,
	    .hInstance = hInstance,
	    .hIcon = LoadIcon(NULL, IDI_APPLICATION),
	    .hCursor = LoadCursor(NULL, IDC_ARROW),
	    .lpszClassName = "QkWindowClass",
	};

	if (!RegisterClassExA(&wcex)) {
		return errorWin32("Failed to register window class");
	}

	HWND hwnd =
	    CreateWindowExA(0, wcex.lpszClassName, "QK", WS_OVERLAPPEDWINDOW,
			    CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
			    CW_USEDEFAULT, NULL, NULL, hInstance, NULL);

	if (!hwnd) {
		return errorWin32("Failed to create window");
	}

	ShowWindow(hwnd, nCmdShow);

	MSG msg = {0};
	while (GetMessage(&msg, hwnd, 0, 0) && !quit) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}

#endif /* _WIN32 */
