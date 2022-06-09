#pragma once
#include "BalanceModelInterface.h"
#include <vector>


class BalanceModel : public BalanceModelInterface
{
public:
	int GetBalance() override;

	void SetBalance(int value) override;

	void RegisterObservers(BalancePresenter* presenter) override;

	void RemoveObserver(BalancePresenter* presenter) override;

private:
	void notifyObservers() const;

private:
	int balance{ 0 };
	std::vector<BalancePresenter*> observers;
};

