#pragma once

#define WIN32_LEAN_AND_MEAN	//������� �ʴ� Window �Լ��� �ɷ������ν� ����ȭ ����
#include <Windows.h>
#include "GraphicsClass.h"

class SystemClass
{
public:
	SystemClass();
	SystemClass(const SystemClass&);
	~SystemClass();

public:
	bool Init();
	void Update();
	void Destroy();
	LRESULT CALLBACK MessageHander(HWND, UINT, WPARAM, LPARAM);

private:
	bool Frame();
	void InitializeWindows(int&, int&);
	void ShutdownWindows();

private:
	LPCWSTR		m_applicationName;
	HINSTANCE	m_hInstance;
	HWND		m_hWnd;

	GraphicsClass* m_pGraphics;
};

static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

static SystemClass* ApplicationHandle = NULL;