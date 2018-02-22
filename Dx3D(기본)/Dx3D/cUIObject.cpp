#include "stdafx.h"
#include "cUIObject.h"


cUIObject::cUIObject()
	: m_vLocalPos(0, 0, 0)
	, m_pParent(NULL)
	, m_stSize(0, 0)
	, m_isDebugRender(false)
    , m_isActive(true)
    , m_isMove(false)
	, m_nTag(-1)
    , m_ptPrev(g_ptMouse)
    , m_isKeyDown(false)
{
	D3DXMatrixIdentity(&m_matWorld);
}


cUIObject::~cUIObject()
{
}

void cUIObject::Update()
{
    // Ȱ��ȭ�� �Ǿ� ���� �ʴٸ� ������Ʈ ����(�ڽ�����)
    if (!m_isActive) return;

    // �̵��� ������ UI���
    if (m_isMove)
    {
        if (g_ptMouse.x > m_vLocalPos.x
            && g_ptMouse.x < m_vLocalPos.x + m_stSize.fWidth
            &&g_ptMouse.y > m_vLocalPos.y
            && g_ptMouse.y < m_vLocalPos.y + 20)
        {
            if (g_pKeyManager->isOnceKeyDown(VK_LBUTTON))
            {
                m_isKeyDown = true;
                m_ptPrev = g_ptMouse;
            }
            else if (g_pKeyManager->isOnceKeyUp(VK_LBUTTON))
            {
                m_isKeyDown = false;
            }
        }

        if (m_isKeyDown)
        {
            m_vLocalPos.x += g_ptMouse.x - m_ptPrev.x;
            m_vLocalPos.y += g_ptMouse.y - m_ptPrev.y;
            m_ptPrev = g_ptMouse;
        }
    }

	UpdateWorldTM();
	UpdateChildren();
}

void cUIObject::UpdateWorldTM()
{
	m_matWorld._41 = m_vLocalPos.x;
	m_matWorld._42 = m_vLocalPos.y;
	m_matWorld._43 = m_vLocalPos.z;

	if (m_pParent)	// Ʈ�� ���������� ��ġ�� �θ� ���󰣴�.
	{
		m_matWorld._41 += m_pParent->m_matWorld._41;
		m_matWorld._42 += m_pParent->m_matWorld._42;
		m_matWorld._43 += m_pParent->m_matWorld._43;
	}
}

void cUIObject::UpdateChildren()
{
	for each (auto p in m_vecChild)
		p->Update();
}

void cUIObject::Render(LPD3DXSPRITE pSprite)
{
    // Ȱ��ȭ�� �Ǿ� ���� �ʴٸ� ���� ����(�ڽ�����)
    if (!m_isActive) return;

	if (m_isDebugRender)
	{
		float l = m_matWorld._41;
		float t = m_matWorld._42;
		float r = m_matWorld._41 + m_stSize.fWidth;
		float b = m_matWorld._42 + m_stSize.fHeight;

		D3DCOLOR c = D3DCOLOR_XRGB(255, 0, 0);
		vector<ST_RHWC_VERTEX> vecVertex;
		vecVertex.reserve(8);

		vecVertex.push_back(ST_RHWC_VERTEX(D3DXVECTOR4(l, t, 0, 1), c));
		vecVertex.push_back(ST_RHWC_VERTEX(D3DXVECTOR4(r, t, 0, 1), c));

		vecVertex.push_back(ST_RHWC_VERTEX(D3DXVECTOR4(r, t, 0, 1), c));
		vecVertex.push_back(ST_RHWC_VERTEX(D3DXVECTOR4(r, b, 0, 1), c));

		vecVertex.push_back(ST_RHWC_VERTEX(D3DXVECTOR4(r, b, 0, 1), c));
		vecVertex.push_back(ST_RHWC_VERTEX(D3DXVECTOR4(l, b, 0, 1), c));

		vecVertex.push_back(ST_RHWC_VERTEX(D3DXVECTOR4(l, b, 0, 1), c));
		vecVertex.push_back(ST_RHWC_VERTEX(D3DXVECTOR4(l, t, 0, 1), c));

		g_pD3DDevice->SetFVF(ST_RHWC_VERTEX::FVF);
		g_pD3DDevice->DrawPrimitiveUP(D3DPT_LINELIST, 4, &vecVertex[0], sizeof(ST_RHWC_VERTEX));
	}

	for each (auto p in m_vecChild)
		p->Render(pSprite);
}

void cUIObject::AddChild(cUIObject* pChild)
{
	pChild->m_pParent = this;		// �ڽ����� �߰� �� UI�� �θ� ���� UI�� ����
	m_vecChild.push_back(pChild);	// �ڽ� ���Ϳ� �߰�
}

cUIObject* cUIObject::GetChildByTag(int tag)
{
	if (m_nTag == tag)	// ã�� �ִ� tag ���� ���� UI �� ���
		return this;

	for each(auto p in m_vecChild)	// �ڽĵ� �߿� ã�� tag ���� �ִ��� �˻�
	{
		cUIObject* pChild = p->GetChildByTag(tag);
		if (pChild)
			return pChild;
	}

	return NULL;
}
