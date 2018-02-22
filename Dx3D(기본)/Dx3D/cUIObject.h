#pragma once
#include "cObject.h"

struct ST_SIZE
{
	float fWidth;
	float fHeight;

	ST_SIZE() : fWidth(0.0f), fHeight(0.0f) {}
	ST_SIZE(float w, float h) : fWidth(w), fHeight(h) {}
};

class cUIObject : public cObject
{
protected:
	D3DXMATRIXA16		m_matWorld;
	vector<cUIObject*>	m_vecChild;
    POINT               m_ptPrev;
    bool                m_isKeyDown;        // �̵� ������ UI�� Ŭ���� ���¶�� true

	SYNTHESIZE(D3DXVECTOR3, m_vLocalPos, LocalPos);
	SYNTHESIZE(cUIObject*, m_pParent, Parent);
	SYNTHESIZE(ST_SIZE, m_stSize, Size);
	SYNTHESIZE(bool, m_isDebugRender, DebugRender);
    SYNTHESIZE(bool, m_isActive, Active); //UI�̵� ���� ����
    SYNTHESIZE(bool, m_isMove, Move); //UI�̵� ���� ����
	SYNTHESIZE(int, m_nTag, Tag);

public:
	cUIObject();
	virtual ~cUIObject();

	virtual void Update();
	virtual void UpdateWorldTM();
	virtual void UpdateChildren();
	virtual void Render(LPD3DXSPRITE pSprite);
	virtual void AddChild(cUIObject* pChild);
	virtual cUIObject* GetChildByTag(int tag);
};

