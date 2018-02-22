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
	// �ؽ��ĸ� �Ŵ����� �߰� �ϰ� �ؽ��� �迭�� ����
	D3DXIMAGE_INFO stImageInfo;

	g_pTextureManager->AddTexture(sNormal, sNormal);
	m_aTexture[E_NORMAL] = g_pTextureManager->GetTexture(sNormal);

	g_pTextureManager->AddTexture(sMouseOver, sMouseOver);
	m_aTexture[E_MOUSEOVER] = g_pTextureManager->GetTexture(sMouseOver);

	g_pTextureManager->AddTexture(sSelected, sSelected, true);
	m_aTexture[E_SELECTED] = g_pTextureManager->GetTexture(sSelected, &stImageInfo); //�̹��� ������� �´� �Ȱ��� ������ ���������� �̹��� ������ �޾ƿ�

	// �̹��� ������ ����
	m_stSize.fWidth = stImageInfo.Width;
	m_stSize.fHeight = stImageInfo.Height;
}

void cUIButton::Update()
{
	cUIObject::UpdateWorldTM(); // �ڽ��� ��ġ ������ �޾ƿ� ��

	RECT rc;	// ��ư ���� ��Ʈ
	SetRect(&rc, m_matWorld._41, m_matWorld._42,
		m_matWorld._41 + m_stSize.fWidth, m_matWorld._42 + m_stSize.fHeight);

	//POINT ptCurrMouse;
	//GetCursorPos(&ptCurrMouse);				// �ü�������� ���콺 ��ǥ
	//ScreenToClient(g_hWnd, &ptCurrMouse);	    // ���� ���α׷������� ��ǥ�� ���

	if (PtInRect(&rc, g_ptMouse))	// ��ư ���� Ŀ���� �ִ� ���
	{
		if (g_pKeyManager->isStayKeyDown(VK_LBUTTON)) // Ŭ����
		{
            g_pKeyManager->isOnceKeyDown(VK_LBUTTON);  // ��ư �Ѿ �ִ� ��ǥ�� Ŭ���� ���� �ʵ��� ������ �۾�

			if (m_eButtonState == E_MOUSEOVER)
				m_eButtonState = E_SELECTED;
		}
		else // Ŭ���� ���ϰ� ���� ��
		{
			if (m_eButtonState == E_SELECTED) // ������ Ŭ���� �ߴ� ���¶��
			{
				// ������ ��
				if (m_pButton)
					m_pButton->OnClick(this);
			}
			m_eButtonState = E_MOUSEOVER; // ���� ���´� ���콺 ���� ����
		}
	}
	else // ��ư �ۿ� Ŀ���� �ִ� ���
	{
        m_eButtonState = E_NORMAL;
	}

	cUIObject::UpdateChildren();
}

void cUIButton::Render(LPD3DXSPRITE pSprite)
{
	RECT rc;
	SetRect(&rc, 0, 0, m_stSize.fWidth, m_stSize.fHeight);  // �ϴܿ��� �����Ʈ������ ��ȭ�� �ְ� �ֱ� ������ 0, 0���� ������ ��.
	pSprite->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE);

	pSprite->SetTransform(&m_matWorld);
	pSprite->Draw(m_aTexture[m_eButtonState], &rc, &D3DXVECTOR3(0, 0, 0),
		&D3DXVECTOR3(0, 0, 0), D3DCOLOR_XRGB(255, 255, 255));

	pSprite->End();

	cUIObject::Render(pSprite);	// �θ��� ���� ȣ��
}
