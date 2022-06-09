#pragma once

class BalanceController;


struct BalanceModelInterface
{
	virtual int GetBalance() = 0;
	virtual void SetBalance(int value) = 0;
	virtual void RegisterObservers(BalanceController* view) = 0;
	virtual void RemoveObserver(BalanceController* view) = 0;
};

