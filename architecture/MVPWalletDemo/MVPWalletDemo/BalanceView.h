#pragma once
#include "UIEventHandler.h"
#include "BalanceViewInterface.h"


// forward declaration
class BalancePresenter;


class BalanceView : public WindowImplBase, public BalanceViewInterface
{
public:
	BalanceView();

	void InitWindow() override;
	void Notify(TNotifyUI& msg) override;
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) override;

	// 公共接口
	int GetBalance() override;
	void UpdateBalance(int value) override;

protected:
	CDuiString GetSkinFile() override;
	LPCTSTR GetWindowClassName(void) const override;

private:
	void OnClickTopUp(TNotifyUI* pObj);            // 充值事件
	void OnClickBalanceIncrease(TNotifyUI* pObj);  // 余额自增事件
	void OnClickBalanceDecrease(TNotifyUI* pObj);  // 余额自减事件

private:
	std::unique_ptr<BalancePresenter> pPresenter;

	UIEventHandler m_ClickHandler;  // 点击事件处理
	CButtonUI* btnTopUp;            // 充值按钮
	CButtonUI* btnBalanceIncrease;  // 余额自增按钮
	CButtonUI* btnBalanceDecrease;  // 余额自减按钮
};

