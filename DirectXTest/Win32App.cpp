#include "stdafx.h"
#include "Win32App.h"

HWND Win32App::_hWnd = nullptr;
RECT Win32App::_rect = {0};
int Win32App::Width = 1270;
int Win32App::Height = 720;
int Win32App::_screenWidth = GetSystemMetrics(SM_CXSCREEN);
int Win32App::_screenHeight = GetSystemMetrics(SM_CYSCREEN);
shared_ptr<Renderer> Win32App::_renderer = nullptr;
shared_ptr<Scene> Win32App::_scene = nullptr;
unique_ptr<Keyboard> Win32App::_keyboardEvent = nullptr;
unique_ptr<Mouse> Win32App::_mouseEvent = nullptr;


void Win32App::Init(HINSTANCE hInstance, int nCmdShow) {
	InitWindow(hInstance, nCmdShow);
	_keyboardEvent = make_unique<Keyboard>();
	_mouseEvent = make_unique<Mouse>();
}

int Win32App::Run() {
	return MainLoop();
}

void Win32App::InitWindow(HINSTANCE hInstance, int nCmdShow) {
	WNDCLASSEXW windowClass = {0};
	windowClass.cbSize = sizeof(WNDCLASSEX);
	windowClass.style = CS_HREDRAW | CS_VREDRAW;
	windowClass.lpfnWndProc = WindowProc;
	windowClass.cbClsExtra = 0;
	windowClass.cbWndExtra = 0;
	windowClass.hInstance = hInstance;
	windowClass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DIRECTXTEST));
	windowClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
	windowClass.hbrBackground = (HBRUSH) (COLOR_WINDOW + 1);
	windowClass.lpszClassName = L"DirectXTest";
	windowClass.hIconSm = LoadIcon(windowClass.hInstance, MAKEINTRESOURCE(IDI_SMALL));
	RegisterClassEx(&windowClass);

	_hWnd = CreateWindowW(
			windowClass.lpszClassName,
			L"DirectX Test",
			WS_OVERLAPPEDWINDOW,
			(_screenWidth - Width) / 2, (_screenHeight - Height) / 2, Width, Height,
			nullptr,
			nullptr,
			hInstance,
			nullptr
		);

	GetClientRect(_hWnd, &_rect);

	ShowWindow(_hWnd, nCmdShow);
}

int Win32App::MainLoop() {
	MSG msg = {0};

	while (msg.message != WM_QUIT) {
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		} else {
			if (_scene) {
				_scene->Update();
			}
			if (_renderer) {
				_renderer->Render();
			}
		}
	}

	return (int) msg.wParam;
}

LRESULT CALLBACK Win32App::WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	
	switch (message) {
		case WM_ACTIVATEAPP:
			Keyboard::ProcessMessage(message, wParam, lParam);
			Mouse::ProcessMessage(message, wParam, lParam);
			break;
		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
		case WM_KEYUP:
		case WM_SYSKEYUP:
			Keyboard::ProcessMessage(message, wParam, lParam);
			break;
		case WM_INPUT:
		case WM_MOUSEMOVE:
		case WM_LBUTTONDOWN:
		case WM_LBUTTONUP:
		case WM_RBUTTONDOWN:
		case WM_RBUTTONUP:
		case WM_MBUTTONDOWN:
		case WM_MBUTTONUP:
		case WM_MOUSEWHEEL:
		case WM_XBUTTONDOWN:
		case WM_XBUTTONUP:
		case WM_MOUSEHOVER:
			Mouse::ProcessMessage(message, wParam, lParam);
			break;
		case WM_PAINT:
			{
				PAINTSTRUCT ps;
				HDC hdc = BeginPaint(hWnd, &ps);
				// TODO: Add any drawing code that uses hdc here...
				EndPaint(hWnd, &ps);
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

HWND Win32App::GethWnd() {
	return _hWnd;
}

RECT Win32App::GetScreenRect() {
	return _rect;
}

void Win32App::SetRenderer(shared_ptr<Renderer>& renderer) {
	_renderer = renderer;
}

void Win32App::SetScene(shared_ptr<Scene>& scene) { 
	_scene = scene;
}
