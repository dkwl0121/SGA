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

    // ������Ʈ ���� �۾� ���� �ϱ�!!!
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

	// ���� ����Ʈ
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

    //// == OBJ �� ==
    //D3DXMATRIXA16 mat, matS, matR, matT;
    //D3DXMatrixIdentity(&mat);
    //D3DXMatrixScaling(&matS, 0.05f, 0.05f, 0.05f);
    //D3DXMatrixRotationX(&matR, -D3DX_PI * 0.5f);
    //D3DXMatrixTranslation(&matT, 0, 0, -3);
    //mat = matS * matR * matT;

    //cObjLoader* pObjLoader = new cObjLoader;
    //m_pMeshMap = pObjLoader->LoadMesh("obj/map.obj", &mat, m_vecMtlTex);
    //SAFE_RELEASE(pObjLoader);

    //// == ���� �浹ó�� �׷� ==
    //m_pObjMap = new cObjMap;
    //g_pAutoReleasePool->AddObject(m_pObjMap);
    //m_pObjMap->Load("obj/map_surface.obj", &mat);

    // == ����Ʈ�� ==
    m_pHeightMap = new cHeightMap;
    g_pAutoReleasePool->AddObject(m_pHeightMap);
    D3DXMATRIXA16 matW;
    D3DXMatrixScaling(&matW, 1.0f, 1.0f, 1.0f);
    m_pHeightMap->Load("HeightMapData/HeightMap.raw", &matW);

    // ��ŷ
    m_pPicking = new cPicking;
    g_pAutoReleasePool->AddObject(m_pPicking);
    m_pPicking->Setup();

    // ���̽�Ÿ
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

    // ��ŷ�� ������
    if (m_pPicking->GetIsPick())
    {
        m_pPicking->SetIsPick(false);
        m_pAster->FindPath(*m_pController->GetPosition(), m_pPicking->GetPickPos()); // ������, ������ �ֱ�
        m_pController->SetDestPos(*m_pController->GetPosition()); // ��ŷ�� ������ ������ �������� �ʱ�ȭ �ϱ�
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

    // �޽��� �׸���
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

    // ���̾� ��� ���
    //g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

    g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, true);

    if (m_pAster)
        m_pAster->Render();

    RECT rt = { 10, 10, 200, 200 };
    g_pDrawTextManager->DrawTextOut("Git D3DX Base", rt, WHITE, "����9");

	g_pD3DDevice->EndScene();

	g_pD3DDevice->Present(0, 0, 0, 0);
}