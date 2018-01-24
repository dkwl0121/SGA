#include "stdafx.h"
#include "cCharacterController.h"
#include "iMap.h"

cCharacterController::cCharacterController()
	: m_vPosition(0, 0, 0)
	, m_fRotY(0)
	, m_fMoveSpeed(0.5f)
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

void cCharacterController::Update(bool IsControl, iMap* pMap/*= NULL*/)
{
	m_isMoving = IsControl; // ��Ʈ���� �Ұ����ϸ� ���� �޽�!
    D3DXVECTOR3 vMovePos(m_vPosition);
    D3DXMATRIXA16 matRY;
    D3DXMatrixRotationY(&matRY, m_fRotY);

	// ĳ���� ��Ʈ���� �� �� ������
	if (IsControl)
	{
		// == ȸ �� ========================================
		if (g_pKeyManager->isStayKeyDown('A'))
			m_fRotY -= 0.1f;
		else if (g_pKeyManager->isStayKeyDown('D'))
			m_fRotY += 0.1f;

        D3DXMatrixRotationY(&matRY, m_fRotY);

		// == �� �� ========================================
		D3DXVECTOR3 vDir(0, 0, 1);							// �̵� ���� ����, �⺻ �̵� ������ z�� ��� ������ �����ϰ�
		D3DXVec3TransformNormal(&vDir, &vDir, &matRY);	// ť���� ȸ�� ���� �̵� ���⿡�� ���� ���Ѽ� �̵� ������ ����Ѵ�.
		m_vDirection = vDir;

		if (g_pKeyManager->isStayKeyDown('W'))
		{
            vMovePos = vMovePos + vDir * m_fMoveSpeed;
			m_isMoving = true;
		}
		else if (g_pKeyManager->isStayKeyDown('S'))
		{
            vMovePos = vMovePos - vDir * m_fMoveSpeed;
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

	// == ���ϴ� ĳ���� ��Ʈ�ѿ� ������� ������ ���� =============
    // �������̸�
	if (m_nJumpCnt > 0)
	{
        // �������� �׻� ����
		m_fCurrGravity += GRAVITY;
        m_vPosition.y = m_vPosition.y + JUMP_POWER - m_fCurrGravity;

		// ����üũ�� ���� �ʾƵ� �ǰ�, ���� ���� ���
		if (m_vPosition.y <= 0.0f && pMap == NULL)
		{
            m_vPosition.y = 0.0f;
			m_fCurrGravity = 0.0f;
			m_nJumpCnt = 0;
		}
	}
    
    // ������ �浹 üũ
    if (pMap != NULL)
    {
        // ���� �浹�� ���� �̵� ����
        bool rayMove = false;

        if (pMap->GetHeight(vMovePos.x, vMovePos.y, vMovePos.z))
        {
            // �������̸�
            if (m_nJumpCnt > 0)
            {
                // ������ ��Ұ�, �������̸� ���� ��
                if (m_vPosition.y < vMovePos.y && JUMP_POWER - m_fCurrGravity < 0.0f)
                {
                    m_fCurrGravity = 0.0f;
                    m_nJumpCnt = 0;
                    m_vPosition.y = vMovePos.y;
                }
            }
            // �������� �ƴϸ�
            else
            {
                m_vPosition.y = vMovePos.y;
            }
            // �̵���ġ�� �������̵�
            m_vPosition.x = vMovePos.x;
            m_vPosition.z = vMovePos.z;
            rayMove = true;
        }

        // ���� �߿� ���� �İ� ���� �� ó��
        if (m_nJumpCnt > 0 && m_isMoving && !rayMove)
        {                                     
            if (pMap->GetHeight(m_vPosition.x, vMovePos.y, m_vPosition.z)   // x, y, z �̵� �� ��ǥ�������� 
                && m_vPosition.y < vMovePos.y)                              // ���� �İ� ���ٸ� ���� ��
            {
                m_fCurrGravity = 0.0f;
                m_nJumpCnt = 0;
                m_vPosition.y = vMovePos.y;
            }
        }
    }
    // �������� ������
    else
    {
        m_vPosition = vMovePos;
    }
	
	D3DXMATRIXA16 matT;
	D3DXMatrixTranslation(&matT, m_vPosition.x, m_vPosition.y, m_vPosition.z);
	
	m_matWorld = matRY * matT;
}
