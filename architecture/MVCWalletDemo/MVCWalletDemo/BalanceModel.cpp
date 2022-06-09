#include "BalanceModel.h"
#include "BalanceController.h"


int BalanceModel::GetBalance()
{
	return balance;
}

void BalanceModel::SetBalance(int value)
{
	balance = value;
	notifyObservers();
}

void BalanceModel::RegisterObservers(BalanceController* view)
{
	observers.push_back(view);
}

void BalanceModel::RemoveObserver(BalanceController* view)
{
	auto iter = std::find(observers.begin(), observers.end(), view);
	if (iter != observers.end())
		observers.erase(iter);
}

void BalanceModel::notifyObservers() const
{
	for (auto& observer : observers)
		(*observer).OnSuccess();
}
