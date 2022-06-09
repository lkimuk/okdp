#pragma once

class BalancePresenter;


struct BalanceModelInterface
{
	virtual int GetBalance() = 0;
	virtual void SetBalance(int value) = 0;
	virtual void RegisterObservers(BalancePresenter* presenter) = 0;
	virtual void RemoveObserver(BalancePresenter* presenter) = 0;
};

