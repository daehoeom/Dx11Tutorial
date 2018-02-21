#include "SystemClass.h"
#include "Define.h"

void DestroySingleObject()
{
	CameraClass::GetInstance()->DestroyInstance();
}

int WINAPI WinMain(HINSTANCE _hInstance, HINSTANCE _hPrevInstance, PSTR _pScmdline, int _iCmdshow)
{
	SystemClass* pSystem;

	pSystem = new SystemClass;
	if (!pSystem)
		return 0;

	if (pSystem->Init())
	{
		pSystem->Update();
	}

	pSystem->Destroy();
	SAFE_DELETE(pSystem);
	DestroySingleObject();

	return 0;
}
