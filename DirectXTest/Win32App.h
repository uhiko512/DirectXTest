#pragma once

#include "resource.h"

class Renderer;
class Scene;
namespace DirectX {
	class Keyboard;
	class Mouse;
}

using namespace std;
using namespace DirectX;

class Win32App {
public:
	static void Init(HINSTANCE hInstance, int nCmdShow);
	static int Run();
	static HWND GethWnd();
	static RECT GetScreenRect();
	static void SetRenderer(shared_ptr<Renderer>& renderer);
	static void SetScene(shared_ptr<Scene>& scene);
	static int Width;
	static int Height;
	

protected:
	static LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

private:
	static HWND _hWnd;
	static RECT _rect;
	static int _screenWidth;
	static int _screenHeight;
	static shared_ptr<Renderer> _renderer;
	static shared_ptr<Scene> _scene;
	static unique_ptr<Keyboard> _keyboardEvent;
	static unique_ptr<Mouse> _mouseEvent;

	static void InitWindow(HINSTANCE hInstance, int nCmdShow);
	static int MainLoop();
};

