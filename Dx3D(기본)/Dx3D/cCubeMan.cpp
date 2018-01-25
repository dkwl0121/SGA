#include "stdafx.h"
#include "cCubeMan.h"
#include "cCube.h"
#include "cCharacterController.h"

cCubeMan::cCubeMan()
	: m_pRootCube(NULL)
	, m_pController(NULL)
	, m_vPos(0, 0, 0)
	, m_vDest(0, 0, 0)
	, m_fMoveSpeed(0.05f)
{
	D3DXMatrixIdentity(&m_matR);
	D3DXMatrixIdentity(&m_matWorld);
}


cCubeMan::~cCubeMan()
{
	SAFE_RELEASE(m_pRootCube);
}

void cCubeMan::Setup()
{
	g_pTextureManager->AddTexture("batman", "images/batman.png");

	D3DXMATRIXA16 matS, matT, matSRT;

	// �ؽ��� ���� ����
	vector<D3DXVECTOR2> vecT;
	vector<tagXY> vecXY;

	// �Ʒ� �� ���� ���� ������ ����
	SetTextureVertex(vecT, tagXY(7, 5), tagXY(2, 1));
	SetTextureVertex(vecT, tagXY(5, 5), tagXY(2, 1));
	SetTextureVertex(vecT, tagXY(7, 8), tagXY(1, 3));
	SetTextureVertex(vecT, tagXY(5, 8), tagXY(2, 3));
	SetTextureVertex(vecT, tagXY(4, 8), tagXY(1, 3));
	SetTextureVertex(vecT, tagXY(8, 8), tagXY(2, 3));

	// ť��� ���� (��Ʈ)
	D3DXMatrixScaling(&matS, 1.0f, 1.0f, 0.5f);
	D3DXMatrixTranslation(&matT, 0.0f, 3.0f, 0.0f);
	matSRT = matS * matT;
	m_pRootCube = new cCube;
    g_pAutoReleasePool->AddObject(m_pRootCube);
	m_pRootCube->Setup(&vecT, "batman", WHITE_MTRL, &matSRT);
	vecT.clear();

	cCube* pCube;

	// �Ʒ� �� ���� ���� ������ ����
	SetTextureVertex(vecT, tagXY(4, 2), tagXY(2, 2));
	SetTextureVertex(vecT, tagXY(2, 2), tagXY(2, 2));
	SetTextureVertex(vecT, tagXY(4, 4), tagXY(2, 2));
	SetTextureVertex(vecT, tagXY(2, 4), tagXY(2, 2));
	SetTextureVertex(vecT, tagXY(0, 4), tagXY(2, 2));
	SetTextureVertex(vecT, tagXY(6, 4), tagXY(2, 2));

	// ť��� �Ӹ�
	D3DXMatrixScaling(&matS, 0.8f, 0.8f, 0.8f);
	D3DXMatrixTranslation(&matT, 0.0f, 4.8f, 0.0f);
	matSRT = matS * matT;
	pCube = new cCube;
    g_pAutoReleasePool->AddObject(pCube);
	pCube->Setup(&vecT, "batman", WHITE_MTRL, &matSRT);
	vecT.clear();
	pCube->SetRotSpeedY(0.05f);
	m_pRootCube->AddChild(pCube);

	// �Ʒ� �� ���� ���� ������ ����
	SetTextureVertex(vecT, tagXY(12, 5), tagXY(1, 1));
	SetTextureVertex(vecT, tagXY(11, 5), tagXY(1, 1));
	SetTextureVertex(vecT, tagXY(12, 8), tagXY(1, 3));
	SetTextureVertex(vecT, tagXY(11, 8), tagXY(1, 3));
	SetTextureVertex(vecT, tagXY(10, 8), tagXY(1, 3));
	SetTextureVertex(vecT, tagXY(13, 8), tagXY(1, 3));

	// ť��� ����
	D3DXMATRIXA16 matR;
	D3DXMatrixRotationY(&matR, D3DX_PI);
	D3DXMatrixScaling(&matS, 0.5f, 1.0f, 0.5f);
	D3DXMatrixTranslation(&matT, -1.5f, 3.0f, 0.0f);
	matSRT = matS * matR * matT;
	pCube = new cCube;
    g_pAutoReleasePool->AddObject(pCube);
	pCube->Setup(&vecT, "batman", WHITE_MTRL, &matSRT);
	pCube->SetRotSpeedX(0.05f);
	pCube->SetLocalPos(D3DXVECTOR3(0.0f, -4.0f, 0.0f));
	m_pRootCube->AddChild(pCube);

	// ť��� ������
	D3DXMatrixScaling(&matS, 0.5f, 1.0f, 0.5f);
	D3DXMatrixTranslation(&matT, 1.5f, 3.0f, 0.0f);
	matSRT = matS * matT;
	pCube = new cCube;
    g_pAutoReleasePool->AddObject(pCube);
	pCube->Setup(&vecT, "batman", WHITE_MTRL, &matSRT);
	vecT.clear();
	pCube->SetRotSpeedX(-0.05f);
	pCube->SetLocalPos(D3DXVECTOR3(0.0f, -4.0f, 0.0f));
	m_pRootCube->AddChild(pCube);

	// ����
	D3DXMatrixScaling(&matS, 0.1f, 0.1f, 2.0f);
	D3DXMatrixTranslation(&matT, 1.5f, 2.0f, 1.0f);
	matSRT = matS * matT;
	cCube* pWeapon = new cCube;
    g_pAutoReleasePool->AddObject(pWeapon);
	pWeapon->Setup(NULL, "null", WHITE_MTRL, &matSRT);
	pCube->AddChild(pWeapon);

	// �Ʒ� �� ���� ���� ������ ����
	SetTextureVertex(vecT, tagXY(2, 5), tagXY(1, 1));
	SetTextureVertex(vecT, tagXY(1, 5), tagXY(1, 1));
	SetTextureVertex(vecT, tagXY(2, 8), tagXY(1, 3));
	SetTextureVertex(vecT, tagXY(1, 8), tagXY(1, 3));
	SetTextureVertex(vecT, tagXY(0, 8), tagXY(1, 3));
	SetTextureVertex(vecT, tagXY(3, 8), tagXY(1, 3));

	// ť��� ���� �ٸ�
	D3DXMatrixScaling(&matS, 0.5f, 1.0f, 0.5f);
	D3DXMatrixTranslation(&matT, -0.5f, 1.0f, 0.0f);
	matSRT = matS * matT;
	pCube = new cCube;
    g_pAutoReleasePool->AddObject(pCube);
	pCube->Setup(&vecT, "batman", WHITE_MTRL, &matSRT);
	pCube->SetRotSpeedX(-0.05f);
	pCube->SetLocalPos(D3DXVECTOR3(0.0f, -2.0f, 0.0f));
	m_pRootCube->AddChild(pCube);

	// ť��� ������ �ٸ�
	D3DXMatrixScaling(&matS, 0.5f, 1.0f, 0.5f);
	D3DXMatrixTranslation(&matT, 0.5f, 1.0f, 0.0f);
	matSRT = matS * matT;
	pCube = new cCube;
    g_pAutoReleasePool->AddObject(pCube);
	pCube->Setup(&vecT, "batman", WHITE_MTRL, &matSRT);
	vecT.clear();
	pCube->SetRotSpeedX(0.05f);
	pCube->SetLocalPos(D3DXVECTOR3(0.0f, -2.0f, 0.0f));
	m_pRootCube->AddChild(pCube);
}

void cCubeMan::SetTextureVertex(vector<D3DXVECTOR2>& vecT, tagXY vXY, tagXY sXY)
{
	// 0 1 2     0 2 3
	vecT.push_back(D3DXVECTOR2(vXY.x / 16.0f, vXY.y / 8.0f));
	vecT.push_back(D3DXVECTOR2(vXY.x / 16.0f, (vXY.y - sXY.y) / 8.0f));
	vecT.push_back(D3DXVECTOR2((vXY.x + sXY.x) / 16.0f, (vXY.y - sXY.y) / 8.0f));

	vecT.push_back(D3DXVECTOR2(vXY.x / 16.0f, vXY.y / 8.0f));
	vecT.push_back(D3DXVECTOR2((vXY.x + sXY.x) / 16.0f, (vXY.y - sXY.y) / 8.0f));
	vecT.push_back(D3DXVECTOR2((vXY.x + sXY.x) / 16.0f, vXY.y / 8.0f));
}

void cCubeMan::Update()
{
	if (m_pRootCube)
	{
		/* ��Ʈ�ѷ� ���۽� �̵� Ȯ�� */
		m_pRootCube->SetIsMoving(m_pController->GetIsMoving());
		D3DXMATRIXA16* mat = m_pController->GetWorldTM();
		m_pRootCube->Update(mat);

		/* ������ ���� �� ĳ���� �̵� ���ǵ带 ����Ͽ� �������� �̵� */
		//D3DXMATRIXA16 matR;
		//D3DXMatrixIdentity(&matR);
		// �������� �ڵ� �̵�
		//if (m_vPos != m_vDest)
		//{
		//	D3DXVECTOR3 vDir = m_vDest - m_vPos;
		//	float fMoveSpeed = D3DXVec3Length(&vDir);
		//
		//	if (D3DXVec3Length(&vDir) > m_fMoveSpeed)
		//		fMoveSpeed = m_fMoveSpeed;
		//
		//	D3DXVec3Normalize(&vDir, &vDir);
		//
		//	m_vPos += vDir * fMoveSpeed;
		//	m_pRootCube->SetIsMoving(true);
		//
		//	// �̵� �������� �ٶ󺸰� ����� (�� ��Ʈ������ Ư�� Ȱ��)
		//	D3DXMatrixLookAtLH(&m_matR, &D3DXVECTOR3(0, 0, 0), &vDir, &D3DXVECTOR3(0, 1, 0));
		//	D3DXMatrixTranspose(&m_matR, &m_matR);
		//	// == D3DXMatrixInverse()
		//	// : ȸ�� ������ �ִ� ��Ʈ������ ��� Ʈ�������� ��Ű�� �ι��� ��Ʈ������ ������ ����� ���´�.
		//}

		/* �ܺο��� ���� �� �̵� ��Ʈ������ ����Ͽ� �̵� */
		//m_pRootCube->Update(&m_matWorld);
		//m_pRootCube->SetIsMoving(true);

		// ����
		//D3DLIGHT9 stLight = g_pLightManager->GetLight("cubeman");
		//stLight.Direction = *m_pController->GetDirection();
		//stLight.Position = *m_pController->GetPosition();
		//stLight.Position.y += 5.0f;
		//g_pLightManager->SetLight("cubeman", stLight);
		//if (g_pKeyManager->isToggleKey(VK_F1))
		//{
		//	static bool on = false;
		//	on = !on;
		//	g_pLightManager->EnableLight("cubeman", on);
		//}
	}
}

void cCubeMan::Render()
{
	if (m_pRootCube)
	{
		m_pRootCube->Render();
	}
}
