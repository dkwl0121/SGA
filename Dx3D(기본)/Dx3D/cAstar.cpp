#include "stdafx.h"
#include "cAstar.h"
#include "cNode.h"
#include "cRay.h"

cAstar::cAstar()
    : m_pMesh(NULL)
    , m_vecObsVertex(NULL)
{
}

cAstar::~cAstar()
{
    SAFE_RELEASE(m_pMesh);
}

void cAstar::Setup(vector<D3DXVECTOR3> vecVertex)
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

void cAstar::Render()
{
    g_pD3DDevice->SetTexture(0, NULL);
    for each (auto iter in m_vecpNode)
    {
        if (iter->S == E_NONE)
            g_pD3DDevice->SetMaterial(&WHITE_MTRL);
        else if (iter->S == E_OPEN)
            g_pD3DDevice->SetMaterial(&GREEN_MTRL);
        else if (iter->S == E_CLOSE)
            g_pD3DDevice->SetMaterial(&RED_MTRL);
        else if (iter->S == E_USING)
            g_pD3DDevice->SetMaterial(&YELLOW_MTRL);
        else if (iter->S == E_BLOCK)
            g_pD3DDevice->SetMaterial(&BLUE_MTRL);
        D3DXMATRIXA16 matW;
        D3DXMatrixTranslation(&matW, iter->m_vLocation.x, iter->m_vLocation.y, iter->m_vLocation.z);
        g_pD3DDevice->SetTransform(D3DTS_WORLD, &matW);
        m_pMesh->DrawSubset(0);
        
        D3DXMatrixIdentity(&matW);
        g_pD3DDevice->SetTransform(D3DTS_WORLD, &matW);
        g_pD3DDevice->SetMaterial(&WHITE_MTRL);
        g_pD3DDevice->SetFVF(D3DFVF_XYZ);
        g_pD3DDevice->DrawPrimitiveUP(D3DPT_LINELIST, iter->GetLines().size() / 2,
            &iter->GetLines()[0], sizeof(D3DXVECTOR3));
    }
}

// ���̽�Ÿ ���� �Լ�
vector<D3DXVECTOR3> cAstar::FindPath(D3DXVECTOR3 vFrom, D3DXVECTOR3 vTo, vector<D3DXVECTOR3>* vecVertex/*=NULL*/)
{
    if (vecVertex)
        m_vecObsVertex = vecVertex;

    // �ʱ�ȭ
    Reset();

    // ����, ���� ��� ã��
    int nFromIndex = FindNodeIndex(vFrom);
    int nToIndex = FindNodeIndex(vTo);
    
    // ����, ���� ��带 ã�� ���ߴٸ� ��� ����
    if (nFromIndex == -1 || nToIndex == -1)
        return vector<D3DXVECTOR3>();

    m_vecpNode[nFromIndex]->m_pVia = m_vecpNode[nFromIndex];
    m_vecpNode[nFromIndex]->G = 0.0f;
    m_vecpNode[nFromIndex]->H = GetManhattanDistance(m_vecpNode[nFromIndex], m_vecpNode[nToIndex]);

    cNode* currNode = m_vecpNode[nFromIndex];

    do
    {
        // ��� ����
        Extend(currNode, m_vecpNode[nToIndex]);

        // ���⼭ ���� ��尡 ���� ��� ��δ� ����

        if (m_mapMinFNode.empty()) return vector<D3DXVECTOR3>();

        // F���� ���� ª�� ���� ������ �˻�
        currNode = m_mapMinFNode.begin()->second;

    } while (m_vecpNode[nToIndex]->S != E_CLOSE); // �������� Ŭ�ο��� ������ �׸�

    // ���� ��� ���
    return SetPath(m_vecpNode[nToIndex], vFrom, vTo);
}

// ��� ��� �ʱ�ȭ
void cAstar::Reset()
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

// ���� ���⿡ �ִ� ��ֹ� ����
bool cAstar::IsBlocked(D3DXVECTOR3 vFrom, D3DXVECTOR3 vTo)
{
    // ���� ��ġ���� ������ ���������� ���� ����
    cRay ray;
    ray.m_vOrigin = vFrom;
    ray.m_vDirection = vTo - vFrom;
    float fPickDist = D3DXVec3Length(&ray.m_vDirection);	// ������������ ���� �Ÿ�
    D3DXVec3Normalize(&ray.m_vDirection, &ray.m_vDirection);

    bool isBlocked = false;

    if (m_vecObsVertex)
    {
        for (int i = 0; i < m_vecObsVertex->size(); i += 3)
        {
            float fDist;
            if (D3DXIntersectTri(&(*m_vecObsVertex)[i + 0], &(*m_vecObsVertex)[i + 1], &(*m_vecObsVertex)[i + 2], &ray.m_vOrigin, &ray.m_vDirection,
                NULL, NULL, &fDist))
            {
                // ����üũ ���� �ȿ� ��ֹ��� ������
                if (fDist < fPickDist)
                {
                    isBlocked = true;
                    break;
                }
            }
        }
    }

    return isBlocked;
}

// ���� ����� ��ġ�� ��� ã��
int cAstar::FindNodeIndex(D3DXVECTOR3 vPos)
{
    float fMinDist = D3DX_16F_MAX;
    int nIndex = -1;
    for (int i = 0; i < m_vecpNode.size(); ++i)
    {
        // ��ֹ��� ������ �ִ� ���� �н�
        if (m_vecObsVertex && IsBlocked(vPos, m_vecpNode[i]->m_vLocation)) continue;

        float fdist = fabsf(m_vecpNode[i]->m_vLocation.x - vPos.x)
            + fabsf(m_vecpNode[i]->m_vLocation.y - vPos.y)
            + fabsf(m_vecpNode[i]->m_vLocation.z - vPos.z);

        // ��ġ�� ���� ����� �н�
        if (fdist < D3DX_16F_EPSILON) continue;

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
float cAstar::GetManhattanDistance(cNode* n1, cNode* n2)
{
    return fabsf(n1->m_vLocation.x - n2->m_vLocation.x)
        + fabsf(n1->m_vLocation.y - n2->m_vLocation.y)
        + fabsf(n1->m_vLocation.z - n2->m_vLocation.z);
}

// ��� ����(Ȯ��) �Լ�
void cAstar::Extend(cNode* currNode, cNode* destNode)
{
    currNode->F = currNode->G + currNode->H;
    currNode->S = E_CLOSE;

    m_mapMinFNode.clear();

    vector<ST_EdgeInfo*>& vecEdgeNode = currNode->GetEdgeInfos();
    for (int i = 0; i < vecEdgeNode.size(); ++i)
    {
        cNode* edgeNode = vecEdgeNode[i]->pEdgeNode;

        // E_NONE�����̰� ��ֹ��� �� �� ���� ����
        if (m_vecObsVertex && edgeNode->S == E_NONE && IsBlocked(currNode->m_vLocation, edgeNode->m_vLocation))
            edgeNode->S = E_BLOCK;

        // Ŭ�ο��� �ưų� ��ֹ��� ���� �� �� ���� ���� �н�
        if (edgeNode->S == E_CLOSE || edgeNode->S == E_BLOCK) continue;

        float fNewG = currNode->G + vecEdgeNode[i]->fEdgeCost;

        if (edgeNode->S == E_OPEN)
        {
            if (fNewG < edgeNode->G)
            {
                edgeNode->G = fNewG;
                edgeNode->F = edgeNode->G + edgeNode->H;
                edgeNode->m_pVia = currNode;
            }
        }
        else
        {
            edgeNode->S = E_OPEN;
            edgeNode->H = GetManhattanDistance(edgeNode, destNode);
            edgeNode->G = fNewG;
            edgeNode->F = edgeNode->G + edgeNode->H;
            edgeNode->m_pVia = currNode;
        }
    }

    // ���� ���� ��� �ֱ� 
    for (int i = 0; i < m_vecpNode.size(); ++i)
    {
        if (m_vecpNode[i]->S == E_OPEN)
            m_mapMinFNode[m_vecpNode[i]->F] = m_vecpNode[i];
    }
}

// ���� ��� ���� �Լ�
vector<D3DXVECTOR3> cAstar::SetPath(cNode* destNode, D3DXVECTOR3 vStartPos, D3DXVECTOR3 vPickPos)
{
    vector<D3DXVECTOR3> vecPath;

    vecPath.push_back(vPickPos);

    cNode* temp = destNode;
    temp->S = E_USING;
    vecPath.push_back(temp->m_vLocation);

    do
    {
        temp = temp->m_pVia;
        temp->S = E_USING;
        vecPath.push_back(temp->m_vLocation);
    } while (temp->G != 0.0f);
    
    // ��ŸƮ �����Ǳ��� �ְ� ����ȭ�� �� �� pop_back();
    vecPath.push_back(vStartPos);
    // ����ȭ
    OptimizationPath(0, vecPath.size() - 1, vecPath);
    // ��ŸƮ ������ ����
    vecPath.pop_back();
    return vecPath;
}

// ����ȭ (��� ����)
void cAstar::OptimizationPath(int nFirst, int nLast, OUT vector<D3DXVECTOR3>& vecPath)
{
    if (nFirst >= nLast) return;

    // ��δ� �ڿ��� �ռ����� �����Ǿ� ����
    for (int i = nLast; i > nFirst + 1; --i)
    {
        // �� ��� ���̿� ��ֹ��� ��.��.��!
        if (!IsBlocked(vecPath[i], vecPath[nFirst]))
        {
            for (auto iter = vecPath.begin() + nFirst + 1; iter != vecPath.begin() + i; )
            {
                iter = vecPath.erase(iter);
                --i;        //erase �ϴ� ��ŭ �ε����� �ٱ� ������!
                --nLast;    //erase �ϴ� ��ŭ �ε����� �ٱ� ������!
            }
            break;
        }
    }

    // ��δ� �ڿ��� �ռ����� �����Ǿ� ����
    for (int i = nFirst; i < nLast - 1; ++i)
    {
        // �� ��� ���̿� ��ֹ��� ��.��.��!
        if (!IsBlocked(vecPath[i], vecPath[nLast]))
        {
            for (auto iter = vecPath.begin() + i + 1; iter != vecPath.begin() + nLast; )
            {
                iter = vecPath.erase(iter);
                --nLast;    //erase �ϴ� ��ŭ �ε����� �ٱ� ������!
            }
            break;
        }
    }

    OptimizationPath(nFirst + 1, nLast - 1, vecPath);
}
