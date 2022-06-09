#include "BalanceModel.h"
#include "BalancePresenter.h"


int BalanceModel::GetBalance()
{
	return balance;
}

void BalanceModel::SetBalance(int value)
{
	balance = value;
	notifyObservers();
}

void BalanceModel::RegisterObservers(BalancePresenter* presenter)
{
	observers.push_back(presenter);
}

void BalanceModel::RemoveObserver(BalancePresenter* presenter)
{
	auto iter = std::find(observers.begin(), observers.end(), presenter);
	if (iter != observers.end())
		observers.erase(iter);
}

void BalanceModel::notifyObservers() const
{
	for (auto& observer : observers)
		(*observer).OnSuccess();
}
