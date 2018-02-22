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
		E_NORMAL,           // 노말
		E_MOUSEOVER,        // 버튼에 마우스가 있을 때
		E_SELECTED,         // 버튼 선택
		E_COUNT             // 버튼 상태 개수
	};

	eButtonState		m_eButtonState;			// 현재 버튼 상태
	LPDIRECT3DTEXTURE9	m_aTexture[E_COUNT];	// 버튼 텍스쳐 배열

	SYNTHESIZE(iUIButton*, m_pButton, UIButton);

public:
	cUIButton();
	virtual ~cUIButton();

	void SetTexture(string sNormal, string sMouseOver, string sSelected);
	
	// cUIObject override
	virtual void Update() override;
	virtual void Render(LPD3DXSPRITE pSprite) override;
};

