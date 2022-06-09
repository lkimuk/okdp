#pragma once
#include "UIEventHandler.h"
#include "BalanceModel.h"

// forward declaration
class BalanceController;


class BalanceView : public WindowImplBase
{
public:
	BalanceView(BalanceController* controller);

	void InitWindow() override;
	void Notify(TNotifyUI& msg) override;
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) override;

	void UpdateBalance(int value);

protected:
	CDuiString GetSkinFile() override;
	LPCTSTR GetWindowClassName(void) const override;

private:
	void OnClickTopUp(TNotifyUI* pObj);            // ��ֵ�¼�
	void OnClickBalanceIncrease(TNotifyUI* pObj);  // ��������¼�
	void OnClickBalanceDecrease(TNotifyUI* pObj);  // ����Լ��¼�

private:
	BalanceController* pController;

	UIEventHandler m_ClickHandler;  // ����¼�����
	CButtonUI* btnTopUp;            // ��ֵ��ť
	CButtonUI* btnBalanceIncrease;  // ���������ť
	CButtonUI* btnBalanceDecrease;  // ����Լ���ť
};

