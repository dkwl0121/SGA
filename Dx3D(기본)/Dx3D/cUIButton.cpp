#include "stdafx.h"
#include "cUIButton.h"


cUIButton::cUIButton()
{
}


cUIButton::~cUIButton()
{
}

void cUIButton::SetTexture(string sNormal, string sMouseOver, string sSelected)
{
	// 텍스쳐를 매니져에 추가 하고 텍스쳐 배열에 셋팅
	D3DXIMAGE_INFO stImageInfo;

	g_pTextureManager->AddTexture(sNormal, sNormal);
	m_aTexture[E_NORMAL] = g_pTextureManager->GetTexture(sNormal);

	g_pTextureManager->AddTexture(sMouseOver, sMouseOver);
	m_aTexture[E_MOUSEOVER] = g_pTextureManager->GetTexture(sMouseOver);

	g_pTextureManager->AddTexture(sSelected, sSelected, true);
	m_aTexture[E_SELECTED] = g_pTextureManager->GetTexture(sSelected, &stImageInfo); //이미지 사이즈는 셋다 똑같기 때문에 마지막에만 이미지 정보를 받아옴

	// 이미지 사이즈 셋팅
	m_stSize.fWidth = stImageInfo.Width;
	m_stSize.fHeight = stImageInfo.Height;
}

void cUIButton::Update()
{
	cUIObject::UpdateWorldTM(); // 자신의 위치 정보를 받아온 뒤

	RECT rc;	// 버튼 영역 렉트
	SetRect(&rc, m_matWorld._41, m_matWorld._42,
		m_matWorld._41 + m_stSize.fWidth, m_matWorld._42 + m_stSize.fHeight);

	//POINT ptCurrMouse;
	//GetCursorPos(&ptCurrMouse);				// 운영체제에서의 마우스 좌표
	//ScreenToClient(g_hWnd, &ptCurrMouse);	    // 현재 프로그램에서의 좌표로 계산

	if (PtInRect(&rc, g_ptMouse))	// 버튼 위에 커서가 있는 경우
	{
		if (g_pKeyManager->isStayKeyDown(VK_LBUTTON)) // 클릭중
		{
            g_pKeyManager->isOnceKeyDown(VK_LBUTTON);  // 버튼 넘어에 있는 좌표에 클릭이 되지 않도록 버리는 작업

			if (m_eButtonState == E_MOUSEOVER)
				m_eButtonState = E_SELECTED;
		}
		else // 클릭을 안하고 있을 때
		{
			if (m_eButtonState == E_SELECTED) // 이전에 클릭을 했던 상태라면
			{
				// 눌렀다 땜
				if (m_pButton)
					m_pButton->OnClick(this);
			}
			m_eButtonState = E_MOUSEOVER; // 현재 상태는 마우스 오버 상태
		}
	}
	else // 버튼 밖에 커서가 있는 경우
	{
        m_eButtonState = E_NORMAL;
	}

	cUIObject::UpdateChildren();
}

void cUIButton::Render(LPD3DXSPRITE pSprite)
{
	RECT rc;
	SetRect(&rc, 0, 0, m_stSize.fWidth, m_stSize.fHeight);  // 하단에서 월드매트릭스로 변화를 주고 있기 때문에 0, 0으로 셋팅을 함.
	pSprite->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE);

	pSprite->SetTransform(&m_matWorld);
	pSprite->Draw(m_aTexture[m_eButtonState], &rc, &D3DXVECTOR3(0, 0, 0),
		&D3DXVECTOR3(0, 0, 0), D3DCOLOR_XRGB(255, 255, 255));

	pSprite->End();

	cUIObject::Render(pSprite);	// 부모의 렌더 호출
}
