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

	// �����ӿ�
	int GetBalance() override;
	void UpdateBalance(int value) override;

protected:
	CDuiString GetSkinFile() override;
	LPCTSTR GetWindowClassName(void) const override;

private:
	void OnClickTopUp(TNotifyUI* pObj);            // ��ֵ�¼�
	void OnClickBalanceIncrease(TNotifyUI* pObj);  // ��������¼�
	void OnClickBalanceDecrease(TNotifyUI* pObj);  // ����Լ��¼�

private:
	std::unique_ptr<BalancePresenter> pPresenter;

	UIEventHandler m_ClickHandler;  // ����¼�����
	CButtonUI* btnTopUp;            // ��ֵ��ť
	CButtonUI* btnBalanceIncrease;  // ���������ť
	CButtonUI* btnBalanceDecrease;  // ����Լ���ť
};

