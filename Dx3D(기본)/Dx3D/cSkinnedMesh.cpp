#include "stdafx.h"
#include "cSkinnedMesh.h"
#include "cAllocateHierarchy.h"


cSkinnedMesh::cSkinnedMesh()
	: m_pRoot(NULL)
    , m_pAnimController(NULL)
    , m_fBlendDuration(0.5f)
    , m_fPassedBlendTime(0.0f)
{
}


cSkinnedMesh::~cSkinnedMesh()
{
	// 생성한 프레임들에 대한 해제를 해줘야 한다.
	cAllocateHierarchy alloc;
	D3DXFrameDestroy(m_pRoot, &alloc);

    SAFE_RELEASE(m_pAnimController);
}

void cSkinnedMesh::Load(char* szFolder, char* szFile)
{
	string sFolder(szFolder);
	string sFile(szFile);
	string sFullPath = sFolder + "\\" + sFile;

	cAllocateHierarchy alloc;
	alloc.SetFolder(sFolder);

	// ID3DXAllocateHierarchy 를 상속하여 만든 파서 클래스를 사용하여 프레임들을 로드한다.
	D3DXLoadMeshHierarchyFromX(sFullPath.c_str(), D3DXMESH_MANAGED, g_pD3DDevice,
		&alloc, NULL, &m_pRoot, &m_pAnimController);

	SetupBoneMatrixPtrs(m_pRoot); // 파일 로드가 끝난 후 매트릭스 포인터들을 연결

    // 기본은 IDLE로 셋팅
    SetAnimation(E_ANIMSET_IDLE, false);
}

void cSkinnedMesh::SetupBoneMatrixPtrs(LPD3DXFRAME pFrame)
{
    // 각 프레임의 메시 컨테이너에 있는 pSkinInfo를 이용하여 영향받는 모든 
    // 프레임의 매트릭스를 ppBoneMatrixPtrs에 연결한다.
    // pSkinInfo->GetNumBones() 으로 영향받는 본의 개수를 찾음.
    // pSkinInfo->GetBoneName(i) 로 i번 프레임의 이름을 찾음
    // D3DXFrameFind(루트 프레임, 프레임 이름) 로 프레임을 찾음.
    // 찾아서 월드매트릭스를 걸어줘라.

    if (pFrame == NULL) return;

    ST_BONE_MESH* pBoneMesh = (ST_BONE_MESH*)pFrame->pMeshContainer;

    if (pBoneMesh && pBoneMesh->pSkinInfo)
    {
        DWORD dwNumBones = pBoneMesh->pSkinInfo->GetNumBones();

        for (DWORD i = 0; i < dwNumBones; ++i)
        {
            ST_BONE* pFindBoneFrame = (ST_BONE*)D3DXFrameFind(m_pRoot, pBoneMesh->pSkinInfo->GetBoneName(i));
            pBoneMesh->ppBoneMatrixPtrs[i] = &pFindBoneFrame->CombinedTrasformationMatrix; // 주소값을 연결만 한다!
        }
    }

    // 재귀적으로 모든 프레임에 대해서 실행.
    if (pFrame->pFrameSibling)
        SetupBoneMatrixPtrs(pFrame->pFrameSibling);

    if (pFrame->pFrameFirstChild)
        SetupBoneMatrixPtrs(pFrame->pFrameFirstChild);
}

void cSkinnedMesh::Update()
{
	float fDeltaTime = g_pTimeManager->GetDeltaTime();
	m_pAnimController->AdvanceTime(fDeltaTime, NULL);

	Update(m_pRoot, NULL);
	UpdateSkinnedMesh(m_pRoot);

    //m_pAnimController->GetMaxNumAnimationSets(); // 총 애니메이션 갯수
    //m_pAnimController->GetAnimationSet(인덱스, 애니메이션셋);// 애니메이션 셋 가져오기
    //m_pAnimController->SetTrackAnimationSet(트랙번호, 애니메이션셋);//트랙에 애니메이션셋 셋팅 -> ******기본은 0번으로 되어있음*********
    //m_pAnimController->SetTrackPosition(트랙번호, 포지션);// 애니메이션 트랙 포지션 셋팅 -> 포지션 : 애니메이션을 어느 위치부터 실행을 할지 정하는 값!!

    // 보간! -> 가중치에 따라서 알아서 보간이 됨  -> 가중치가 1이 됐을 때 알아서 비활성화를 하면 된다.
    //m_pAnimController->SetTrackWeight(트랙번호, 가중치); // 애니메이션 가중치 설정
    //m_pAnimController->SetTrackEnable(트랙번호, 불값); // 애니메이션 활성화 설정
    //m_pAnimController->GetTrackAnimationSet(트랙번호, 애니메이션셋); // 특정 트랙의 애니메이션 셋을 가지고 온다.
    //m_pAnimController->GetTrackDesc(트랙번호, 트랙정보구조체); // 트랙정보 가져오기
    //m_pAnimController->SetTrackDesc(트랙번호, 트랙정보구조체); // 트랙정보 셋팅

    if (g_pKeyManager->isOnceKeyDown('1'))
    {
        SetAnimation(E_ANIMSET_ATT1, false);
    }
    else if (g_pKeyManager->isOnceKeyDown('2'))
    {
        SetAnimation(E_ANIMSET_ATT2, false);
    }
    else if (g_pKeyManager->isOnceKeyDown('3'))
    {
        SetAnimation(E_ANIMSET_ATT3, false);
    }
    else if (g_pKeyManager->isOnceKeyDown('4'))
    {
        SetAnimation(E_ANIMSET_RUN, true);
    }
    else if (g_pKeyManager->isOnceKeyDown('5'))
    {
        SetAnimation(E_ANIMSET_IDLE, true);
    }

    if (m_fPassedBlendTime < m_fBlendDuration) // 블렌딩 진행 중
    {
        m_fPassedBlendTime += fDeltaTime; // == g_pTimeManager->GetDeltaTime();

        if (m_fPassedBlendTime < m_fBlendDuration) // 블렌딩이 끝날 때가 되지 않음
        {
            float fWeight = m_fPassedBlendTime / m_fBlendDuration;

            m_pAnimController->SetTrackWeight(0, fWeight);
            m_pAnimController->SetTrackWeight(1, 1.0f - fWeight);
        }
        else // 블렌딩이 끝날 시점
        {
            m_pAnimController->SetTrackWeight(0, 1);
            m_pAnimController->SetTrackEnable(1, false);
        }
    }
}

void cSkinnedMesh::Update(LPD3DXFRAME pFrame, LPD3DXFRAME pParent)
{
    if (pFrame == NULL) return;

    // 스킨드 메쉬의 월드 매트릭스(컴바인드 트랜스폼 매트릭스) 업데이트

    ST_BONE* pBone = (ST_BONE*)pFrame;

    pBone->CombinedTrasformationMatrix = pBone->TransformationMatrix;

    if (pParent)
    {
        ST_BONE* pBoneParent = (ST_BONE*)pParent;
        pBone->CombinedTrasformationMatrix *= pBoneParent->CombinedTrasformationMatrix;
    }

    // 재귀적으로 모든 프레임에 대해서 실행.
    if (pFrame->pFrameSibling)
        Update(pFrame->pFrameSibling, pParent); // -> (이진트리방식이 아니기때문에!!!!!) 실빌링(형제)는 부모가 자신의 부모가 같다!!!!! 주의!!!!!!!

    if (pFrame->pFrameFirstChild)
        Update(pFrame->pFrameFirstChild, pFrame);
}

void cSkinnedMesh::UpdateSkinnedMesh(LPD3DXFRAME pFrame)
{
	// pCurrentBoneMatrices 를 계산하시오
	// pCurrentBoneMatrices = pBoneOffsetMatrices * ppBoneMatrixPtrs

	if (pFrame == NULL) return;

    ST_BONE_MESH* pBoneMesh = (ST_BONE_MESH*)pFrame->pMeshContainer;

	if (pBoneMesh)
	{
		if (pBoneMesh->pSkinInfo)
		{
			LPD3DXSKININFO pSkinInfo = pBoneMesh->pSkinInfo;
			DWORD dwNumBones = pSkinInfo->GetNumBones();
			for (DWORD i = 0; i < dwNumBones; ++i)
			{
				pBoneMesh->pCurrentBoneMatrices[i] = pBoneMesh->pBoneOffsetMatrixces[i] * *(pBoneMesh->ppBoneMatrixPtrs[i]);
			}

			BYTE* src = NULL;
			BYTE* dest = NULL;

			pBoneMesh->pOrigMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&src);
			pBoneMesh->pWorkMesh->LockVertexBuffer(0, (void**)&dest);

			// 메쉬 업데이트
			pSkinInfo->UpdateSkinnedMesh(pBoneMesh->pCurrentBoneMatrices, NULL, src, dest);

			pBoneMesh->pWorkMesh->UnlockVertexBuffer();
			pBoneMesh->pOrigMesh->UnlockVertexBuffer();
		}
	}

	// 재귀적으로 모든 프레임에 대해서 실행.
    if (pFrame->pFrameFirstChild)
        UpdateSkinnedMesh(pFrame->pFrameFirstChild);
    if (pFrame->pFrameSibling)
        UpdateSkinnedMesh(pFrame->pFrameSibling);
}

void cSkinnedMesh::Render(LPD3DXFRAME pFrame)
{
	D3DXMATRIXA16 mat;
	D3DXMatrixIdentity(&mat);

	D3DXMatrixScaling(&mat, 10, 10, 10);

	if (pFrame == NULL)
	{
		pFrame = m_pRoot;
	}

	if (pFrame)
	{
		/*
		매트릭스(항등행렬), 속성, 텍스쳐 설정
		메쉬 드로우
		*/
		if (pFrame->pMeshContainer)
		{
            ST_BONE_MESH* pBoneMeshContainer = (ST_BONE_MESH*)pFrame->pMeshContainer;

			for (int i = 0; i < pBoneMeshContainer->NumMaterials; ++i)
			{
				LPD3DXMATERIAL pMtlTex = &pBoneMeshContainer->pMaterials[i];

				g_pD3DDevice->SetTransform(D3DTS_WORLD, &mat);
				g_pD3DDevice->SetMaterial(&pMtlTex->MatD3D);
				g_pD3DDevice->SetTexture(0, g_pTextureManager->GetTexture(pMtlTex->pTextureFilename));

                pBoneMeshContainer->pWorkMesh->DrawSubset(i); // => 워크 메쉬를 사용하여 렌더
			}
		}
	}

	/*
	재귀: 형제, 자식 들도 렌더링
	*/
	if (pFrame->pFrameSibling)
		Render(pFrame->pFrameSibling);

	if (pFrame->pFrameFirstChild)
		Render(pFrame->pFrameFirstChild);
}

void cSkinnedMesh::SetAnimation(int nIndex, bool isBlend)
{
    // 변경할 애니메이션셋 셋팅
    LPD3DXANIMATIONSET pNextAnimSet = NULL;
    m_pAnimController->GetAnimationSet(nIndex, &pNextAnimSet);

    if (isBlend)
    {
        LPD3DXANIMATIONSET	pPrevAnimSet = NULL;
        D3DXTRACK_DESC	stTrackDesc;
        // 현재 재생중이던 애니메이션 정보 가져오기
        m_pAnimController->GetTrackAnimationSet(0, &pPrevAnimSet);
        m_pAnimController->GetTrackDesc(0, &stTrackDesc);
        // 0번 트랙 -> 1번 트랙
        m_pAnimController->SetTrackAnimationSet(1, pPrevAnimSet);
        m_pAnimController->SetTrackDesc(1, &stTrackDesc);

        m_pAnimController->SetTrackWeight(0, 0.0f);

        SAFE_RELEASE(pPrevAnimSet);

        m_fPassedBlendTime = 0.0f;
    }

    m_pAnimController->SetTrackAnimationSet(0, pNextAnimSet);
    m_pAnimController->SetTrackPosition(0, 0.0);
    SAFE_RELEASE(pNextAnimSet);
}
