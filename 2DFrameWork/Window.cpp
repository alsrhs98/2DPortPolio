#include "Framework.h"

Application	 App;
Scene* Window::main = nullptr;

WPARAM Window::Run(Scene* main)
{
	Window::main = main;
	//Window->D3D->main
	Create(); 
	D3D->Create();
	GameObject::CreateStaticMember();
	ObRect::CreateStaticMember();
	ObLine::CreateStaticMember();
	ObCircle::CreateStaticMember();
	ObStar::CreateStaticMember();
	ObImage::CreateStaticMember();
	main->Init();
	MSG msg = { 0 };
	while (true)

	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				break;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			TIMER->Chronometry(App.fixFrame);
			INPUT->Update();
			SOUND->Update();
			GUI->Update();
			main->Update();
			main->LateUpdate();
			CAM->Set();
			LIGHT->Set();
			D3D->SetRenderTarget();
			D3D->Clear(App.background);
			DWRITE->GetDC()->BeginDraw();
			{
				main->Render();
				GUI->Render();
			}
			DWRITE->GetDC()->EndDraw();
			D3D->Present();
		}
	}
	GameObject::DeleteStaticMember();
	ObRect::DeleteStaticMember();
	ObLine::DeleteStaticMember();
	ObCircle::DeleteStaticMember();
	ObStar::DeleteStaticMember();
	ObImage::DeleteStaticMember();

	main->Release();
	TIMER->DeleteSingleton();
	INPUT->DeleteSingleton();
	GUI->DeleteSingleton();
	CAM->DeleteSingleton();
	RANDOM->DeleteSingleton();
	SOUND->DeleteSingleton();
	TEXTURE->DeleteSingleton();
	LIGHT->DeleteSingleton();
	SCENE->DeleteSingleton();
	DWRITE->DeleteSingleton();
	D3D->DeleteSingleton();
	Destroy();

	return msg.wParam;
}

void Window::Create()
{

	WNDCLASSEXW wndClass;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wndClass.hIconSm = wndClass.hIcon;
	wndClass.hInstance = App.instance;
	wndClass.lpfnWndProc = (WNDPROC)WndProc;
	wndClass.lpszClassName = App.appName.c_str();
	wndClass.lpszMenuName = NULL;
	wndClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wndClass.cbSize = sizeof(WNDCLASSEX);

	WORD wHr = RegisterClassExW(&wndClass);
	assert(wHr != 0);

	if (App.fullScreen == true)
	{
		DEVMODE devMode = { 0 };
		devMode.dmSize = sizeof(DEVMODE);
		devMode.dmPelsWidth = (DWORD)App.GetWidth();
		devMode.dmPelsHeight = (DWORD)App.GetHeight();
		devMode.dmBitsPerPel = 32;
		devMode.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		ChangeDisplaySettings(&devMode, CDS_FULLSCREEN);
	}
	App.handle = CreateWindowExW
	(
		WS_EX_APPWINDOW
		, App.appName.c_str()
		, App.appName.c_str()
		, WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_OVERLAPPEDWINDOW
		, CW_USEDEFAULT
		, CW_USEDEFAULT
		, CW_USEDEFAULT
		, CW_USEDEFAULT
		, NULL
		, (HMENU)NULL
		, App.instance
		, NULL
	);
	assert(App.handle != NULL);

	RECT rect = { 0, 0, (LONG)App.GetWidth(), (LONG)App.GetHeight() };

	UINT centerX = (GetSystemMetrics(SM_CXSCREEN) - (UINT)App.GetWidth()) / 2;
	UINT centerY = (GetSystemMetrics(SM_CYSCREEN) - (UINT)App.GetHeight()) / 2;

	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);
	MoveWindow
	(
		App.handle
		, centerX, centerY
		, rect.right - rect.left, rect.bottom - rect.top
		, TRUE
	);
	ShowWindow(App.handle, SW_SHOWNORMAL);
	SetForegroundWindow(App.handle);
	SetFocus(App.handle);

	ShowCursor(true);
}

void Window::Destroy()
{

	if (App.fullScreen == true)
		ChangeDisplaySettings(NULL, 0);

	DestroyWindow(App.handle);

	UnregisterClass(App.appName.c_str(), App.instance);
}


LRESULT Window::WndProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (Gui::MsgProc(handle, message, wParam, lParam))
		return true;

	if (message == WM_MOUSEMOVE)
	{
		INPUT->mouseScreenPos.x = (float)LOWORD(lParam);
		INPUT->mouseScreenPos.y = (float)HIWORD(lParam);
	}

	if (message == WM_SIZE)
	{
		if (D3D->GetCreated())
		{
			float width = (float)LOWORD(lParam);
			float height = (float)HIWORD(lParam);
			D3D->ResizeScreen(width, height);
			CAM->ResizeScreen();
			GUI->ResizeScreen();
			if (main)
				main->ResizeScreen();
		}
	}

	if (message == WM_CLOSE || message == WM_DESTROY)
	{
		PostQuitMessage(0);

		return 0;
	}

	return DefWindowProc(handle, message, wParam, lParam);
}

