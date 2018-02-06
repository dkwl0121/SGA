#include "stdafx.h"
#include "cGroup.h"


cGroup::cGroup()
	: m_pMtlTex(NULL)
{
}


cGroup::~cGroup()
{
	SAFE_RELEASE(m_pMtlTex);
}

void cGroup::Render()
{
    g_pD3DDevice->SetTexture(0, m_pMtlTex->GetTexture());
    g_pD3DDevice->SetMaterial(m_pMtlTex->GetMtl());
    g_pD3DDevice->SetFVF(ST_PNT_VERTEX::FVF);
    g_pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST, m_vecPNTVertex.size() / 3,
        &m_vecPNTVertex[0], sizeof(ST_PNT_VERTEX));
}
