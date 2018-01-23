#include "stdafx.h"
#include "cCube.h"


cCube::cCube()
	: m_vLocalPos(0, 0, 0)
	, m_fRotX(0)
	, m_fRotY(0)
	, m_fRotXSpeed(0)
	, m_fRotYSpeed(0)
	, m_isMoving(false)
	, m_vCenter(0, 0, 0)
	, m_pTexture(NULL)
{
	D3DXMatrixIdentity(&m_matWorld);
}


cCube::~cCube()
{
}

void cCube::Setup(vector<D3DXVECTOR2>* vecT, string key, D3DMATERIAL9 material, D3DXMATRIXA16* pMat/*= NULL*/)
{
	vector<D3DXVECTOR3> vecVertex;
	vecVertex.push_back(D3DXVECTOR3(-1.0f,-1.0f,-1.0f));	// 0
	vecVertex.push_back(D3DXVECTOR3(-1.0f, 1.0f,-1.0f));	// 1
	vecVertex.push_back(D3DXVECTOR3( 1.0f, 1.0f,-1.0f));	// 2
	vecVertex.push_back(D3DXVECTOR3( 1.0f,-1.0f,-1.0f));	// 3
	vecVertex.push_back(D3DXVECTOR3(-1.0f,-1.0f, 1.0f));	// 4
	vecVertex.push_back(D3DXVECTOR3(-1.0f, 1.0f, 1.0f));	// 5
	vecVertex.push_back(D3DXVECTOR3( 1.0f, 1.0f, 1.0f));	// 6
	vecVertex.push_back(D3DXVECTOR3( 1.0f,-1.0f, 1.0f));	// 7

	if (pMat)
	{
		for (int i = 0; i < vecVertex.size(); ++i)
			D3DXVec3TransformCoord(&vecVertex[i], &vecVertex[i], pMat);

		D3DXVec3TransformCoord(&m_vCenter, &m_vCenter, &m_matWorld);/////////////////////////////////////////////////////////////////////////
	}

	vector<int>	vecIndex;
	// 아랫면
	vecIndex.push_back(4);
	vecIndex.push_back(0);
	vecIndex.push_back(3);
	vecIndex.push_back(4);
	vecIndex.push_back(3);
	vecIndex.push_back(7);
	// 윗쪽
	vecIndex.push_back(1);
	vecIndex.push_back(5);
	vecIndex.push_back(6);
	vecIndex.push_back(1);
	vecIndex.push_back(6);
	vecIndex.push_back(2);
	// 왼쪽
	vecIndex.push_back(4);
	vecIndex.push_back(5);
	vecIndex.push_back(1);
	vecIndex.push_back(4);
	vecIndex.push_back(1);
	vecIndex.push_back(0);
	// 앞면
	vecIndex.push_back(7);
	vecIndex.push_back(6);
	vecIndex.push_back(5);
	vecIndex.push_back(7);
	vecIndex.push_back(5);
	vecIndex.push_back(4);
	// 오른쪽
	vecIndex.push_back(3);
	vecIndex.push_back(2);
	vecIndex.push_back(6);
	vecIndex.push_back(3);
	vecIndex.push_back(6);
	vecIndex.push_back(7);
	// 뒷면
	vecIndex.push_back(0);
	vecIndex.push_back(1);
	vecIndex.push_back(2);
	vecIndex.push_back(0);
	vecIndex.push_back(2);
	vecIndex.push_back(3);

	if (vecT == NULL)
		SetPNVertex(vecVertex, vecIndex);
	else
	{
		m_pTexture = g_pTextureManager->GetTexture(key);
		SetPNTVertex(vecVertex, vecIndex, vecT);
	}

	m_stMaterial = material;
}

void cCube::SetPNVertex(vector<D3DXVECTOR3>& vecV, vector<int>& vecI)
{
	D3DXVECTOR3 n;

	for (int i = 0; i < vecI.size(); i += 6)
	{
		ComputeNormal(&n, &vecV[vecI[i + 0]], &vecV[vecI[i + 1]], &vecV[vecI[i + 2]]);

		m_vecPNVertex.push_back(ST_PN_VERTEX(vecV[vecI[i + 0]], n));
		m_vecPNVertex.push_back(ST_PN_VERTEX(vecV[vecI[i + 1]], n));
		m_vecPNVertex.push_back(ST_PN_VERTEX(vecV[vecI[i + 2]], n));
		m_vecPNVertex.push_back(ST_PN_VERTEX(vecV[vecI[i + 3]], n));
		m_vecPNVertex.push_back(ST_PN_VERTEX(vecV[vecI[i + 4]], n));
		m_vecPNVertex.push_back(ST_PN_VERTEX(vecV[vecI[i + 5]], n));
	}
}

void cCube::SetPNTVertex(vector<D3DXVECTOR3>& vecV, vector<int>& vecI,
	vector<D3DXVECTOR2>* vecT)
{
	D3DXVECTOR3 n;

	for (int i = 0; i < vecI.size(); i += 6)
	{
		ComputeNormal(&n, &vecV[vecI[i + 0]], &vecV[vecI[i + 1]], &vecV[vecI[i + 2]]);

		m_vecPNTVertex.push_back(ST_PNT_VERTEX(vecV[vecI[i + 0]], n, (*vecT)[(i + 0) % (*vecT).size()]));
		m_vecPNTVertex.push_back(ST_PNT_VERTEX(vecV[vecI[i + 1]], n, (*vecT)[(i + 1) % (*vecT).size()]));
		m_vecPNTVertex.push_back(ST_PNT_VERTEX(vecV[vecI[i + 2]], n, (*vecT)[(i + 2) % (*vecT).size()]));
		m_vecPNTVertex.push_back(ST_PNT_VERTEX(vecV[vecI[i + 3]], n, (*vecT)[(i + 3) % (*vecT).size()]));
		m_vecPNTVertex.push_back(ST_PNT_VERTEX(vecV[vecI[i + 4]], n, (*vecT)[(i + 4) % (*vecT).size()]));
		m_vecPNTVertex.push_back(ST_PNT_VERTEX(vecV[vecI[i + 5]], n, (*vecT)[(i + 5) % (*vecT).size()]));
	}
}

void cCube::Update(D3DXMATRIXA16* pParent/*= NULL*/)
{
	// X 축의 로테이션 값 계산
	if (m_isMoving)
	{
		m_fRotY = 0.0f;	// 앞만 봐라!

		m_fRotX += m_fRotXSpeed;

		if (m_fRotX >= D3DX_PI / 4 - D3DX_16F_EPSILON ||
			m_fRotX <= -D3DX_PI / 4 + D3DX_16F_EPSILON)
			m_fRotXSpeed *= -1;
	}
	else
	{
		m_fRotX = 0.0f;	// 차렷

		// Y 축의 로테이션 값 계산
		m_fRotY += m_fRotYSpeed;

		if (m_fRotY >= D3DX_PI / 3 - D3DX_16F_EPSILON ||
			m_fRotY <= -D3DX_PI / 3 + D3DX_16F_EPSILON)
			m_fRotYSpeed *= -1;
	}

	D3DXMATRIXA16 matRX, matRY, matRT, matInvRT, matT;

	D3DXMatrixRotationX(&matRX, m_fRotX);
	D3DXMatrixRotationY(&matRY, m_fRotY);

	D3DXMatrixTranslation(&matRT, m_vLocalPos.x, m_vLocalPos.y, m_vLocalPos.z);
	D3DXMatrixTranslation(&matInvRT, -m_vLocalPos.x, -m_vLocalPos.y, -m_vLocalPos.z);

	D3DXMatrixTranslation(&matT, m_vCenter.x, m_vCenter.y, m_vCenter.z);

	// 회전축을 이동 시키고 회전을 시킨 뒤 원래 위치로 복구
	m_matWorld = matRT * matRX * matRY * matInvRT * matT;

	if (pParent)
		m_matWorld *= *pParent;

	for (int i = 0; i < m_vecChild.size(); ++i)
		m_vecChild[i]->Update(&m_matWorld);
}

void cCube::Render()
{
	g_pD3DDevice->SetTransform(D3DTS_WORLD, &m_matWorld);
	g_pD3DDevice->SetTexture(0, m_pTexture);
	g_pD3DDevice->SetMaterial(&m_stMaterial);

	if (!m_vecPNVertex.empty())
	{
		g_pD3DDevice->SetFVF(ST_PN_VERTEX::FVF);
		g_pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST, m_vecPNVertex.size() / 3,
			&m_vecPNVertex[0], sizeof(ST_PN_VERTEX));
	}
	else if (!m_vecPNTVertex.empty())
	{
		g_pD3DDevice->SetFVF(ST_PNT_VERTEX::FVF);
		g_pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST, m_vecPNTVertex.size() / 3,
			&m_vecPNTVertex[0], sizeof(ST_PNT_VERTEX));
	}

	for (int i = 0; i < m_vecChild.size(); ++i)
		m_vecChild[i]->Render();
}

void cCube::AddChild(cCube* pCube)
{
	m_vecChild.push_back(pCube);
}

void cCube::Release()
{
	for (int i = 0; i < m_vecChild.size(); ++i)
		m_vecChild[i]->Release();

	delete this;
}

void cCube::SetIsMoving(bool move)
{
	m_isMoving = move;

	for (int i = 0; i < m_vecChild.size(); ++i)
		m_vecChild[i]->SetIsMoving(move);
}

// AABB 충돌
bool cCube::Collision(D3DXVECTOR3& center)
{
	bool isCol = false;
	float fRadius = BLANK_DISTANCE * 0.5f;

	// 충돌 했으면
	if (m_vCenter.x - fRadius < center.x + fRadius && m_vCenter.x + fRadius > center.x - fRadius
		&& m_vCenter.y - fRadius < center.y + fRadius && m_vCenter.y + fRadius > center.y - fRadius
		&& m_vCenter.z - fRadius < center.z + fRadius && m_vCenter.z + fRadius > center.z - fRadius)
		isCol = true;

	return isCol;
}

// 구충돌
//bool cCube::Collision(D3DXVECTOR3& center, float radius)
//{
//	bool isCol = false;
//
//	D3DXVECTOR3 v = m_vCenter - center;
//	float length = D3DXVec3Length(&v);
//
//	if (length < m_fRadius + radius) // 충돌
//	{
//		D3DXVec3Normalize(&v, &v);
//		m_vCenter += v * 1.0f;
//		isCol = true;
//	}
//
//	return isCol;
//}