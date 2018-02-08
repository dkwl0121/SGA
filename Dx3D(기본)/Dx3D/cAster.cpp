#include "stdafx.h"
#include "cAster.h"
#include "cNode.h"
#include "cRay.h"

cAster::cAster()
    : m_pMesh(NULL)
{
}

cAster::~cAster()
{
    SAFE_RELEASE(m_pMesh);
}

void cAster::Setup(vector<D3DXVECTOR3> vecVertex)
{
    // �Ž� ����, �� ����
    D3DXCreateSphere(g_pD3DDevice, NODE_RADIUS, 10, 10, &m_pMesh, NULL);

    int nX = 0;
    int nZ = 0;

    // ��� ��ǥ ����
    for (int z = 0; z < 257; z += 15)
    {
        nX = 0;
        for (int x = 0; x < 257; x += 15)
        {
            int nIndex = (z * 257) + x;

            cNode* node = new cNode;
            g_pAutoReleasePool->AddObject(node);
            node->m_vLocation = vecVertex[nIndex];
            m_vecpNode.push_back(node);
            ++nX;
        }
        ++nZ;
    }

    // ���� ��� ����
    for (int z = 0; z < nZ; ++z)
    {
        for (int x = 0; x < nX; ++x)
        {
            int nIndex = (z * nZ) + x;

            // ž ���
            if (z < nZ - 1)
                m_vecpNode[nIndex]->AddEdge(m_vecpNode[nIndex + nZ]);
            // ���� ���
            if (z > 0)
                m_vecpNode[nIndex]->AddEdge(m_vecpNode[nIndex - nZ]);
            // ���� ���
            if (x > 0)
                m_vecpNode[nIndex]->AddEdge(m_vecpNode[nIndex - 1]);
            // ���� ���
            if (x < nZ - 1)
                m_vecpNode[nIndex]->AddEdge(m_vecpNode[nIndex + 1]);
        }
    }
}

void cAster::Render()
{
    g_pD3DDevice->SetTexture(0, NULL);
    for each (auto iter in m_vecpNode)
    {
        if (iter->S == E_NONE)
            g_pD3DDevice->SetMaterial(&WHITE_MTRL);
        else if (iter->S == E_OPEN)
            g_pD3DDevice->SetMaterial(&GREEN_MTRL);
        else if (iter->S == E_CLOSE)
            g_pD3DDevice->SetMaterial(&YELLOW_MTRL);
        else if (iter->S == E_USING)
            g_pD3DDevice->SetMaterial(&RED_MTRL);
        D3DXMATRIXA16 matW;
        D3DXMatrixTranslation(&matW, iter->m_vLocation.x, iter->m_vLocation.y, iter->m_vLocation.z);
        g_pD3DDevice->SetTransform(D3DTS_WORLD, &matW);
        m_pMesh->DrawSubset(0);
        
        D3DXMatrixIdentity(&matW);
        g_pD3DDevice->SetTransform(D3DTS_WORLD, &matW);
        g_pD3DDevice->SetFVF(D3DFVF_XYZ);
        g_pD3DDevice->DrawPrimitiveUP(D3DPT_LINELIST, iter->GetLines().size() / 2,
            &iter->GetLines()[0], sizeof(D3DXVECTOR3));
    }
}

// ���̽�Ÿ ���� �Լ�
void cAster::FindPath(D3DXVECTOR3 vFrom, D3DXVECTOR3 vTo)
{
    // �ʱ�ȭ
    ResetNode();

    // ���� ��� ã��
    int nFromIndex = FindNodeIndex(vFrom);

    // ���� ��� ã��
    int nToIndex = FindNodeIndex(vTo);

    m_vecpNode[nFromIndex]->m_pVia = m_vecpNode[nFromIndex];
    m_vecpNode[nFromIndex]->G = 0.0f;
    m_vecpNode[nFromIndex]->H = GetManhattanDistance(m_vecpNode[nFromIndex], m_vecpNode[nToIndex]);

    cNode* currNode = m_vecpNode[nFromIndex];

    do
    {
       // ��� ����
       Extend(currNode, m_vecpNode[nToIndex]);

        // F���� ���� ª�� ������ �˻�
        currNode = m_mapMinFNode.begin()->second;

    } while (m_vecpNode[nToIndex]->S != E_CLOSE); // �������� Ŭ�ο��� ������ �׸�

    // ���� ��� ���
    SetPath(m_vecpNode[nToIndex], vTo);
}

// ��� ��� �ʱ�ȭ
void cAster::ResetNode()
{
    for each (auto iter in m_vecpNode)
    {
        iter->m_pVia = NULL;
        iter->F = 0;
        iter->G = D3DX_16F_MAX;
        iter->H = D3DX_16F_MAX;
        iter->S = E_NONE;
    }
}

// ���� ����� ��ġ�� ��� ã��
int cAster::FindNodeIndex(D3DXVECTOR3 vPos)
{
    float fMinDist = D3DX_16F_MAX;
    int nIndex = 0;
    for (int i = 0; i < m_vecpNode.size(); ++i)
    {
        float fdist = fabsf(m_vecpNode[i]->m_vLocation.x - vPos.x)
            + fabsf(m_vecpNode[i]->m_vLocation.y - vPos.y)
            + fabsf(m_vecpNode[i]->m_vLocation.z - vPos.z);
        // �Ÿ��� ���� ����� �����
        if (fdist < fMinDist)
        {
            fMinDist = fdist;
            nIndex = i;
        }
    }
    return nIndex;
}

// H�� ���
float cAster::GetManhattanDistance(cNode* n1, cNode* n2)
{
    return fabsf(n1->m_vLocation.x - n2->m_vLocation.x)
        + fabsf(n1->m_vLocation.y - n2->m_vLocation.y)
        + fabsf(n1->m_vLocation.z - n2->m_vLocation.z);
}

// ��� ����(Ȯ��) �Լ�
void cAster::Extend(cNode* currNode, cNode* destNode)
{
    currNode->F = currNode->G + currNode->H;
    currNode->S = E_CLOSE;

    m_mapMinFNode.clear();

    vector<ST_EdgeInfo*>& vecEdgeNode = currNode->GetEdgeInfos();
    for (int i = 0; i < vecEdgeNode.size(); ++i)
    {
        cNode* edgeNode = vecEdgeNode[i]->pEdgeNode;

        if (edgeNode->S == E_CLOSE) continue;

        edgeNode->S = E_OPEN;
        float newG = currNode->G + vecEdgeNode[i]->fEdgeCost;
        if (newG < edgeNode->G)
        {
            edgeNode->H = GetManhattanDistance(edgeNode, destNode);
            edgeNode->G = newG;
            edgeNode->m_pVia = currNode;
        }
        edgeNode->F = edgeNode->G + edgeNode->H;
    }

    for (int i = 0; i < m_vecpNode.size(); ++i)
    {
        if (m_vecpNode[i]->S == E_OPEN)
            m_mapMinFNode[m_vecpNode[i]->F] = m_vecpNode[i];
    }
}

// ���� ��� ���� �Լ�
void cAster::SetPath(cNode* destNode, D3DXVECTOR3 vPickPos)
{
    m_vecPath.clear();

    m_vecPath.push_back(vPickPos);

    cNode* temp = destNode;
    temp->S = E_USING;
    m_vecPath.push_back(temp->m_vLocation);

    do
    {
        temp = temp->m_pVia;
        temp->S = E_USING;
        m_vecPath.push_back(temp->m_vLocation);
    } while (temp->G != 0.0f);
}
