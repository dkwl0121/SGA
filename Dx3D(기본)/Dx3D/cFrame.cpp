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
    ����TM ��� : ����R * ����T
    ����T : CalcLocalTranslationMatrix()
    ����R : CalcLocalRotationMatrix()

    �θ� ���� �� ���
    ����TM = ����TM * �θ�TM

    ��� �ڽĵ��� ��������� ó�� �Ѵ�.
    */
}

void cFrame::CalcLocalTM(D3DXMATRIXA16* matParent/*=NULL*/)// cFrame* pParent)
{
    m_matLocalTM = m_matWorldTM;

    // �θ� ���� TM�� ������
    if (matParent)
    {
        D3DXMATRIXA16 matParentInv;
        D3DXMatrixInverse(&matParentInv, NULL, matParent);
        m_matLocalTM *= matParentInv;
    }

    for each (auto p in m_vecChild)
        p->CalcLocalTM(&m_matWorldTM);

    // ����TM�� ����Ѵ�.
    // NODE_TM �� �θ��� ����TM���� ������ �� ���� ��Ʈ���� �̴�.
    // ����TM = NODE_TM * �θ�Inv
    // �ڽĵ���� ��������� ����
}

void cFrame::CalcLocalTranslationMatrix(IN int nKeyFrame, OUT D3DXMATRIXA16& mat)
{
    D3DXMatrixIdentity(&mat);

    // Ʈ�� ������ ������
    if (m_vecPosTrack.empty())
    {
        mat._41 = m_matLocalTM._41;
        mat._42 = m_matLocalTM._42;
        mat._43 = m_matLocalTM._43;
    }
    // ù��° �����Ӻ��� �۰ų� ������
    else if (nKeyFrame <= m_vecPosTrack.front().n)
    {
        mat._41 = m_vecPosTrack.front().v.x;
        mat._42 = m_vecPosTrack.front().v.y;
        mat._43 = m_vecPosTrack.front().v.z;
    }
    // ������ �����Ӻ��� ũ�ų� ������
    else if (nKeyFrame >= m_vecPosTrack.back().n)
    {
        mat._41 = m_vecPosTrack.back().v.x;
        mat._42 = m_vecPosTrack.back().v.y;
        mat._43 = m_vecPosTrack.back().v.z;
    }
    // ������ �ؾ��ϸ�
    else
    {
        // ù��° �������� ������ �˻��ϱ� ������ ���ܽ�Ŵ (i - 1)�� ����
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
    - Ű�������� �ּ� Ű������ ���� ���� ��� �ּ� Ű�������� ��ġ�� ����
    - ���� Ű �������� �������� �յ� �ΰ��� Ű������ ���� �����ؼ� ���� ���� ����Ѵ�.
    */

    /*
    mat._41, mat._42, mat._43 ��Ʈ���� �� ����
    Ʈ�� ������ ���� ��� ���� ����TM�� ������ �״�� ����

    ������ ����Ͽ� �� ���� : D3DXVec3Lerp()
    */
}

void cFrame::CalcLocalRotationMatrix(IN int nKeyFrame, OUT D3DXMATRIXA16& mat)
{
    D3DXMatrixIdentity(&mat);

    // Ʈ�� ������ ������
    if (m_vecRotTrack.empty())
    {
        mat = m_matLocalTM;
        mat._41 = 0.0f;
        mat._42 = 0.0f;
        mat._43 = 0.0f;
    }
    // ù��° �����Ӻ��� �۰ų� ������
    else if (nKeyFrame <= m_vecRotTrack.front().n)
    {
        D3DXMatrixRotationQuaternion(&mat, &m_vecRotTrack.front().q);
    }
    // ������ �����Ӻ��� ũ�ų� ������
    else if (nKeyFrame >= m_vecRotTrack.back().n)
    {
        D3DXMatrixRotationQuaternion(&mat, &m_vecRotTrack.back().q);
    }
    // ������ �ؾ��ϸ�
    else
    {
        // ù��° �������� ������ �˻��ϱ� ������ ���ܽ�Ŵ (i - 1)�� ����
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
    - Ű�������� �ּ� Ű������ ���� ���� ��� �ּ� Ű�������� ��ġ�� ����
    - ���� Ű �������� �������� �յ� �ΰ��� Ű������ ���� �����ؼ� ���� ���� ����Ѵ�.
    */

    /*
    Ʈ�� ������ ���� ��� ���� ����TM�� ������ �״�� ����
    ����TM���� �̵� ���� ����
    */

    // D3DXMatrixRotationQuaternion(&mat, ���ʹϾ� ��)
    // D3DXQuaternionSlerp() ���ʹϾ� ����(ȸ�� ����)
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