#include "stdafx.h"
#include "cNode.h"


cNode::cNode()
    : m_vLocation(0, 0, 0)
    , m_pVia(NULL)
    , F(0)
    , G(D3DX_16F_MAX)
    , H(D3DX_16F_MAX)
    , S(E_NONE)
{
}


cNode::~cNode()
{
    for each (auto iter in m_vecEdgeInfos)
        SAFE_DELETE(iter);
}

void cNode::AddEdge(cNode * pNode)
{
    // ���� ��� �߰�
    ST_EdgeInfo* stEdgeNode = new ST_EdgeInfo;
    stEdgeNode->pEdgeNode = pNode;
    D3DXVECTOR3 dist = this->m_vLocation - pNode->m_vLocation;
    stEdgeNode->fEdgeCost = D3DXVec3Length(&dist);
    //stEdgeNode->fEdgeCost = 1.0f;
    m_vecEdgeInfos.push_back(stEdgeNode);

    // ���� ��� ��ġ ���� (���� ����� ����)
    m_vecLines.push_back(this->m_vLocation);
    m_vecLines.push_back(pNode->m_vLocation);
}
