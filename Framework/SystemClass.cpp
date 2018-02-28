#include "SystemClass.h"

#ifndef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

SystemClass::SystemClass() 
	: m_pGraphics(NULL)
{
}

SystemClass::SystemClass(const SystemClass& arg)
{

}

SystemClass::~SystemClass()
{
}

bool SystemClass::Init()
{
	int screenWidth = 0, screenHeight = 0;

	InitializeWindows(screenWidth, screenHeight);

	m_pGraphics = new GraphicsClass;
	if (!m_pGraphics)
	{
		return false;
	}

	if (!m_pGraphics->Init(screenWidth, screenHeight, m_hWnd))
	{
		return false;
	}

	return true;
}

void SystemClass::Update()
{
	MSG msg;
	bool done = false;

	ZeroMemory(&msg, sizeof(MSG));

	while (!done)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if (msg.message == WM_QUIT)
		{
			done = true;
		}
		else
		{
			if (!Frame())
			{
				done = true;
			}
		}
	}

	return;
}

void SystemClass::Destroy()
{
	safe_delete_destroy(m_pGraphics);

	ShutdownWindows();

	return;
}

bool SystemClass::Frame()
{
	if (!m_pGraphics->Frame())
	{
		return false;
	}

	return true;
}

LRESULT CALLBACK SystemClass::MessageHander(HWND _hWnd, UINT _msg, WPARAM _wParam, LPARAM _lParam)
{
	switch (_msg)
	{
	case WM_KEYDOWN:
		switch (_wParam)
		{
		case VK_ESCAPE:
			PostQuitMessage(0);
			break;
		}
		break;
	default:
		return DefWindowProc(_hWnd, _msg, _wParam, _lParam);
	}
	return NULL;
}

void SystemClass::InitializeWindows(int& _screenWidth, int& _screenHeight)
{
	WNDCLASSEX wc;
	int posX = 0, posY = 0;

	ApplicationHandle = this;

	m_hInstance = GetModuleHandle(NULL);

	m_applicationName = L"Engine";

	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = m_applicationName;
	wc.cbSize = sizeof(WNDCLASSEX);

	RegisterClassEx(&wc);

	_screenWidth = 800;	_screenHeight = 600;

	posX = (GetSystemMetrics(SM_CXSCREEN) - _screenWidth) / 2;
	posY = (GetSystemMetrics(SM_CYSCREEN) - _screenHeight) / 2;

	m_hWnd = CreateWindowEx(WS_EX_APPWINDOW, m_applicationName, m_applicationName,
		WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
		posX, posY, _screenWidth, _screenHeight, NULL, NULL, m_hInstance, NULL);

	ShowWindow(m_hWnd, SW_SHOW);
	SetForegroundWindow(m_hWnd);
	SetFocus(m_hWnd);

	return;
}

void SystemClass::ShutdownWindows()
{
	DestroyWindow(m_hWnd);
	m_hWnd = NULL;

	UnregisterClass(m_applicationName, m_hInstance);
	m_hInstance = NULL;

	ApplicationHandle = NULL;

	return;
}

LRESULT CALLBACK WndProc(HWND _hWnd, UINT _msg, WPARAM _wParam, LPARAM _lParam)
{
	switch (_msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_CLOSE:
		PostQuitMessage(0);
		break;
	default:
		return ApplicationHandle->MessageHander(_hWnd, _msg, _wParam, _lParam);
		break;
	}

	return NULL;
}