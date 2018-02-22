#pragma once
#include "cObject.h"

class iMap;
class cRay;
class cAstar;
class iObstacle;

#define JUMP_POWER		1.5f
#define JUMP_CNT_MAX	2

enum E_PICK_STAT {
    E_PICK_STAT_NONE,
    E_PICK_STAT_NOASTAR,
    E_PICK_STAT_ASTAR
};

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

    D3DXVECTOR3             m_vPickPos;
    E_PICK_STAT             m_ePickStat;

private:

    bool CheckFrontRay(vector<D3DXVECTOR3> vecVertex, D3DXVECTOR3 vDest);

public:
	cCharacterController();
	~cCharacterController();
    
    void Setup(iMap* pMap);
	void Update(bool IsControl);
    void Render();

    bool CheckPick(cRay* Ray, iObstacle* piObstacle = NULL);

	// ╟ыем
	D3DXVECTOR3* GetPosition() { return &m_vPosition; }
	D3DXVECTOR3* GetDirection() { return &m_vDirection; }
	D3DXMATRIXA16* GetWorldTM() { return &m_matWorld; }
	bool GetIsMoving() { return m_isMoving; }
    E_PICK_STAT GetPickStat() { return m_ePickStat; }
    D3DXVECTOR3 GetPickPos() { return m_vPickPos; }
    int GetPathCnt() { return m_vecPath.size(); }

	// ╪бем
	void SetIsMoving(bool move) { m_isMoving = false; }
    void SetPickStat(E_PICK_STAT eStat) { m_ePickStat = eStat; }
};

