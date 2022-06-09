#pragma once
#include "BalanceView.h"


class BalanceController
{
public:
	BalanceController();

	void TopUp(int value);
	void BalanceIncrease();
	void BalanceDecrease();

	// Model����ɹ���Ļص�����
	void OnSuccess();

private:
	std::shared_ptr<BalanceModelInterface> pModel;
	std::unique_ptr<BalanceView> pView;
};

