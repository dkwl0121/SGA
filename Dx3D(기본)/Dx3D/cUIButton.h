#pragma once
#include "cUIObject.h"

class cUIButton;

interface iUIButton
{
	virtual void OnClick(cUIButton* pSender) PURE;
};

class cUIButton : public cUIObject
{
private:
	enum eButtonState
	{
		E_NORMAL,           // �븻
		E_MOUSEOVER,        // ��ư�� ���콺�� ���� ��
		E_SELECTED,         // ��ư ����
		E_COUNT             // ��ư ���� ����
	};

	eButtonState		m_eButtonState;			// ���� ��ư ����
	LPDIRECT3DTEXTURE9	m_aTexture[E_COUNT];	// ��ư �ؽ��� �迭

	SYNTHESIZE(iUIButton*, m_pButton, UIButton);

public:
	cUIButton();
	virtual ~cUIButton();

	void SetTexture(string sNormal, string sMouseOver, string sSelected);
	
	// cUIObject override
	virtual void Update() override;
	virtual void Render(LPD3DXSPRITE pSprite) override;
};

