#include "stdafx.h"
#include "cMainGame.h"
#include "cCamera.h"
#include "cCharacterController.h"
#include "cMtlTex.h"
#include "cAseCharacter.h"
#include "cHeightMap.h"
#include "cGrid.h"
#include "cPicking.h"
#include "cPillar.h"
#include "cUIController.h"
#include "cSkinnedMesh.h"

cMainGame::cMainGame()
	: m_pCamera(NULL)
	, m_pController(NULL)
	, m_IsTarget(true)
    , m_pMeshMap(NULL)
    , m_pAseCharacter(NULL)
    , m_pHeightMap(NULL)
    , m_pGrid(NULL)
    , m_pPicking(NULL)
    , m_pUIController(NULL)
    , m_pSkinnedMesh(NULL)
{
}

cMainGame::~cMainGame()
{
    SAFE_RELEASE(m_pMeshMap);

    g_pLightManager->Destroy();
	g_pTextureManager->Destroy();
    g_pFontManager->Destory();

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

	srand(time(NULL));
	rand();

	// ���� ����Ʈ
	D3DXVECTOR3 dir(1.0f, -1.0f, 0.0f);
	D3DXVec3Normalize(&dir, &dir);
	D3DLIGHT9 stLight = InitDirectional(&dir, &WHITE);
	g_pLightManager->AddLight("main", stLight);

    // ���� ���� ���������͵��� �������ִ� �Լ�!! (��� �������͸� ��ֶ����� ���ִ� �Լ�!!!) �̷��� �ؾ� ���� ���� ����� �� ��.
    g_pD3DDevice->SetRenderState(D3DRS_NORMALIZENORMALS, true);

    m_pGrid = new cGrid;
    g_pAutoReleasePool->AddObject(m_pGrid);
    m_pGrid->Setup();

    // == ����Ʈ�� ==
    m_pHeightMap = new cHeightMap;
    g_pAutoReleasePool->AddObject(m_pHeightMap);
    D3DXMATRIXA16 matW;
    D3DXMatrixScaling(&matW, 1.0f, 1.0f, 1.0f);
    m_pHeightMap->Load("HeightMapData/HeightMap.raw", &matW);

	m_pCamera = new cCamera;
    g_pAutoReleasePool->AddObject(m_pCamera);
	m_pCamera->Setup();

	m_pController = new cCharacterController;
    g_pAutoReleasePool->AddObject(m_pController);
    m_pController->Setup(m_pHeightMap);

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
        
    // ��ŷ
    m_pPicking = new cPicking;
    g_pAutoReleasePool->AddObject(m_pPicking);

    // ���(��ֹ�)
    m_pPillar = new cPillar;
    g_pAutoReleasePool->AddObject(m_pPillar);
    m_pPillar->Setup();

    // UI
    m_pUIController = new cUIController;
    g_pAutoReleasePool->AddObject(m_pUIController);
    m_pUIController->Setup();

    m_pSkinnedMesh = new cSkinnedMesh;
    g_pAutoReleasePool->AddObject(m_pSkinnedMesh);
    m_pSkinnedMesh->Load("Zealot", "zealot.X");
}

void cMainGame::Update()
{
	g_pTimeManager->Update();

	m_IsTarget = g_pKeyManager->isToggleKey(VK_TAB);

    if (m_pUIController)
        m_pUIController->Update();

    if (m_pPicking)
        m_pPicking->Update();

    // ��ŷ�� ������
    if (m_pPicking->GetIsPick())
    {
        m_pPicking->SetIsPick(false);
        
        // �̵� ������ ���� ��ŷ�� �ߴٸ�
        if (m_pController->CheckPick(m_pPicking->GetRay(), m_pPillar))
        {
            // ���̽�Ÿ ������� �̵��ؾ� �Ѵٸ�
            if (m_pController->GetPickStat() == E_PICK_STAT_ASTAR)
            {
                m_pUIController->SetInfo(E_UI_KIND_LOADINFO, tagUIInfo(*m_pController->GetPosition(), m_pController->GetPickPos(), true));
            }
            // ���̷�Ʈ ������� �̵��ؾ� �Ѵٸ�
            else if (m_pController->GetPickStat() == E_PICK_STAT_NOASTAR)
            {
                m_pUIController->SetInfo(E_UI_KIND_LOADINFO, tagUIInfo(*m_pController->GetPosition(), m_pController->GetPickPos(), false));
            }
        }
    }

    // ���� �� ��� ���� ���
    if (m_pController->GetPickStat() == E_PICK_STAT_NONE)
    {
        m_pUIController->SetInfo(E_UI_KIND_LOADCNT, tagUIInfo(*m_pController->GetPosition(), m_pController->GetPathCnt()));
    }
    else
    {
        m_pUIController->SetInfo(E_UI_KIND_LOADCNT, tagUIInfo(*m_pController->GetPosition(), 0));
    }

    // "���" UI�� Ŭ�� ������ ĳ���� �̵� ����
    if (m_pController->GetPickStat() != E_PICK_STAT_NONE && m_pUIController->GetIsClickStart())
        m_pController->SetPickStat(E_PICK_STAT_NONE);

    if (m_pController)
        m_pController->Update(m_IsTarget);

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

    if (m_pSkinnedMesh)
        m_pSkinnedMesh->Update();
}

void cMainGame::Render()
{
	g_pD3DDevice->Clear(NULL, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
		D3DCOLOR_XRGB(47, 121, 112), 1.0f, 0);

	g_pD3DDevice->BeginScene();

    //g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, false);

    // ���̾� ��� ���
    //g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

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

    g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, true);

    if (m_pHeightMap)
        m_pHeightMap->Render();

    if (m_pAseCharacter)
        m_pAseCharacter->Render();

    if (m_pPillar)
        m_pPillar->Render();

    if (m_pController)
        m_pController->Render(); // AStar������

    if (m_pSkinnedMesh)
        m_pSkinnedMesh->Render();

    if (m_pUIController)
        m_pUIController->Render();

	g_pD3DDevice->EndScene();

	g_pD3DDevice->Present(0, 0, 0, 0);
}