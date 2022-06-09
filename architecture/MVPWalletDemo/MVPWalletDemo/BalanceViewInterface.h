#pragma once

struct BalanceViewInterface
{
	virtual int GetBalance() = 0;
	virtual void UpdateBalance(int value) = 0;
};

