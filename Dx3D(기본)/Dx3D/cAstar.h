#pragma once
#include "cObject.h"

class cNode;

#define NODE_RADIUS  1.0f

class cAstar : public cObject
{
private:
    LPD3DXMESH              m_pMesh;

    vector<cNode*>          m_vecpNode;

    map<float, cNode*>      m_mapMinFNode;

    vector<D3DXVECTOR3>*    m_vecObsVertex; // ��ֹ� ��ǥ

private:
    bool                IsBlocked(D3DXVECTOR3 vFrom, D3DXVECTOR3 vTo);                              // ���� ���⿡ �ִ� ��ֹ� ����
    int                 FindNodeIndex(D3DXVECTOR3 vPos);                                            // ���� ����� ��ġ�� ��� ã��
    float	            GetManhattanDistance(cNode* n1, cNode* n2);	                                // H�� ��� �Լ�
    void	            Extend(cNode* currNode, cNode* destNode);			                        // ��� ����(Ȯ��) �Լ�
    vector<D3DXVECTOR3>	SetPath(cNode* destNode, D3DXVECTOR3 vStartPos, D3DXVECTOR3 vPickPos);	    // ���� ��� ���� �Լ�
    void                OptimizationPath(int nFirst, int nLast, OUT vector<D3DXVECTOR3>& vecPath);  // ����ȭ

public:
    cAstar();
    ~cAstar();

    void Setup(vector<D3DXVECTOR3> vecVertex);
    void Render();

    void                Reset();                                            // �ʱ�ȭ

    vector<D3DXVECTOR3> FindPath(D3DXVECTOR3 vTo, D3DXVECTOR3 vFrom, vector<D3DXVECTOR3>* vecVertex = NULL);		 // ���̽�Ÿ ���� �Լ�
};

