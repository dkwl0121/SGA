#pragma once
#include "cObject.h"

class cNode;

struct ST_EdgeInfo
{
    cNode*	pEdgeNode;
	float	fEdgeCost;
};

enum eNodeState { E_NONE, E_OPEN, E_CLOSE, E_USING };  // E_USING == ���������� ���õ� ���

class cNode : public cObject
{
private:
	// ���� ��忡 ���� �Ǿ� �ִ� ������� ����
	SYNTHESIZE_PASS_BY_REF(vector<ST_EdgeInfo*>, m_vecEdgeInfos, EdgeInfos);
	// ���� �� ������ ���� �߱� ���� ��ġ ����(������ġ, ���� �����ġ �ݺ�)
	SYNTHESIZE_PASS_BY_REF(vector<D3DXVECTOR3>, m_vecLines, Lines);

public:
	D3DXVECTOR3 m_vLocation;	// ����� ��ġ ��
	//int			m_nIndex;		// ���� ����� �ε��� ��ȣ
    cNode*			m_pVia;			// ��� ������ ������������ �ִܰŸ� ���� ���

	float		F;				// �� ��� (G + H)
	float		G;				// ���� ��� ������ ���� ���
	float		H;				// ���� ��忡�� ������������ ���� ��� (����ź ���Ͻ�)
	eNodeState	S;				// ���� ����� ����

public:
	cNode();
	~cNode();

	void AddEdge(cNode* pNode);
};

