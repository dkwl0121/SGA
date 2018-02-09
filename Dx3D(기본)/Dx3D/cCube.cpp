#include "stdafx.h"
#include "cCube.h"


cCube::cCube()
	: m_vLocalPos(0, 0, 0)
	, m_fRotX(0)
	, m_fRotY(0)
	, m_fRotXSpeed(0)
	, m_fRotYSpeed(0)
    , m_fRadiusX(1)
    , m_fRadiusY(1)
    , m_fRadiusZ(1)
	, m_isMoving(false)
	, m_vCenter(0, 0, 0)
	, m_pTexture(NULL)
    , m_vb(NULL)
    , m_ib(NULL)
{
	D3DXMatrixIdentity(&m_matWorld);
}


cCube::~cCube()
{
    SAFE_RELEASE(m_vb);
    SAFE_RELEASE(m_ib);
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
        m_vCenter = D3DXVECTOR3(pMat->_41, pMat->_42, pMat->_43);

        //pMat->_41 = 0.0f;
        //pMat->_42 = 0.0f;
        //pMat->_43 = 0.0f;

        // 반지름 사이즈
        m_fRadiusX = pMat->_11;
        m_fRadiusY = pMat->_22;
        m_fRadiusZ = pMat->_33;

		for (int i = 0; i < vecVertex.size(); ++i)
			D3DXVec3TransformCoord(&vecVertex[i], &vecVertex[i], pMat);
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
    ST_PN_VERTEX*		pPNVertex;
    WORD*				pIndex;

    // 버텍스 버퍼 생성
    g_pD3DDevice->CreateVertexBuffer(24 * sizeof(ST_PN_VERTEX), NULL,
        ST_PN_VERTEX::FVF, D3DPOOL_MANAGED, &m_vb, NULL);
    // 인덱스 버퍼 생성
    g_pD3DDevice->CreateIndexBuffer(36 * sizeof(WORD), NULL,
        D3DFMT_INDEX16, D3DPOOL_DEFAULT, &m_ib, NULL);

    // 버퍼 접근 시 락 설정!
    m_vb->Lock(NULL, NULL, (LPVOID*)&pPNVertex, NULL);

    D3DXVECTOR3 n; // 노말값
    int index = 0;
    for (int i = 0; i < vecI.size(); i += 6)
    {
        ComputeNormal(&n, &vecV[vecI[i + 0]], &vecV[vecI[i + 1]], &vecV[vecI[i + 2]]);

        pPNVertex[index++] = ST_PN_VERTEX(vecV[vecI[i + 0]], n);
        pPNVertex[index++] = ST_PN_VERTEX(vecV[vecI[i + 1]], n);
        pPNVertex[index++] = ST_PN_VERTEX(vecV[vecI[i + 2]], n);
        pPNVertex[index++] = ST_PN_VERTEX(vecV[vecI[i + 5]], n);
    }

    // 인덱스 버퍼 접근시 락 설정!
    m_ib->Lock(NULL, NULL, (LPVOID*)&pIndex, NULL);

    index = 0;
    for (int i = 0; i < 36; i += 6)
    {
        pIndex[i + 0] = index + 0;
        pIndex[i + 1] = index + 1;
        pIndex[i + 2] = index + 2;
        pIndex[i + 3] = index + 0;
        pIndex[i + 4] = index + 2;
        pIndex[i + 5] = index + 3;
        index += 4;
    }

    // 버텍스 정보만 저장
    for (int i = 0; i < 36; ++i)
    {
        m_vecVertex.push_back(pPNVertex[pIndex[i]].p);
    }

    m_vb->Unlock(); // 언락!!
    m_ib->Unlock(); // 언락!!
}

void cCube::SetPNTVertex(vector<D3DXVECTOR3>& vecV, vector<int>& vecI, vector<D3DXVECTOR2>* vecT)
{
    ST_PNT_VERTEX*		pPNTVertex;

    // 버텍스 버퍼 생성
    g_pD3DDevice->CreateVertexBuffer(36 * sizeof(ST_PNT_VERTEX), NULL,
        ST_PNT_VERTEX::FVF, D3DPOOL_MANAGED, &m_vb, NULL);

    // 버퍼 접근 시 락 설정!
    m_vb->Lock(NULL, NULL, (LPVOID*)&pPNTVertex, NULL);

    D3DXVECTOR3 n; // 노말값
    for (int i = 0; i < vecI.size(); i += 6)
    {
        ComputeNormal(&n, &vecV[vecI[i + 0]], &vecV[vecI[i + 1]], &vecV[vecI[i + 2]]);

        pPNTVertex[i + 0] = ST_PNT_VERTEX(vecV[vecI[i + 0]], n, (*vecT)[(i + 0) % (*vecT).size()]);
        pPNTVertex[i + 1] = ST_PNT_VERTEX(vecV[vecI[i + 1]], n, (*vecT)[(i + 1) % (*vecT).size()]);
        pPNTVertex[i + 2] = ST_PNT_VERTEX(vecV[vecI[i + 2]], n, (*vecT)[(i + 2) % (*vecT).size()]);
        pPNTVertex[i + 3] = ST_PNT_VERTEX(vecV[vecI[i + 3]], n, (*vecT)[(i + 3) % (*vecT).size()]);
        pPNTVertex[i + 4] = ST_PNT_VERTEX(vecV[vecI[i + 4]], n, (*vecT)[(i + 4) % (*vecT).size()]);
        pPNTVertex[i + 5] = ST_PNT_VERTEX(vecV[vecI[i + 5]], n, (*vecT)[(i + 5) % (*vecT).size()]);
    }

    // 버텍스 정보만 저장
    for (int i = 0; i < 36; ++i)
    {
        m_vecVertex.push_back(pPNTVertex[i].p);
    }

    m_vb->Unlock(); // 언락!!
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

	if (m_ib && m_vb)
	{
        D3DVERTEXBUFFER_DESC vbDesc;
        m_vb->GetDesc(&vbDesc);
        D3DINDEXBUFFER_DESC ibDesc;
        m_ib->GetDesc(&ibDesc);

		g_pD3DDevice->SetFVF(ST_PN_VERTEX::FVF);
        g_pD3DDevice->SetStreamSource(0, m_vb, 0, sizeof(ST_PN_VERTEX));
        g_pD3DDevice->SetIndices(m_ib);
        g_pD3DDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0,
            vbDesc.Size, 0, ibDesc.Size / 3);
		//g_pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST, m_vecPNVertex.size() / 3,
		//	&m_vecPNVertex[0], sizeof(ST_PN_VERTEX));
	}
	else if (m_vb)
	{
        D3DVERTEXBUFFER_DESC vbDesc;
        m_vb->GetDesc(&vbDesc);

		g_pD3DDevice->SetFVF(ST_PNT_VERTEX::FVF);
        g_pD3DDevice->SetStreamSource(0, m_vb, 0, sizeof(ST_PNT_VERTEX));
        g_pD3DDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, vbDesc.Size / 3);
		//g_pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST, m_vecPNTVertex.size() / 3,
		//	&m_vecPNTVertex[0], sizeof(ST_PNT_VERTEX));
	}

	for (int i = 0; i < m_vecChild.size(); ++i)
		m_vecChild[i]->Render();
}

void cCube::AddChild(cCube* pCube)
{
	m_vecChild.push_back(pCube);
}

void cCube::SetIsMoving(bool move)
{
	m_isMoving = move;

	for (int i = 0; i < m_vecChild.size(); ++i)
		m_vecChild[i]->SetIsMoving(move);
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

//// AABB 충돌
//bool cCube::Collision(D3DXVECTOR3& center)
//{
//	bool isCol = false;
//	float fRadius = BLANK_DISTANCE * 0.5f;
//
//	// 충돌 했으면
//	if (m_vCenter.x - fRadius < center.x + fRadius && m_vCenter.x + fRadius > center.x - fRadius
//		&& m_vCenter.y - fRadius < center.y + fRadius && m_vCenter.y + fRadius > center.y - fRadius
//		&& m_vCenter.z - fRadius < center.z + fRadius && m_vCenter.z + fRadius > center.z - fRadius)
//		isCol = true;
//
//	return isCol;
//}

//// 레이 충돌
//bool cCube::CollisionRay(D3DXVECTOR3 vOrigin, D3DXVECTOR3 vDir, D3DXVECTOR3 vDest)
//{
//    D3DXVECTOR3 vDist = vDest - vOrigin;
//    float fPickDist = D3DXVec3Length(&vDist);
//    bool isCol = false;
//
//    for (int i = 0; i < m_vecVertex.size(); i += 3)
//    {
//        float fDist;
//        if (D3DXIntersectTri(&m_vecVertex[i + 0], &m_vecVertex[i + 1], &m_vecVertex[i + 2], &vOrigin, &vDir,
//            NULL, NULL, &fDist))
//        {
//            // 레이체크 범위 안에 장애물이 있으면
//            if (fDist < fPickDist)
//            {
//                isCol = true;
//                break;
//            }
//        }
//    }
//
//    return isCol;
//}
//
//// 큐브와 구 충돌 -> 구가 정육면체 인것처럼 처리하고 있음...
//bool cCube::CollisionSphere(D3DXVECTOR3 vCenter, float vRadius)
//{
//    bool isCol = false;
//
//    // 충돌 했으면
//    if (m_vCenter.x - m_fRadiusX < vCenter.x + vRadius && m_vCenter.x + m_fRadiusX > vCenter.x - vRadius
//    	&& m_vCenter.y - m_fRadiusY < vCenter.y + vRadius && m_vCenter.y + m_fRadiusY > vCenter.y - vRadius
//    	&& m_vCenter.z - m_fRadiusZ < vCenter.z + vRadius && m_vCenter.z + m_fRadiusZ > vCenter.z - vRadius)
//        isCol = true;
//    
//    return isCol;
//}

// 큐브와 포지션의 충돌 여부
bool cCube::CollisionPos(D3DXVECTOR3 vPos)
{
    bool isCol = false;

    // 충돌 했으면
    if (m_vCenter.x - m_fRadiusX < vPos.x && m_vCenter.x + m_fRadiusX > vPos.x
    	&& m_vCenter.y - m_fRadiusY < vPos.y && m_vCenter.y + m_fRadiusY > vPos.y
    	&& m_vCenter.z - m_fRadiusZ < vPos.z && m_vCenter.z + m_fRadiusZ > vPos.z)
        isCol = true;
    
    return isCol;
}
