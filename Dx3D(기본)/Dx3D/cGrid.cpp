#include "stdafx.h"
#include "cGrid.h"
#include "cPyramid.h"


cGrid::cGrid()
{
}


cGrid::~cGrid()
{
	for (int i = 0; i < m_vecPyramid.size(); ++i)
		SAFE_DELETE(m_vecPyramid[i]);

	m_vecPyramid.clear();
}

void cGrid::Setup()
{
	int nNumLine = 5;
	float fInterval = 1.0f;
	float fMax = fInterval * nNumLine;

	D3DCOLOR c = D3DCOLOR_XRGB(255, 255, 255);

	for (int i = 1; i <= nNumLine; ++i)
	{
		m_vecPCVertex.push_back(ST_PC_VERTEX(D3DXVECTOR3(-fMax, 0.0f, i * fInterval), c));
		m_vecPCVertex.push_back(ST_PC_VERTEX(D3DXVECTOR3( fMax, 0.0f, i * fInterval), c));
								
		m_vecPCVertex.push_back(ST_PC_VERTEX(D3DXVECTOR3(-fMax, 0.0f,-i * fInterval), c));
		m_vecPCVertex.push_back(ST_PC_VERTEX(D3DXVECTOR3( fMax, 0.0f,-i * fInterval), c));
								
		m_vecPCVertex.push_back(ST_PC_VERTEX(D3DXVECTOR3( i * fInterval, 0.0f,-fMax), c));
		m_vecPCVertex.push_back(ST_PC_VERTEX(D3DXVECTOR3( i * fInterval, 0.0f, fMax), c));
								
		m_vecPCVertex.push_back(ST_PC_VERTEX(D3DXVECTOR3(-i * fInterval, 0.0f,-fMax), c));
		m_vecPCVertex.push_back(ST_PC_VERTEX(D3DXVECTOR3(-i * fInterval, 0.0f, fMax), c));
	}

	cPyramid* pyramid;
	D3DXMATRIXA16 matS, matR, matW;

	c = D3DCOLOR_XRGB(255, 0, 0);
	m_vecPCVertex.push_back(ST_PC_VERTEX(D3DXVECTOR3(-fMax, 0.0f, 0.0f), c));
	m_vecPCVertex.push_back(ST_PC_VERTEX(D3DXVECTOR3( fMax, 0.0f, 0.0f), c));

	D3DXMatrixScaling(&matS, 3.0f, 0.2f, 0.2f);
	D3DXMatrixIdentity(&matR);
	matW = matS * matR;
	pyramid = new cPyramid;
    g_pAutoReleasePool->AddObject(pyramid);
	pyramid->Setup(c, matW);
	m_vecPyramid.push_back(pyramid);

	c = D3DCOLOR_XRGB(0, 255, 0);
	m_vecPCVertex.push_back(ST_PC_VERTEX(D3DXVECTOR3( 0.0f,-fMax, 0.0f), c));
	m_vecPCVertex.push_back(ST_PC_VERTEX(D3DXVECTOR3( 0.0f, fMax, 0.0f), c));

	D3DXMatrixRotationZ(&matR, D3DX_PI / 2);
	matW = matS * matR;
	pyramid = new cPyramid;
    g_pAutoReleasePool->AddObject(pyramid);
	pyramid->Setup(c, matW);
	m_vecPyramid.push_back(pyramid);

	c = D3DCOLOR_XRGB(0, 0, 255);
	m_vecPCVertex.push_back(ST_PC_VERTEX(D3DXVECTOR3( 0.0f, 0.0f,-fMax), c));
	m_vecPCVertex.push_back(ST_PC_VERTEX(D3DXVECTOR3( 0.0f, 0.0f, fMax), c));

	D3DXMatrixRotationY(&matR, -D3DX_PI / 2);
	matW = matS * matR;
	pyramid = new cPyramid;
    g_pAutoReleasePool->AddObject(pyramid);
	pyramid->Setup(c, matW);
	m_vecPyramid.push_back(pyramid);
}

void cGrid::Render()
{
	D3DXMATRIXA16 matW;
	D3DXMatrixIdentity(&matW);

	g_pD3DDevice->SetFVF(ST_PC_VERTEX::FVF);
	g_pD3DDevice->SetTransform(D3DTS_WORLD, &matW);
	g_pD3DDevice->SetTexture(0, NULL);
	g_pD3DDevice->DrawPrimitiveUP(D3DPT_LINELIST, m_vecPCVertex.size() / 2,
		&m_vecPCVertex[0], sizeof(ST_PC_VERTEX));

	for (int i = 0; i < m_vecPyramid.size(); ++i)
		m_vecPyramid[i]->Render();
}
