#include "BalanceController.h"

BalanceController::BalanceController()
{
	pModel = std::make_shared<BalanceModel>();
	pModel->RegisterObservers(this);

	// Æô¶¯View
	pView = std::make_unique<BalanceView>(this);
	pView->Create(nullptr, L"BalanceView", UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE);
	pView->CenterWindow();
	pView->ShowModal();
}

void BalanceController::TopUp(int value)
{
	pModel->SetBalance(value);
}

void BalanceController::BalanceIncrease()
{
	int value = pModel->GetBalance();
	pModel->SetBalance(value + 1);
}

void BalanceController::BalanceDecrease()
{
	int value = pModel->GetBalance();
	pModel->SetBalance(value - 1);
}

void BalanceController::OnSuccess()
{
	int value = pModel->GetBalance();
	pView->UpdateBalance(value);
}
