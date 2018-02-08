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
    void    ResetNode();                                        // 모든 노드 초기화
    int     FindNodeIndex(D3DXVECTOR3 vPos);                    // 가장 가까운 위치의 노드 찾기
    float	GetManhattanDistance(cNode* n1, cNode* n2);	        // H값 계산 함수
    void	Extend(cNode* currNode, cNode* destNode);			// 노드 갱신(확장) 함수
    void	SetPath(cNode* destNode, D3DXVECTOR3 vPickPos);	    // 최종 경로 설정 함수

public:
    cAster();
    ~cAster();

    void Setup(vector<D3DXVECTOR3> vecVertex);
    void Render();

    void FindPath(D3DXVECTOR3 vTo, D3DXVECTOR3 vFrom);		// 에이스타 메인 함수

    // == 겟터 ==
    vector<D3DXVECTOR3>& GetPath() { return m_vecPath; }
};

