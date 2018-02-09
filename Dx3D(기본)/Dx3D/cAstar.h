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

    vector<D3DXVECTOR3>*    m_vecObsVertex; // 장애물 좌표

private:
    bool                IsBlocked(D3DXVECTOR3 vFrom, D3DXVECTOR3 vTo);                              // 가는 방향에 있는 장애물 여부
    int                 FindNodeIndex(D3DXVECTOR3 vPos);                                            // 가장 가까운 위치의 노드 찾기
    float	            GetManhattanDistance(cNode* n1, cNode* n2);	                                // H값 계산 함수
    void	            Extend(cNode* currNode, cNode* destNode);			                        // 노드 갱신(확장) 함수
    vector<D3DXVECTOR3>	SetPath(cNode* destNode, D3DXVECTOR3 vStartPos, D3DXVECTOR3 vPickPos);	    // 최종 경로 설정 함수
    void                OptimizationPath(int nFirst, int nLast, OUT vector<D3DXVECTOR3>& vecPath);  // 최적화

public:
    cAstar();
    ~cAstar();

    void Setup(vector<D3DXVECTOR3> vecVertex);
    void Render();

    void                Reset();                                            // 초기화

    vector<D3DXVECTOR3> FindPath(D3DXVECTOR3 vTo, D3DXVECTOR3 vFrom, vector<D3DXVECTOR3>* vecVertex = NULL);		 // 에이스타 메인 함수
};

