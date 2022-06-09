#include "BalanceView.h"
#include "BalancePresenter.h"



BalanceView::BalanceView()
{
	pPresenter = std::make_unique<BalancePresenter>(this);
}

void BalanceView::InitWindow()
{
	// 充值按钮
	btnTopUp = static_cast<CButtonUI*>(m_PaintManager.FindControl(L"btnTopUp"));
	m_ClickHandler.AddHandler(btnTopUp, std::bind(&BalanceView::OnClickTopUp, this, std::placeholders::_1));

	// 余额自增按钮
	btnBalanceIncrease = static_cast<CButtonUI*>(m_PaintManager.FindControl(L"btnBalanceIncrease"));
	m_ClickHandler.AddHandler(btnBalanceIncrease, std::bind(&BalanceView::OnClickBalanceIncrease, this, std::placeholders::_1));

	// 余额自减按钮
	btnBalanceDecrease = static_cast<CButtonUI*>(m_PaintManager.FindControl(L"btnBalanceDecrease"));
	m_ClickHandler.AddHandler(btnBalanceDecrease, std::bind(&BalanceView::OnClickBalanceDecrease, this, std::placeholders::_1));
}

void BalanceView::Notify(TNotifyUI& msg)
{
	if (msg.sType == L"click") {
		if (msg.pSender->GetName() == L"btnMin")
			SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0);
		else if (msg.pSender->GetName() == L"btnClose")
			PostQuitMessage(0);
		else
			m_ClickHandler.Dispatch(&msg);
	}
}

LRESULT BalanceView::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return __super::HandleMessage(uMsg, wParam, lParam);
}

int BalanceView::GetBalance()
{
	CEditUI* etTopUpValue = static_cast<CEditUI*>(m_PaintManager.FindControl(L"editTopUpValue"));
	return _ttoi(etTopUpValue->GetText().GetData());
}

void BalanceView::UpdateBalance(int value)
{
	CDuiString strValue;
	strValue.Format(L"%d", value);

	CEditUI* etCurrentBalance = static_cast<CEditUI*>(m_PaintManager.FindControl(L"editCurrentBalance"));
	etCurrentBalance->SetText(strValue);
}

DuiLib::CDuiString BalanceView::GetSkinFile()
{
	return L"layout.xml";
}

LPCTSTR BalanceView::GetWindowClassName(void) const
{
	return L"BalanceView";
}

void BalanceView::OnClickTopUp(TNotifyUI* pObj)
{
	// 交由Presenter负责与Model沟通
	pPresenter->TopUp();
}

void BalanceView::OnClickBalanceIncrease(TNotifyUI* pObj)
{
	// 交由Presenter负责与Model沟通
	pPresenter->BalanceIncrease();
}

void BalanceView::OnClickBalanceDecrease(TNotifyUI* pObj)
{
	// 交由Presenter负责与Model沟通
	pPresenter->BalanceDecrease();
}
