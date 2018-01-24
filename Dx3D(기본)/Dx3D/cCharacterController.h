#pragma once

class iMap;

#define JUMP_POWER		1.5f
#define JUMP_CNT_MAX	2

class cCharacterController
{
private:
	D3DXVECTOR3		m_vPosition;
	float			m_fMoveSpeed;
	float			m_fRotY;
	D3DXVECTOR3		m_vDirection;

	D3DXMATRIXA16	m_matWorld;

	bool			m_isMoving;
	int				m_nJumpCnt;
	float			m_fCurrGravity;

public:
	cCharacterController();
	~cCharacterController();

	void Update(bool IsControl, iMap* pMap = NULL);
	
	// ╟ыем
	D3DXVECTOR3* GetPosition() { return &m_vPosition; }
	D3DXVECTOR3* GetDirection() { return &m_vDirection; }
	D3DXMATRIXA16* GetWorldTM() { return &m_matWorld; }
	bool GetIsMoving() { return m_isMoving; }

	// ╪бем
	void SetIsMoving(bool move) { m_isMoving = false; }
};

