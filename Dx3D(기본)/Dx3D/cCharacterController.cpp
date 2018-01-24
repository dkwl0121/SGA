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
	m_isMoving = IsControl; // 컨트롤이 불가능하면 무브 펄스!
    D3DXVECTOR3 vMovePos(m_vPosition);
    D3DXMATRIXA16 matRY;
    D3DXMatrixRotationY(&matRY, m_fRotY);

	// 캐릭터 컨트롤을 할 수 있으면
	if (IsControl)
	{
		// == 회 전 ========================================
		if (g_pKeyManager->isStayKeyDown('A'))
			m_fRotY -= 0.1f;
		else if (g_pKeyManager->isStayKeyDown('D'))
			m_fRotY += 0.1f;

        D3DXMatrixRotationY(&matRY, m_fRotY);

		// == 이 동 ========================================
		D3DXVECTOR3 vDir(0, 0, 1);							// 이동 방향 설정, 기본 이동 방향인 z축 양수 방향을 설정하고
		D3DXVec3TransformNormal(&vDir, &vDir, &matRY);	// 큐브의 회전 값을 이동 방향에도 적용 시켜서 이동 방향을 계산한다.
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

		// == 점 프 ==========================================
		if (g_pKeyManager->isOnceKeyDown(VK_SPACE) && m_nJumpCnt < JUMP_CNT_MAX)
		{
			++m_nJumpCnt;
			m_fCurrGravity = 0.0f;
		}
	}

	// == 이하는 캐릭터 컨트롤에 상관없이 무조건 실행 =============
    // 점프중이면
	if (m_nJumpCnt > 0)
	{
        // 점프값은 항상 적용
		m_fCurrGravity += GRAVITY;
        m_vPosition.y = m_vPosition.y + JUMP_POWER - m_fCurrGravity;

		// 지형체크를 하지 않아도 되고, 착지 했을 경우
		if (m_vPosition.y <= 0.0f && pMap == NULL)
		{
            m_vPosition.y = 0.0f;
			m_fCurrGravity = 0.0f;
			m_nJumpCnt = 0;
		}
	}
    
    // 지형물 충돌 체크
    if (pMap != NULL)
    {
        // 지형 충돌로 인한 이동 여부
        bool rayMove = false;

        if (pMap->GetHeight(vMovePos.x, vMovePos.y, vMovePos.z))
        {
            // 점프중이면
            if (m_nJumpCnt > 0)
            {
                // 지형에 닿았고, 착륙중이면 점프 끝
                if (m_vPosition.y < vMovePos.y && JUMP_POWER - m_fCurrGravity < 0.0f)
                {
                    m_fCurrGravity = 0.0f;
                    m_nJumpCnt = 0;
                    m_vPosition.y = vMovePos.y;
                }
            }
            // 점프중이 아니면
            else
            {
                m_vPosition.y = vMovePos.y;
            }
            // 이동위치로 포지션이동
            m_vPosition.x = vMovePos.x;
            m_vPosition.z = vMovePos.z;
            rayMove = true;
        }

        // 점프 중에 땅을 파고 들어갔을 때 처리
        if (m_nJumpCnt > 0 && m_isMoving && !rayMove)
        {                                     
            if (pMap->GetHeight(m_vPosition.x, vMovePos.y, m_vPosition.z)   // x, y, z 이동 전 좌표기준으로 
                && m_vPosition.y < vMovePos.y)                              // 땅을 파고 들어갔다면 점프 끝
            {
                m_fCurrGravity = 0.0f;
                m_nJumpCnt = 0;
                m_vPosition.y = vMovePos.y;
            }
        }
    }
    // 지형물이 없으면
    else
    {
        m_vPosition = vMovePos;
    }
	
	D3DXMATRIXA16 matT;
	D3DXMatrixTranslation(&matT, m_vPosition.x, m_vPosition.y, m_vPosition.z);
	
	m_matWorld = matRY * matT;
}
