#include "BalancePresenter.h"


INT WINAPI _tWinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance, LPWSTR lpCmdLine, INT nCmdShow)
{
	CPaintManagerUI::SetInstance(hInstance);
	CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath() + L"skin");

	auto view = std::make_unique<BalanceView>();
	view->Create(nullptr, L"BalanceView", UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE);
	view->CenterWindow();
	view->ShowModal();


	return 0;
}
