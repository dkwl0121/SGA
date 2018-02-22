#include "stdafx.h"
#include "cAllocateHierarchy.h"


cAllocateHierarchy::cAllocateHierarchy()
{
}


cAllocateHierarchy::~cAllocateHierarchy()
{
}

STDMETHODIMP cAllocateHierarchy::CreateFrame(LPCSTR Name, LPD3DXFRAME* ppNewFrame)
{
	/*
	������ ���� - �̸� ����(�̸� ���� ��� �η� �ʱ�ȭ), ��Ʈ���� �ʱ�ȭ
	*/
    ST_BONE* pFrame = new ST_BONE;

	if (Name)
	{
		pFrame->Name = new char[strlen(Name) + 1];
		strcpy_s(pFrame->Name, strlen(Name) + 1, Name);
	}
	else
	{
		pFrame->Name = NULL;
	}

	D3DXMatrixIdentity(&(pFrame->TransformationMatrix));
    D3DXMatrixIdentity(&(pFrame->CombinedTrasformationMatrix));

	*ppNewFrame = pFrame;

	return S_OK;
}

STDMETHODIMP cAllocateHierarchy::CreateMeshContainer(
	LPCSTR Name,
	CONST D3DXMESHDATA* pMeshData,
	CONST D3DXMATERIAL* pMaterials,
	CONST D3DXEFFECTINSTANCE* pEffectInstances,
	DWORD NumMaterials,
	CONST DWORD* pAdjacency,
	LPD3DXSKININFO pSkinInfo,
	LPD3DXMESHCONTAINER* ppNewMeshContainer)
{
	// �޽� ����
    ST_BONE_MESH* pMeshContainer = new ST_BONE_MESH;

	// 1. �̸� ����(���ص� ����)
	pMeshContainer->Name = new char[strlen(Name) + 1];
	strcpy_s(pMeshContainer->Name, strlen(Name) + 1, Name);

	// 2. �޽� ������ ����
	// ADD1. �޽� ���� (���� �޽� ���, ��ũ �޽� ����)
    SAFE_ADDREF(pMeshData->pMesh);                          // ���۷��� ī���͸� �ϳ� ������Ű��
	pMeshContainer->pOrigMesh = pMeshData->pMesh;	        // ���� ���縦 �Ͽ� ���

    //********pMeshContainer->pWorkMesh = pMeshData->pMesh;************ // ���� ���縦 �ϸ� �������� �Ž����� �ٲ�� ������ ���� �ȵ�!!
    // ���� ����!!!!!!!!!!!!!!!!!11
    pMeshData->pMesh->CloneMeshFVF(pMeshData->pMesh->GetOptions(), pMeshData->pMesh->GetFVF(), g_pD3DDevice, &pMeshContainer->pWorkMesh);

	// 3. ���͸��� ����(���͸��� �Ӽ�, �ؽ���)
	pMeshContainer->pMaterials = new D3DXMATERIAL[NumMaterials];
	for (int i = 0; i < NumMaterials; ++i)
	{
		string filePath(pMaterials[i].pTextureFilename);
		string fullPath = m_sFolder + "\\" + filePath;
		g_pTextureManager->AddTexture(filePath, fullPath);

		pMeshContainer->pMaterials[i].MatD3D = pMaterials[i].MatD3D;
		pMeshContainer->pMaterials[i].pTextureFilename =
			new char[strlen(pMaterials[i].pTextureFilename) + 1];
		strcpy_s(pMeshContainer->pMaterials[i].pTextureFilename, strlen(pMaterials[i].pTextureFilename) + 1, pMaterials[i].pTextureFilename);
	}

	// 4. ����Ʈ = NULL
	pMeshContainer->pEffects = NULL;

	// 5. ���͸��� ���� ����
	pMeshContainer->NumMaterials = NumMaterials;

	// 6. �������� = NULL
	pMeshContainer->pAdjacency = NULL;

	// 7. ��Ų���� = NULL
	// ADD2. ��Ų���� ����
	pMeshContainer->pSkinInfo = pSkinInfo;      // ���� ����
    SAFE_ADDREF(pSkinInfo);                     // ���� ����� ���� ���۷��� ī��Ʈ ����

	// ADD3. ��Ʈ�����鿡 ���� �Ҵ� : ppBoneMatrixPtrs, pBoneOffsetMatrixces, pCurrentBoneMatrices
	// pSkinInfo->GetNumBones() ���� �Ž��� ������ �ִ� ��Ʈ���� ����

    DWORD dwNumBone = pMeshContainer->pSkinInfo->GetNumBones();

    pMeshContainer->ppBoneMatrixPtrs = new D3DXMATRIX*[dwNumBone]; // (D3DXMATRIX**) malloc(sizeof(D3DXMATRIX*) * nBones);
    pMeshContainer->pBoneOffsetMatrixces = new D3DXMATRIX[dwNumBone]; // (D3DXMATRIX*) malloc(sizeof(D3DXMATRIX) * nBones);
    pMeshContainer->pCurrentBoneMatrices = new D3DXMATRIX[dwNumBone]; // (D3DXMATRIX*) malloc(sizeof(D3DXMATRIX) * nBones);

	// ADD4. pBoneOffsetMatrixces �� ����
	// pSkinInfo->GetBoneOffsetMatrix(i)
    for (DWORD i = 0; i < dwNumBone; ++i)
    {
        pMeshContainer->pBoneOffsetMatrixces[i] = *pMeshContainer->pSkinInfo->GetBoneOffsetMatrix(i);
    }

	*ppNewMeshContainer = pMeshContainer;

	return S_OK;
}

STDMETHODIMP cAllocateHierarchy::DestroyFrame(LPD3DXFRAME pFrameToFree)
{
	/*
	CreateFrame �Լ����� �Ҵ� �� �޸𸮿� ���� ���� (�̸�, ������)
	*/

    ST_BONE* pBone = (ST_BONE*)pFrameToFree;

	SAFE_DELETE_ARRAY(pBone->Name);
	SAFE_DELETE(pBone);

	return S_OK;
}

STDMETHODIMP cAllocateHierarchy::DestroyMeshContainer(LPD3DXMESHCONTAINER pMeshContainerToFree)
{
	/*
	CreateMeshContainer �Լ����� �Ҵ� �� �޸𸮿� ���� ����
	(�޽�, �ؽ��� ���� �̸�, �Ӽ�, �����̳�)
	*/

    ST_BONE_MESH* pBoneMeshContainer = (ST_BONE_MESH*)pMeshContainerToFree;

    SAFE_DELETE_ARRAY(pBoneMeshContainer->pCurrentBoneMatrices);
    SAFE_DELETE_ARRAY(pBoneMeshContainer->pBoneOffsetMatrixces);
    SAFE_DELETE_ARRAY(pBoneMeshContainer->ppBoneMatrixPtrs);

	// ADD5. CreateMeshContainer ���� �ٲ� ���� �Ǵ� �߰����� �Ҵ翡 ���� ���� 
    SAFE_RELEASE(pBoneMeshContainer->pSkinInfo);

	SAFE_RELEASE(pBoneMeshContainer->pOrigMesh);
    SAFE_RELEASE(pBoneMeshContainer->pWorkMesh);

	for (int i = 0; i < pBoneMeshContainer->NumMaterials; ++i)
	{
		SAFE_DELETE_ARRAY(pBoneMeshContainer->pMaterials[i].pTextureFilename);
	}
	SAFE_DELETE_ARRAY(pBoneMeshContainer->pMaterials);

	SAFE_DELETE_ARRAY(pBoneMeshContainer->Name);

	SAFE_DELETE(pBoneMeshContainer);

	return S_OK;
}
