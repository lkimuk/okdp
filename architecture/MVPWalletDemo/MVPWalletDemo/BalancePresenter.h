#pragma once
#include "BalanceView.h"
#include "BalanceModel.h"



class BalancePresenter
{
public:
	BalancePresenter(BalanceViewInterface* view);

	void TopUp();
	void BalanceIncrease();
	void BalanceDecrease();
	
	void OnSuccess();

private:
	std::shared_ptr<BalanceModelInterface> pModel;
	BalanceViewInterface* pView;
};

