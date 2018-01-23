#include "stdafx.h"
#include "cMainGame.h"
#include "cCamera.h"
#include "cGrid.h"
#include "cCharacterController.h"
#include "cCubeMan.h"


cMainGame::cMainGame()
	: m_pCamera(NULL)
	, m_pGrid(NULL)
	, m_pController(NULL)
	, m_pCubeMan(NULL)
	, m_IsTarget(true)
{
}


cMainGame::~cMainGame()
{
	SAFE_DELETE(m_pCamera);
	SAFE_DELETE(m_pController);
	SAFE_DELETE(m_pGrid);
	SAFE_DELETE(m_pCubeMan);

	g_pTextureManager->Destroy();
	g_pDeviceManager->Destroy();
}

void cMainGame::Setup()
{
	g_pSoundManager->AddSound("bgm", "sounds/bgm.mp3", true, true);
	g_pSoundManager->Play("bgm");

	//json j;
	//j["Player"]["name"] = 2;
	//j["Player"]["level"] = 1;
	//j["Player"]["die"] = true;
	//ifstream i;
	//i.open("text.json", ios_base::in);
	//i.close();
	
	srand(time(NULL));
	rand();

	// ���� ����Ʈ
	D3DXVECTOR3 dir(1.0f, -1.0f, 0.0f);
	D3DXVec3Normalize(&dir, &dir);
	D3DLIGHT9 stLight = InitDirectional(&dir, &WHITE);
	g_pLightManager->AddLight("main", stLight);

	g_pKeyManager->Setup();

	m_pCamera = new cCamera;
	m_pCamera->Setup();

	m_pController = new cCharacterController;

	m_pGrid = new cGrid;
	m_pGrid->Setup();

	m_pCubeMan = new cCubeMan;
	m_pCubeMan->Setup();
	m_pCubeMan->SetController(m_pController);
}

void cMainGame::Update()
{
	g_pTimeManager->Update();

	m_IsTarget = g_pKeyManager->isToggleKey(VK_TAB);

	if (m_pController)
		m_pController->Update(m_IsTarget);

	if (m_pCamera)
	{
		// ī�޶� ����ٳ�� �ϴ� Ÿ���� ������
		if (m_IsTarget)
		{
			m_pCamera->Update(m_pController->GetPosition());
		}
		// ī�޶� ������ �ؾ� �ϸ�
		else
		{
			m_pCamera->Update();
		}
	}

	if (m_pCubeMan)
		m_pCubeMan->Update();
}

void cMainGame::Render()
{
	g_pD3DDevice->Clear(NULL,
		NULL,
		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
		D3DCOLOR_XRGB(47, 121, 112),
		1.0f, 0);

	g_pD3DDevice->BeginScene();

	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, false);

	if (m_pGrid)
		m_pGrid->Render();

	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, true);

	if (m_pCubeMan)
		m_pCubeMan->Render();

	g_pD3DDevice->EndScene();

	g_pD3DDevice->Present(0, 0, 0, 0);
}