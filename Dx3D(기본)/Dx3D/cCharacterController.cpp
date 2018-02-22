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
        if (m_vPosition.y <= 0.0f && m_piMap == NULL)
        {
            m_vPosition.y = 0.0f;
            m_fCurrGravity = 0.0f;
            m_nJumpCnt = 0;
        }
    }
    
    // ������ �浹 üũ
    if (m_piMap != NULL)
    {
        // ���� �浹�� ���� �̵� ����
        bool rayMove = false;

        if (m_piMap->GetHeight(vMovePos.x, vMovePos.y, vMovePos.z))
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
            if (m_piMap->GetHeight(m_vPosition.x, vMovePos.y, m_vPosition.z)   // x, y, z �̵� �� ��ǥ�������� 
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

    // == ��ŷ �������� �̵� ==============================
    // ���������� �����Ǿ� ���� �ʰ�, ���� ��ΰ� �ִٸ�
    if (m_ePickStat == E_PICK_STAT_NONE && !m_isSetDest && !m_vecPath.empty())
    {
        m_vDestPos = m_vecPath.back();
        m_vecPath.pop_back();
        m_isSetDest = true;
    }
    
    // ���������� ���� �Ǿ����� ��, ���� ��ġ�� ���������� ���� �ʴٸ�
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

        // �̵�
        m_vPosition += vDir * fMoveSpeed;

        // ���� ���� ����
        D3DXVECTOR3 vBassDir(0, 0, 1);	

        // ���ΰ� ���ϱ�
        float cosAngle = acosf(D3DXVec3Dot(&vBassDir, &vDir));

        // ���Ȱ� ����
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

    // ���̰� ������ �浹 �ߴٸ� true / �ƴϸ� false -> PickPos�� ��ǥ�� �Ѱ���.
    D3DXVECTOR3 vPickPos;
    if (m_piMap->ColisionRay(&Ray->GetOrigin(), &Ray->GetDirection(), vPickPos))
    {
        // ��ŷ ������ ��ֹ��� �ִ� ��ġ��� ����
        if (piObstacle->IsinObstacle(vPickPos))
            return false;

        // ��ŷ ������ Ȯ������ ����
        m_vPickPos = vPickPos;

        // ��ŷ�� ������ ������ ��������, ��� �ʱ�ȭ
        m_vDestPos = m_vPosition;
        m_isSetDest = false;
        m_vecPath.clear();

        // ���� �濡 ��ֹ��� ������
        if (piObstacle && CheckFrontRay(piObstacle->GetVertex(), m_vPickPos))
        {
            m_vecPath = m_pAStar->FindPath(m_vPosition, m_vPickPos, &piObstacle->GetVertex());
            m_ePickStat = E_PICK_STAT_ASTAR;
        }
        // ���� �濡 ��ֹ��� ������
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
    D3DXVECTOR3 vDir = vDest - vOrigin; // ��ŷ�� ���������� ����
    D3DXVec3Normalize(&vDir, &vDir);

    D3DXVECTOR3 vDist = vDest - vOrigin;
    float fPickDist = D3DXVec3Length(&vDist);

    for (int i = 0; i < vecVertex.size(); i += 3)
    {
        float fDist;
        if (D3DXIntersectTri(&vecVertex[i + 0], &vecVertex[i + 1], &vecVertex[i + 2], &vOrigin, &vDir,
            NULL, NULL, &fDist))
        {
            // ����üũ ���� �ȿ� ��ֹ��� ������
            if (fDist < fPickDist)
            {
                isCol = true;
                break;
            }
        }
    }

    return isCol;
}