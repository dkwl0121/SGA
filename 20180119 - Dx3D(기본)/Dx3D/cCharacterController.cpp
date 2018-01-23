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
	m_isMoving = IsControl; // ��Ʈ���� �Ұ����ϸ� ���� �޽�!
	D3DXMatrixIdentity(&m_matWorld);

	// ĳ���� ��Ʈ���� �� �� ������
	if (IsControl)
	{
		// == ȸ �� ========================================
		if (g_pKeyManager->isStayKeyDown('A'))
			m_fRotY -= 0.1f;
		else if (g_pKeyManager->isStayKeyDown('D'))
			m_fRotY += 0.1f;

		D3DXMATRIXA16 matRY;
		D3DXMatrixRotationY(&matRY, m_fRotY);

		m_matWorld *= matRY;

		// == �� �� ========================================
		D3DXVECTOR3 vDir(0, 0, 1);							// �̵� ���� ����, �⺻ �̵� ������ z�� ��� ������ �����ϰ�
		D3DXVec3TransformNormal(&vDir, &vDir, &matRY);	// ť���� ȸ�� ���� �̵� ���⿡�� ���� ���Ѽ� �̵� ������ ����Ѵ�.
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

		// == �� �� ==========================================
		if (g_pKeyManager->isOnceKeyDown(VK_SPACE) && m_nJumpCnt < JUMP_CNT_MAX)
		{
			++m_nJumpCnt;
			m_fCurrGravity = 0.0f;
		}
	}

	// == ���ϴ� ĳ���� ��Ʈ�ѿ� ������� ���� =============
	if (m_nJumpCnt > 0)
	{
		m_fCurrGravity += GRAVITY;
		m_vPosition.y += JUMP_POWER - m_fCurrGravity;

		// ���� ���� ���
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
