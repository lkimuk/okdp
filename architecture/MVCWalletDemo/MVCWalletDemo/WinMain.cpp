#include "BalanceController.h"


INT WINAPI _tWinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance, LPWSTR lpCmdLine, INT nCmdShow)
{
	CPaintManagerUI::SetInstance(hInstance);
	CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath() + L"skin");

	
	BalanceController controller;

	return 0;
}