#include "stdafx.h"
#include "cFrame.h"

cFrame::cFrame()
	: m_pMtlTex(NULL)
{
	D3DXMatrixIdentity(&m_matWorldTM);
    D3DXMatrixIdentity(&m_matLocalTM);
}

cFrame::~cFrame()
{
}

void cFrame::AddChild(cFrame* pChild)
{
	if (pChild == NULL) return;

	m_vecChild.push_back(pChild);
}

void cFrame::Update(int nKeyFrame, D3DXMATRIXA16* pParent)
{
    D3DXMATRIXA16 matR, matT;

    CalcLocalTranslationMatrix(nKeyFrame, matT);
    CalcLocalRotationMatrix(nKeyFrame, matR);

    m_matWorldTM = matR * matT;
    
    if (pParent)
        m_matWorldTM *= *pParent;

    for each (auto p in m_vecChild)
        p->Update(nKeyFrame, &m_matWorldTM);

    /*
    월드TM 계산 : 로컬R * 로컬T
    로컬T : CalcLocalTranslationMatrix()
    로컬R : CalcLocalRotationMatrix()

    부모가 존재 할 경우
    월드TM = 월드TM * 부모TM

    모든 자식들은 재귀적으로 처리 한다.
    */
}

void cFrame::CalcLocalTM(D3DXMATRIXA16* matParent/*=NULL*/)// cFrame* pParent)
{
    m_matLocalTM = m_matWorldTM;

    // 부모 로컬 TM이 있으면
    if (matParent)
    {
        D3DXMATRIXA16 matParentInv;
        D3DXMatrixInverse(&matParentInv, NULL, matParent);
        m_matLocalTM *= matParentInv;
    }

    for each (auto p in m_vecChild)
        p->CalcLocalTM(&m_matWorldTM);

    // 로컬TM을 계산한다.
    // NODE_TM 은 부모의 로컬TM까지 적용이 된 월드 매트릭스 이다.
    // 로컬TM = NODE_TM * 부모Inv
    // 자식들까지 재귀적으로 적용
}

void cFrame::CalcLocalTranslationMatrix(IN int nKeyFrame, OUT D3DXMATRIXA16& mat)
{
    D3DXMatrixIdentity(&mat);

    // 트랙 정보가 없으면
    if (m_vecPosTrack.empty())
    {
        mat._41 = m_matLocalTM._41;
        mat._42 = m_matLocalTM._42;
        mat._43 = m_matLocalTM._43;
    }
    // 첫번째 프레임보다 작거나 같으면
    else if (nKeyFrame <= m_vecPosTrack.front().n)
    {
        mat._41 = m_vecPosTrack.front().v.x;
        mat._42 = m_vecPosTrack.front().v.y;
        mat._43 = m_vecPosTrack.front().v.z;
    }
    // 마지막 프레임보다 크거나 같으면
    else if (nKeyFrame >= m_vecPosTrack.back().n)
    {
        mat._41 = m_vecPosTrack.back().v.x;
        mat._42 = m_vecPosTrack.back().v.y;
        mat._43 = m_vecPosTrack.back().v.z;
    }
    // 보간을 해야하면
    else
    {
        // 첫번째 프레임은 위에서 검사하기 때문에 제외시킴 (i - 1)을 위해
        for (int i = 1; i < m_vecPosTrack.size(); ++i)
        {
            if (nKeyFrame < m_vecPosTrack[i].n)
            {
                // ex) fT = (17 - 10) / (20 - 10) = 0.7;
                float fT = ((float)nKeyFrame - (float)m_vecPosTrack[i - 1].n) / ((float)m_vecPosTrack[i].n - (float)m_vecPosTrack[i - 1].n);
                D3DXVECTOR3 vPos;
                D3DXVec3Lerp(&vPos, &m_vecPosTrack[i - 1].v, &m_vecPosTrack[i].v, fT);

                mat._41 = vPos.x;
                mat._42 = vPos.y;
                mat._43 = vPos.z;

                break;
            }
        }
    }

    /*
    - 키프레임이 최소 키프레임 보다 낮을 경우 최소 키프레임의 위치로 고정
    - 현재 키 프레임을 기준으로 앞뒤 두개의 키프레임 값을 보간해서 현재 값을 계산한다.
    */

    /*
    mat._41, mat._42, mat._43 매트릭스 값 설정
    트랙 정보가 없을 경우 기존 로컬TM의 값으로 그대로 셋팅

    보간을 사용하여 값 셋팅 : D3DXVec3Lerp()
    */
}

void cFrame::CalcLocalRotationMatrix(IN int nKeyFrame, OUT D3DXMATRIXA16& mat)
{
    D3DXMatrixIdentity(&mat);

    // 트랙 정보가 없으면
    if (m_vecRotTrack.empty())
    {
        mat = m_matLocalTM;
        mat._41 = 0.0f;
        mat._42 = 0.0f;
        mat._43 = 0.0f;
    }
    // 첫번째 프레임보다 작거나 같으면
    else if (nKeyFrame <= m_vecRotTrack.front().n)
    {
        D3DXMatrixRotationQuaternion(&mat, &m_vecRotTrack.front().q);
    }
    // 마지막 프레임보다 크거나 같으면
    else if (nKeyFrame >= m_vecRotTrack.back().n)
    {
        D3DXMatrixRotationQuaternion(&mat, &m_vecRotTrack.back().q);
    }
    // 보간을 해야하면
    else
    {
        // 첫번째 프레임은 위에서 검사하기 때문에 제외시킴 (i - 1)을 위해
        for (int i = 1; i < m_vecRotTrack.size(); ++i)
        {
            if (nKeyFrame < m_vecRotTrack[i].n)
            {
                // ex) fT = (17 - 10) / (20 - 10) = 0.7;
                float fT = ((float)nKeyFrame - (float)m_vecRotTrack[i - 1].n) / ((float)m_vecRotTrack[i].n - (float)m_vecRotTrack[i - 1].n);
                D3DXQUATERNION qRot;
                D3DXQuaternionSlerp(&qRot, &m_vecRotTrack[i - 1].q, &m_vecRotTrack[i].q, fT);

                D3DXMatrixRotationQuaternion(&mat, &qRot);

                break;
            }
        }
    }

    /*
    - 키프레임이 최소 키프레임 보다 낮을 경우 최소 키프레임의 위치로 고정
    - 현재 키 프레임을 기준으로 앞뒤 두개의 키프레임 값을 보간해서 현재 값을 계산한다.
    */

    /*
    트랙 정보가 없을 경우 기존 로컬TM의 값으로 그대로 셋팅
    로컬TM에서 이동 값만 제외
    */

    // D3DXMatrixRotationQuaternion(&mat, 퀀터니언 값)
    // D3DXQuaternionSlerp() 퀀터니언 보간(회전 보간)
}

void cFrame::Render()
{
    if (m_pMtlTex)
    {
        g_pD3DDevice->SetTransform(D3DTS_WORLD, &m_matWorldTM);
        g_pD3DDevice->SetTexture(0, m_pMtlTex->GetTexture());
        g_pD3DDevice->SetMaterial(m_pMtlTex->GetMtl());
        g_pD3DDevice->SetFVF(ST_PNT_VERTEX::FVF);
        g_pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST,
            m_vecPNTVertex.size() / 3, &m_vecPNTVertex[0], sizeof(ST_PNT_VERTEX));
    }

    for each (auto p in m_vecChild)
        p->Render();
}