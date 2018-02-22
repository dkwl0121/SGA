#include "stdafx.h"
#include "cUIDraw.h"
//#include "cUIObject.h"

cUIDraw::cUIDraw()
	: m_pSprite(NULL)
    , m_pUIRoot(NULL)
{
	ZeroMemory(&m_stImageInfo, sizeof(D3DXIMAGE_INFO));
}

cUIDraw::~cUIDraw()
{
	SAFE_RELEASE(m_pSprite);
}

void cUIDraw::Setup()
{
	D3DXCreateSprite(g_pD3DDevice, &m_pSprite);		// 스프라이트 생성
}

void cUIDraw::Update(D3DXVECTOR3* pos/*=NULL*/)
{
    if (m_pUIRoot)
        m_pUIRoot->Update();
}

void cUIDraw::Render()
{
    if (m_pUIRoot)
        m_pUIRoot->Render(m_pSprite);
}
