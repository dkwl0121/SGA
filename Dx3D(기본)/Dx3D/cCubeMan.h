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
	E_CUBEMAN_PARTS_TYPE_BODY,		// 몸
	E_CUBEMAN_PARTS_TYPE_HEAD,		// 머리
	E_CUBEMAN_PARTS_TYPE_ARML,		// 팔(좌)
	E_CUBEMAN_PARTS_TYPE_ARMR,		// 팔(우)
	E_CUBEMAN_PARTS_TYPE_LEGL,		// 다리(좌)
	E_CUBEMAN_PARTS_TYPE_LEGR,		// 
	E_CUBEMAN_PARTS_TYPE_WEAPON		// 
};

class cCubeMan : public cObject
{
private:
	cCube*					m_pRootCube;	// 큐브 오브젝트 클래스
	cCharacterController*	m_pController;	// 플레이어 조작 클래스

	D3DXMATRIXA16			m_matR;
	D3DXMATRIXA16			m_matWorld;		// 월드 공간에서의 이동 매트릭스
	D3DXVECTOR3				m_vPos;			// 현재 큐브맨 위치
	D3DXVECTOR3				m_vDest;		// 이동 목적지
	float					m_fMoveSpeed;	// 이동 속도

	void SetTextureVertex(vector<D3DXVECTOR2>& vecT, tagXY vXY, tagXY sXY);

public:
	cCubeMan();
	~cCubeMan();

	void Setup();
	void Update();
	void Render();

	// 셋터
	void SetController(cCharacterController* c) { m_pController = c; }
	void SetDest(D3DXVECTOR3& dest) { m_vDest = dest; }

	bool IsMoving() { return m_vPos != m_vDest; }
};

