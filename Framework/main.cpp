//Linkers
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")


#include "SystemClass.h"
#include "Define.h"
#include <crtdbg.h>

#ifndef _DEBUG
#define new new(_CLIENT_BLOC, __FILE_,__LINE)
#endif

void DestroySingleObject()
{
	CameraClass::GetInstance()->DestroyInstance();
}

int WINAPI WinMain(HINSTANCE _hInstance, HINSTANCE _hPrevInstance, PSTR _pScmdline, int _iCmdshow)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	SystemClass* pSystem;

	pSystem = new SystemClass;
	if (!pSystem)
		return 0;

	if (pSystem->Init())
	{
		pSystem->Update();
	}

	safe_delete_destroy(pSystem);
	DestroySingleObject();

	return 0;
}

