#include "BalancePresenter.h"


BalancePresenter::BalancePresenter(BalanceViewInterface* view)
	: pView{view}
{
	pModel = std::make_shared<BalanceModel>();
	pModel->RegisterObservers(this);
}

void BalancePresenter::TopUp()
{
	int value = pView->GetBalance();
	pModel->SetBalance(value);
}

void BalancePresenter::BalanceIncrease()
{
	int value = pModel->GetBalance();
	pModel->SetBalance(value + 1);
}

void BalancePresenter::BalanceDecrease()
{
	int value = pModel->GetBalance();
	pModel->SetBalance(value - 1);
}

void BalancePresenter::OnSuccess()
{
	int value = pModel->GetBalance();
	pView->UpdateBalance(value);
}
