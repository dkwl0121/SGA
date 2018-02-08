#pragma once
#include "cObject.h"

class cNode;
class cRay;

#define NODE_RADIUS  1.0f

class cAster : public cObject
{
private:
    LPD3DXMESH              m_pMesh;

    vector<cNode*>          m_vecpNode;

    map<float, cNode*>      m_mapMinFNode;

    vector<D3DXVECTOR3>     m_vecPath;

private:
    void    ResetNode();                                        // ��� ��� �ʱ�ȭ
    int     FindNodeIndex(D3DXVECTOR3 vPos);                    // ���� ����� ��ġ�� ��� ã��
    float	GetManhattanDistance(cNode* n1, cNode* n2);	        // H�� ��� �Լ�
    void	Extend(cNode* currNode, cNode* destNode);			// ��� ����(Ȯ��) �Լ�
    void	SetPath(cNode* destNode, D3DXVECTOR3 vPickPos);	    // ���� ��� ���� �Լ�

public:
    cAster();
    ~cAster();

    void Setup(vector<D3DXVECTOR3> vecVertex);
    void Render();

    void FindPath(D3DXVECTOR3 vTo, D3DXVECTOR3 vFrom);		// ���̽�Ÿ ���� �Լ�

    // == ���� ==
    vector<D3DXVECTOR3>& GetPath() { return m_vecPath; }
};

