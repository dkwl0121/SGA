#include "stdafx.h"
#include "cUITextView.h"


cUITextView::cUITextView()
	: m_pFont(NULL)
	, m_dwDrawTextFormat(DT_CENTER | DT_VCENTER | DT_WORDBREAK | DT_NOCLIP)
	, m_dwColor(D3DCOLOR_XRGB(255, 255, 255))
{
}


cUITextView::~cUITextView()
{
	SAFE_RELEASE(m_pFont);
}

void cUITextView::Render(LPD3DXSPRITE pSprite)
{
	RECT rc;	// 텍스트를 그릴 영역 설정
	SetRect(&rc, m_matWorld._41, m_matWorld._42,
		m_matWorld._41 + m_stSize.fWidth, m_matWorld._42 + m_stSize.fHeight);

	m_pFont->DrawTextA(NULL, m_sText.c_str(), m_sText.length(), &rc,
		m_dwDrawTextFormat, m_dwColor);

	cUIObject::Render(pSprite);
}
