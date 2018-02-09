#pragma once
#include "cObject.h"

class iMap;
class cRay;
class cAstar;
class iObstacle;

#define JUMP_POWER		1.5f
#define JUMP_CNT_MAX	2

class cCharacterController : public cObject
{
private:
	D3DXVECTOR3		        m_vPosition;
    D3DXVECTOR3             m_vDestPos;

	float			        m_fMoveSpeed;
	float			        m_fRotY;
	D3DXVECTOR3		        m_vDirection;

	D3DXMATRIXA16	        m_matWorld;

	bool			        m_isMoving;
    bool                    m_isSetDest;
	int				        m_nJumpCnt;
	float			        m_fCurrGravity;

    iMap*                   m_piMap;
    cAstar*                 m_pAStar;
    vector<D3DXVECTOR3>     m_vecPath;

private:

    bool CheckFrontRay(vector<D3DXVECTOR3> vecVertex, D3DXVECTOR3 vDest);

public:
	cCharacterController();
	~cCharacterController();
    
    void Setup(iMap* pMap);
	void Update(bool IsControl);
    void Render();

    void SetPick(cRay* Ray, iObstacle* piObstacle = NULL);

	// ����
	D3DXVECTOR3* GetPosition() { return &m_vPosition; }
	D3DXVECTOR3* GetDirection() { return &m_vDirection; }
	D3DXMATRIXA16* GetWorldTM() { return &m_matWorld; }
	bool GetIsMoving() { return m_isMoving; }

	// ����
	void SetIsMoving(bool move) { m_isMoving = false; }
};

