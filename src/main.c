#if _WIN32

#include "stdafx.h"
#include "draw.h"
#include "font.h"


#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stb/stb_sprintf.h>
#include <time.h>

#pragma comment(lib, "user32.lib")
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "kernel32.lib")

static bool quit = false;
static Texture backBuffer;

static void displayBuffer(HWND hwnd, HDC deviceContext)
{
	RECT rect = {0};
	GetClientRect(hwnd, &rect);
	int width = rect.right - rect.left;
	int height = rect.bottom - rect.top;

	BITMAPINFO info = {
		.bmiHeader = {
			.biSize = sizeof(info),
			.biWidth = backBuffer.width,
			.biHeight = -(int32_t)backBuffer.height,
			.biPlanes = 1,
			.biBitCount = 32,
			.biCompression = BI_RGB
		}
	};

	StretchDIBits(
		deviceContext,
		0, 0,
		width, height,
		0, 0,
		backBuffer.width, backBuffer.height,
		backBuffer.memory, &info,
		DIB_RGB_COLORS, SRCCOPY);
}

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
	} break;
	case WM_PAINT: {
		PAINTSTRUCT paint;
		HDC dc = BeginPaint(hwnd, &paint);
		displayBuffer(hwnd, dc);
		EndPaint(hwnd, &paint);
	} break;
	case WM_SIZE: {
		RECT clientRect = {0};
		GetClientRect(hwnd, &clientRect);
		backBuffer.width = clientRect.right - clientRect.left;
		backBuffer.height = clientRect.bottom - clientRect.top;

		if (backBuffer.memory)
			VirtualFree(backBuffer.memory, 0, MEM_RELEASE);

		size_t backBufferSize =
		    backBuffer.width * backBuffer.height * sizeof(uint32_t);

		backBuffer.memory = (uint32_t*)VirtualAlloc(0, backBufferSize,
		                                            MEM_COMMIT, PAGE_READWRITE);
		backBuffer.pitch = backBuffer.width;
		draw(&backBuffer);
	} break;
	default: {
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	} break;
	}

	return 0;
}


int WINAPI WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
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

	HWND hwnd = CreateWindowExA(
		0, wcex.lpszClassName, "PK", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL,
		hInstance, NULL);

	if (!hwnd) {
		return errorWin32("Failed to create window");
	}

	ShowWindow(hwnd, nCmdShow);

	MSG msg = {0};
	while (GetMessage(&msg, hwnd, 0, 0) && !quit) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);

		clock_t startClock = clock();
		draw(&backBuffer);
		clock_t endClock = clock();
		size_t frameTimeMS = ((endClock - startClock) * 1000) / CLOCKS_PER_SEC;
		char buffer[255];
		stbsp_snprintf(buffer, sizeof(buffer),"PK - %dms (%dx%d)", frameTimeMS, backBuffer.width, backBuffer.height);
		SetWindowTextA(hwnd, buffer);

		HDC dc = GetDC(hwnd);
		displayBuffer(hwnd, dc);
		ReleaseDC(hwnd, dc);
	}

	return 0;
}

#endif /* _WIN32 */
