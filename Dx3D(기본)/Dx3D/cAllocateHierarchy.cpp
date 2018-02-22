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
	프레임 생성 - 이름 설정(이름 없는 경우 널로 초기화), 매트릭스 초기화
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
	// 메쉬 생성
    ST_BONE_MESH* pMeshContainer = new ST_BONE_MESH;

	// 1. 이름 설정(안해도 무관)
	pMeshContainer->Name = new char[strlen(Name) + 1];
	strcpy_s(pMeshContainer->Name, strlen(Name) + 1, Name);

	// 2. 메쉬 데이터 설정
	// ADD1. 메쉬 복사 (원본 메쉬 사용, 워크 메쉬 복사)
    SAFE_ADDREF(pMeshData->pMesh);                          // 레퍼런스 카운터를 하나 증가시키고
	pMeshContainer->pOrigMesh = pMeshData->pMesh;	        // 얕은 복사를 하여 사용

    //********pMeshContainer->pWorkMesh = pMeshData->pMesh;************ // 얕은 복사를 하면 오리지널 매쉬까지 바뀌기 때문에 절대 안됨!!
    // 깊은 복사!!!!!!!!!!!!!!!!!11
    pMeshData->pMesh->CloneMeshFVF(pMeshData->pMesh->GetOptions(), pMeshData->pMesh->GetFVF(), g_pD3DDevice, &pMeshContainer->pWorkMesh);

	// 3. 매터리얼 정보(메터리얼 속성, 텍스쳐)
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

	// 4. 이펙트 = NULL
	pMeshContainer->pEffects = NULL;

	// 5. 메터리얼 갯수 설정
	pMeshContainer->NumMaterials = NumMaterials;

	// 6. 인접정보 = NULL
	pMeshContainer->pAdjacency = NULL;

	// 7. 스킨인포 = NULL
	// ADD2. 스킨인포 저장
	pMeshContainer->pSkinInfo = pSkinInfo;      // 얕은 복사
    SAFE_ADDREF(pSkinInfo);                     // 얕은 복사로 인한 레퍼런스 카운트 증가

	// ADD3. 매트릭스들에 대한 할당 : ppBoneMatrixPtrs, pBoneOffsetMatrixces, pCurrentBoneMatrices
	// pSkinInfo->GetNumBones() 현재 매쉬에 영향을 주는 매트릭스 갯수

    DWORD dwNumBone = pMeshContainer->pSkinInfo->GetNumBones();

    pMeshContainer->ppBoneMatrixPtrs = new D3DXMATRIX*[dwNumBone]; // (D3DXMATRIX**) malloc(sizeof(D3DXMATRIX*) * nBones);
    pMeshContainer->pBoneOffsetMatrixces = new D3DXMATRIX[dwNumBone]; // (D3DXMATRIX*) malloc(sizeof(D3DXMATRIX) * nBones);
    pMeshContainer->pCurrentBoneMatrices = new D3DXMATRIX[dwNumBone]; // (D3DXMATRIX*) malloc(sizeof(D3DXMATRIX) * nBones);

	// ADD4. pBoneOffsetMatrixces 값 셋팅
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
	CreateFrame 함수에서 할당 한 메모리에 대한 해제 (이름, 프레임)
	*/

    ST_BONE* pBone = (ST_BONE*)pFrameToFree;

	SAFE_DELETE_ARRAY(pBone->Name);
	SAFE_DELETE(pBone);

	return S_OK;
}

STDMETHODIMP cAllocateHierarchy::DestroyMeshContainer(LPD3DXMESHCONTAINER pMeshContainerToFree)
{
	/*
	CreateMeshContainer 함수에서 할당 한 메모리에 대한 해제
	(메쉬, 텍스쳐 파일 이름, 속성, 컨테이너)
	*/

    ST_BONE_MESH* pBoneMeshContainer = (ST_BONE_MESH*)pMeshContainerToFree;

    SAFE_DELETE_ARRAY(pBoneMeshContainer->pCurrentBoneMatrices);
    SAFE_DELETE_ARRAY(pBoneMeshContainer->pBoneOffsetMatrixces);
    SAFE_DELETE_ARRAY(pBoneMeshContainer->ppBoneMatrixPtrs);

	// ADD5. CreateMeshContainer 에서 바뀐 내용 또는 추가적인 할당에 대한 해제 
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
