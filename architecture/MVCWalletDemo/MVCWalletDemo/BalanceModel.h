#pragma once
#include "BalanceModelInterface.h"
#include <vector>


class BalanceModel : public BalanceModelInterface
{
public:

	int GetBalance() override;
	
	void SetBalance(int value) override;

	void RegisterObservers(BalanceController* view) override;

	void RemoveObserver(BalanceController* view) override;

private:
	void notifyObservers() const;

private:
	int balance{ 0 }; // ”‡∂Ó
	std::vector<BalanceController*> observers;
};

