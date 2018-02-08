#include "stdafx.h"
#include "cMainGame.h"
#include "cCamera.h"
#include "cCharacterController.h"
#include "cObjMap.h"
#include "cObjLoader.h"
#include "cMtlTex.h"
#include "cAseCharacter.h"
#include "cHeightMap.h"
#include "cGrid.h"
#include "cPicking.h"
#include "cAster.h"

cMainGame::cMainGame()
	: m_pCamera(NULL)
	, m_pController(NULL)
    , m_pObjMap(NULL)
	, m_IsTarget(true)
    , m_pMeshMap(NULL)
    , m_pAseCharacter(NULL)
    , m_pHeightMap(NULL)
    , m_pGrid(NULL)
    , m_pPicking(NULL)
    , m_pAster(NULL)
{
}

cMainGame::~cMainGame()
{
    SAFE_RELEASE(m_pMeshMap);

    g_pLightManager->Destroy();
	g_pTextureManager->Destroy();
    g_pDrawTextManager->Destroy();

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
	
    g_pKeyManager->Setup();
    g_pDrawTextManager->Setup();

	srand(time(NULL));
	rand();

	// 메인 라이트
	D3DXVECTOR3 dir(1.0f, -1.0f, 0.0f);
	D3DXVec3Normalize(&dir, &dir);
	D3DLIGHT9 stLight = InitDirectional(&dir, &WHITE);
	g_pLightManager->AddLight("main", stLight);

    m_pGrid = new cGrid;
    g_pAutoReleasePool->AddObject(m_pGrid);
    m_pGrid->Setup();

	m_pCamera = new cCamera;
    g_pAutoReleasePool->AddObject(m_pCamera);
	m_pCamera->Setup();

	m_pController = new cCharacterController;
    g_pAutoReleasePool->AddObject(m_pController);

    m_pAseCharacter = new cAseCharacter;
    g_pAutoReleasePool->AddObject(m_pAseCharacter);
    m_pAseCharacter->Load("woman/woman_01_all.ase");

    //// == OBJ 맵 ==
    //D3DXMATRIXA16 mat, matS, matR, matT;
    //D3DXMatrixIdentity(&mat);
    //D3DXMatrixScaling(&matS, 0.05f, 0.05f, 0.05f);
    //D3DXMatrixRotationX(&matR, -D3DX_PI * 0.5f);
    //D3DXMatrixTranslation(&matT, 0, 0, -3);
    //mat = matS * matR * matT;

    //cObjLoader* pObjLoader = new cObjLoader;
    //m_pMeshMap = pObjLoader->LoadMesh("obj/map.obj", &mat, m_vecMtlTex);
    //SAFE_RELEASE(pObjLoader);

    //// == 지형 충돌처리 그룹 ==
    //m_pObjMap = new cObjMap;
    //g_pAutoReleasePool->AddObject(m_pObjMap);
    //m_pObjMap->Load("obj/map_surface.obj", &mat);

    // == 하이트맵 ==
    m_pHeightMap = new cHeightMap;
    g_pAutoReleasePool->AddObject(m_pHeightMap);
    D3DXMATRIXA16 matW;
    D3DXMatrixScaling(&matW, 1.0f, 1.0f, 1.0f);
    m_pHeightMap->Load("HeightMapData/HeightMap.raw", &matW);

    // 픽킹
    m_pPicking = new cPicking;
    g_pAutoReleasePool->AddObject(m_pPicking);
    m_pPicking->Setup();

    // 에이스타
    m_pAster = new cAster;
    g_pAutoReleasePool->AddObject(m_pAster);
    m_pAster->Setup(m_pHeightMap->GetVertex());
}

void cMainGame::Update()
{
	g_pTimeManager->Update();

	m_IsTarget = g_pKeyManager->isToggleKey(VK_TAB);

    if (m_pPicking)
        m_pPicking->Update(m_pHeightMap);

    // 픽킹을 했으면
    if (m_pPicking->GetIsPick())
    {
        m_pPicking->SetIsPick(false);
        m_pAster->FindPath(*m_pController->GetPosition(), m_pPicking->GetPickPos()); // 시작점, 도착점 넣기
        m_pController->SetDestPos(*m_pController->GetPosition()); // 픽킹을 했으면 설정된 도착지점 초기화 하기
    }

    vector<D3DXVECTOR3>& path = m_pAster->GetPath();
    if (!path.empty() && !m_pController->GetIsMoving())
    {
        m_pController->SetDestPos(path.back());
        path.pop_back();
    }

    if (m_pController)
        m_pController->Update(m_IsTarget, m_pHeightMap);

    if (m_pAseCharacter)
        m_pAseCharacter->Update(m_pController->GetWorldTM());

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
}

void cMainGame::Render()
{
	g_pD3DDevice->Clear(NULL, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
		D3DCOLOR_XRGB(47, 121, 112), 1.0f, 0);

	g_pD3DDevice->BeginScene();

    g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, false);

    if (m_pGrid)
        m_pGrid->Render();

    //D3DXMATRIXA16 matW;
    //D3DXMatrixIdentity(&matW);
    //g_pD3DDevice->SetTransform(D3DTS_WORLD, &matW);

    // 메쉬로 그리기
    //for (int i = 0; i < m_vecMtlTex.size(); ++i)
    //{
    //    g_pD3DDevice->SetMaterial(m_vecMtlTex[i]->GetMtl());
    //    g_pD3DDevice->SetTexture(0, m_vecMtlTex[i]->GetTexture());
    //    m_pMeshMap->DrawSubset(i);
    //}

    if (m_pHeightMap)
        m_pHeightMap->Render();

    if (m_pAseCharacter)
        m_pAseCharacter->Render();

    // 와이어 모드 출력
    //g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

    g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, true);

    if (m_pAster)
        m_pAster->Render();

    RECT rt = { 10, 10, 200, 200 };
    g_pDrawTextManager->DrawTextOut("Git D3DX Base", rt, WHITE, "도담9");

	g_pD3DDevice->EndScene();

	g_pD3DDevice->Present(0, 0, 0, 0);
}