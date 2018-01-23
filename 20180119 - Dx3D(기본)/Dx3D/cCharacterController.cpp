#include "stdafx.h"
#include "cCharacterController.h"


cCharacterController::cCharacterController()
	: m_vPosition(0, 0, 0)
	, m_fRotY(0)
	, m_fMoveSpeed(0.1f)
	, m_isMoving(false)
	, m_vDirection(0, 0, 1)
	, m_nJumpCnt(0)
	, m_fCurrGravity(0.0f)
{
	D3DXMatrixIdentity(&m_matWorld);
}


cCharacterController::~cCharacterController()
{
}

void cCharacterController::Update(bool IsControl)
{
	m_isMoving = IsControl; // 컨트롤이 불가능하면 무브 펄스!
	D3DXMatrixIdentity(&m_matWorld);

	// 캐릭터 컨트롤을 할 수 있으면
	if (IsControl)
	{
		// == 회 전 ========================================
		if (g_pKeyManager->isStayKeyDown('A'))
			m_fRotY -= 0.1f;
		else if (g_pKeyManager->isStayKeyDown('D'))
			m_fRotY += 0.1f;

		D3DXMATRIXA16 matRY;
		D3DXMatrixRotationY(&matRY, m_fRotY);

		m_matWorld *= matRY;

		// == 이 동 ========================================
		D3DXVECTOR3 vDir(0, 0, 1);							// 이동 방향 설정, 기본 이동 방향인 z축 양수 방향을 설정하고
		D3DXVec3TransformNormal(&vDir, &vDir, &matRY);	// 큐브의 회전 값을 이동 방향에도 적용 시켜서 이동 방향을 계산한다.
		m_vDirection = vDir;

		if (g_pKeyManager->isStayKeyDown('W'))
		{
			m_vPosition = m_vPosition + vDir * 0.1f;
			m_isMoving = true;
		}
		else if (g_pKeyManager->isStayKeyDown('S'))
		{
			m_vPosition = m_vPosition - vDir * 0.1f;
			m_isMoving = true;
		}
		else
		{
			m_isMoving = false;
		}

		// == 점 프 ==========================================
		if (g_pKeyManager->isOnceKeyDown(VK_SPACE) && m_nJumpCnt < JUMP_CNT_MAX)
		{
			++m_nJumpCnt;
			m_fCurrGravity = 0.0f;
		}
	}

	// == 이하는 캐릭터 컨트롤에 상관없이 실행 =============
	if (m_nJumpCnt > 0)
	{
		m_fCurrGravity += GRAVITY;
		m_vPosition.y += JUMP_POWER - m_fCurrGravity;

		// 착지 했을 경우
		if (m_vPosition.y <= 0.0f)
		{
			m_vPosition.y = 0.0f;
			m_fCurrGravity = 0.0f;
			m_nJumpCnt = 0;
		}
	}
	
	D3DXMATRIXA16 matT;
	D3DXMatrixTranslation(&matT, m_vPosition.x, m_vPosition.y, m_vPosition.z);
	
	m_matWorld *= matT;
}
