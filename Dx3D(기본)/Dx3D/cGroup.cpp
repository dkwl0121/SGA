#include "stdafx.h"
#include "cGroup.h"


cGroup::cGroup()
	: m_pMtlTex(NULL)
    , m_vb(NULL)
    , m_ib(NULL)
{
}


cGroup::~cGroup()
{
    SAFE_RELEASE(m_vb);
    SAFE_RELEASE(m_ib);
	SAFE_RELEASE(m_pMtlTex);
}

void cGroup::Render()
{
    D3DXMATRIXA16 mat;
    D3DXMatrixIdentity(&mat);
    g_pD3DDevice->SetTransform(D3DTS_WORLD, &mat);
	g_pD3DDevice->SetTexture(0, m_pMtlTex->GetTexture());
	g_pD3DDevice->SetMaterial(&m_pMtlTex->GetMtl());
	g_pD3DDevice->SetFVF(ST_PNT_VERTEX::FVF);
    
    if (m_ib && m_vb)
    {
        D3DVERTEXBUFFER_DESC vbDesc;
        m_vb->GetDesc(&vbDesc);
        D3DINDEXBUFFER_DESC ibDesc;
        m_ib->GetDesc(&ibDesc);

        g_pD3DDevice->SetStreamSource(0, m_vb, 0, sizeof(ST_PNT_VERTEX));
        g_pD3DDevice->SetIndices(m_ib);
        g_pD3DDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0,
            vbDesc.Size, 0, ibDesc.Size / 3);
    }
    else if (m_vb)
    {
        D3DVERTEXBUFFER_DESC vbDesc;
        m_vb->GetDesc(&vbDesc);

        g_pD3DDevice->SetFVF(ST_PNT_VERTEX::FVF);
        g_pD3DDevice->SetStreamSource(0, m_vb, 0, sizeof(ST_PNT_VERTEX));
        g_pD3DDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, vbDesc.Size / 3);
    }
}
