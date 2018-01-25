#include "stdafx.h"
#include "cMainGame.h"
#include "cCamera.h"
#include "cGrid.h"
#include "cCharacterController.h"
#include "cCubeMan.h"
#include "cGroup.h"
#include "cObjMap.h"
#include "cObjLoader.h"

cMainGame::cMainGame()
	: m_pCamera(NULL)
	, m_pGrid(NULL)
	, m_pController(NULL)
	, m_pCubeMan(NULL)
    , m_pObjMap(NULL)
	, m_IsTarget(true)
{
}


cMainGame::~cMainGame()
{
	SAFE_DELETE(m_pCamera);
	SAFE_DELETE(m_pController);
	SAFE_DELETE(m_pGrid);
	SAFE_DELETE(m_pCubeMan);

    g_pLightManager->Destroy();
	g_pTextureManager->Destroy();

    // 오브젝트 빼는 작업 부터 하기!!!
    g_pAutoReleasePool->Drain();
    g_pObjectManager->Destory();

	g_pDeviceManager->Destroy();
}

void cMainGame::Setup()
{
	//json j;
	//j["Player"]["name"] = 2;
	//j["Player"]["level"] = 1;
	//j["Player"]["die"] = true;
	//ifstream i;
	//i.open("text.json", ios_base::in);
	//i.close();
	
	srand(time(NULL));
	rand();

	// 메인 라이트
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
    g_pAutoReleasePool->AddObject(m_pCubeMan);
	m_pCubeMan->Setup();
	m_pCubeMan->SetController(m_pController);

    D3DXMATRIXA16 mat, matS, matR, matT;
    D3DXMatrixIdentity(&mat);
    D3DXMatrixScaling(&matS, 0.05f, 0.05f, 0.05f);
    D3DXMatrixRotationX(&matR, -D3DX_PI * 0.5f);
    D3DXMatrixTranslation(&matT, 0, 0, -3);
    mat = matS * matR * matT;

    cObjLoader* pObjLoader = new cObjLoader;
    pObjLoader->Load("obj/map_surface.obj", &mat, m_vecGroup);
    SAFE_RELEASE(pObjLoader);

    // == 지형 충돌처리 그룹 ==
    m_pObjMap = new cObjMap;
    g_pAutoReleasePool->AddObject(m_pObjMap);
    m_pObjMap->Load("obj/map_surface.obj", &mat);
}

void cMainGame::Update()
{
	g_pTimeManager->Update();

	m_IsTarget = g_pKeyManager->isToggleKey(VK_TAB);

	if (m_pController)
		m_pController->Update(m_IsTarget, m_pObjMap);

	if (m_pCamera)
	{
		// 카메라가 따라다녀야 하는 타겟이 있으면
		if (m_IsTarget)
		{
			m_pCamera->Update(m_pController->GetPosition());
		}
		// 카메라 조작을 해야 하면
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

    for each (auto p in m_vecGroup)
        p->Render();

	g_pD3DDevice->EndScene();

	g_pD3DDevice->Present(0, 0, 0, 0);
}