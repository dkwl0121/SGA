#include "stdafx.h"
#include "cCharacterController.h"
#include "iMap.h"
#include "cRay.h"
#include "cAstar.h"
#include "iObstacle.h"

cCharacterController::cCharacterController()
	: m_vPosition(0, 0, 0)
    , m_vDestPos(m_vPosition)
	, m_fRotY(0)
	, m_fMoveSpeed(0.5f)
	, m_isMoving(false)
    , m_isSetDest(false)
	, m_vDirection(0, 0, 1)
	, m_nJumpCnt(0)
	, m_fCurrGravity(0.0f)
    , m_piMap(NULL)
    , m_pAStar(NULL)
    , m_ePickStat(E_PICK_STAT_NONE)
    , m_vPickPos(0, 0, 0)
{
	D3DXMatrixIdentity(&m_matWorld);
}


cCharacterController::~cCharacterController()
{
}

void cCharacterController::Setup(iMap* pMap)
{
    m_piMap = pMap;

    m_pAStar = new cAstar;
    g_pAutoReleasePool->AddObject(m_pAStar);
    m_pAStar->Setup(m_piMap->GetVertex());
}

void cCharacterController::Update(bool IsControl)
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
        if (m_vPosition.y <= 0.0f && m_piMap == NULL)
        {
            m_vPosition.y = 0.0f;
            m_fCurrGravity = 0.0f;
            m_nJumpCnt = 0;
        }
    }
    
    // 지형물 충돌 체크
    if (m_piMap != NULL)
    {
        // 지형 충돌로 인한 이동 여부
        bool rayMove = false;

        if (m_piMap->GetHeight(vMovePos.x, vMovePos.y, vMovePos.z))
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
            if (m_piMap->GetHeight(m_vPosition.x, vMovePos.y, m_vPosition.z)   // x, y, z 이동 전 좌표기준으로 
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

    // == 픽킹 목적지로 이동 ==============================
    // 도착지점이 설정되어 있지 않고, 남은 경로가 있다면
    if (m_ePickStat == E_PICK_STAT_NONE && !m_isSetDest && !m_vecPath.empty())
    {
        m_vDestPos = m_vecPath.back();
        m_vecPath.pop_back();
        m_isSetDest = true;
    }
    
    // 도착지점이 설정 되었으면 고, 현재 위치가 도착지점과 같지 않다면
    if (m_ePickStat == E_PICK_STAT_NONE
        && m_isSetDest
        && (fabsf(m_vPosition.x - m_vDestPos.x) > D3DX_16F_EPSILON
        || fabsf(m_vPosition.y - m_vDestPos.y) > D3DX_16F_EPSILON
        || fabsf(m_vPosition.z - m_vDestPos.z) > D3DX_16F_EPSILON))
    {
        m_isMoving = true;
        D3DXMATRIXA16 matR;
        D3DXMatrixIdentity(&matR);
        D3DXVECTOR3 vDir = m_vDestPos - m_vPosition;
        float fMoveSpeed = D3DXVec3Length(&vDir);

        if (fMoveSpeed > m_fMoveSpeed)
            fMoveSpeed = m_fMoveSpeed;

        D3DXVec3Normalize(&vDir, &vDir);

        // 이동
        m_vPosition += vDir * fMoveSpeed;

        // 기준 방향 벡터
        D3DXVECTOR3 vBassDir(0, 0, 1);	

        // 끼인각 구하기
        float cosAngle = acosf(D3DXVec3Dot(&vBassDir, &vDir));

        // 라디안값 설정
        m_fRotY = (vBassDir.z * vDir.x - vBassDir.x * vDir.z > 0.0f) ? cosAngle : -cosAngle;

        D3DXMatrixRotationY(&matRY, m_fRotY);

        if ((fabsf(m_vPosition.x - m_vDestPos.x) < D3DX_16F_EPSILON
            && fabsf(m_vPosition.y - m_vDestPos.y) < D3DX_16F_EPSILON
            && fabsf(m_vPosition.z - m_vDestPos.z) < D3DX_16F_EPSILON))
            m_isSetDest = false;
    }

    D3DXMATRIXA16 matT;
    D3DXMatrixTranslation(&matT, m_vPosition.x, m_vPosition.y, m_vPosition.z);

    m_matWorld = matRY * matT;
}

void cCharacterController::Render()
{
    if (m_pAStar)
        m_pAStar->Render();
}

bool cCharacterController::CheckPick(cRay* Ray, iObstacle* piObstacle/*= NULL*/)
{
    if (!m_piMap) return false;

    // 레이가 지형과 충돌 했다면 true / 아니면 false -> PickPos에 좌표값 넘겨줌.
    D3DXVECTOR3 vPickPos;
    if (m_piMap->ColisionRay(&Ray->GetOrigin(), &Ray->GetDirection(), vPickPos))
    {
        // 픽킹 지점이 장애물이 있는 위치라면 리턴
        if (piObstacle->IsinObstacle(vPickPos))
            return false;

        // 픽킹 지점이 확정나면 대입
        m_vPickPos = vPickPos;

        // 픽킹을 했으면 설정된 도착지점, 경로 초기화
        m_vDestPos = m_vPosition;
        m_isSetDest = false;
        m_vecPath.clear();

        // 가는 길에 장애물이 있으면
        if (piObstacle && CheckFrontRay(piObstacle->GetVertex(), m_vPickPos))
        {
            m_vecPath = m_pAStar->FindPath(m_vPosition, m_vPickPos, &piObstacle->GetVertex());
            m_ePickStat = E_PICK_STAT_ASTAR;
        }
        // 가는 길에 장애물이 없으면
        else
        {
            m_pAStar->Reset();
            m_vDestPos = m_vPickPos;
            m_isSetDest = true;
            m_ePickStat = E_PICK_STAT_NOASTAR;
        }
    }

    return true;
}

bool cCharacterController::CheckFrontRay(vector<D3DXVECTOR3> vecVertex, D3DXVECTOR3 vDest)
{
    bool isCol = false;
    D3DXVECTOR3 vOrigin(m_vPosition.x, m_vPosition.y + 5.0f, m_vPosition.z);
    D3DXVECTOR3 vDir = vDest - vOrigin; // 픽킹된 지역으로의 방향
    D3DXVec3Normalize(&vDir, &vDir);

    D3DXVECTOR3 vDist = vDest - vOrigin;
    float fPickDist = D3DXVec3Length(&vDist);

    for (int i = 0; i < vecVertex.size(); i += 3)
    {
        float fDist;
        if (D3DXIntersectTri(&vecVertex[i + 0], &vecVertex[i + 1], &vecVertex[i + 2], &vOrigin, &vDir,
            NULL, NULL, &fDist))
        {
            // 레이체크 범위 안에 장애물이 있으면
            if (fDist < fPickDist)
            {
                isCol = true;
                break;
            }
        }
    }

    return isCol;
}