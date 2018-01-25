#pragma once

#include "cObject.h"

class cCube;
class cCharacterController;

struct tagXY
{
	float x;
	float y;

	tagXY(float _x, float _y) { x = _x; y = _y; }
};

enum E_CUBEMAN_PARTS_TYPE
{
	E_CUBEMAN_PARTS_TYPE_BODY,		// ��
	E_CUBEMAN_PARTS_TYPE_HEAD,		// �Ӹ�
	E_CUBEMAN_PARTS_TYPE_ARML,		// ��(��)
	E_CUBEMAN_PARTS_TYPE_ARMR,		// ��(��)
	E_CUBEMAN_PARTS_TYPE_LEGL,		// �ٸ�(��)
	E_CUBEMAN_PARTS_TYPE_LEGR,		// 
	E_CUBEMAN_PARTS_TYPE_WEAPON		// 
};

class cCubeMan : public cObject
{
private:
	cCube*					m_pRootCube;	// ť�� ������Ʈ Ŭ����
	cCharacterController*	m_pController;	// �÷��̾� ���� Ŭ����

	D3DXMATRIXA16			m_matR;
	D3DXMATRIXA16			m_matWorld;		// ���� ���������� �̵� ��Ʈ����
	D3DXVECTOR3				m_vPos;			// ���� ť��� ��ġ
	D3DXVECTOR3				m_vDest;		// �̵� ������
	float					m_fMoveSpeed;	// �̵� �ӵ�

	void SetTextureVertex(vector<D3DXVECTOR2>& vecT, tagXY vXY, tagXY sXY);

public:
	cCubeMan();
	~cCubeMan();

	void Setup();
	void Update();
	void Render();

	// ����
	void SetController(cCharacterController* c) { m_pController = c; }
	void SetDest(D3DXVECTOR3& dest) { m_vDest = dest; }

	bool IsMoving() { return m_vPos != m_vDest; }
};

