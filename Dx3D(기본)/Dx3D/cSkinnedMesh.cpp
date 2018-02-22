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
	// ������ �����ӵ鿡 ���� ������ ����� �Ѵ�.
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

	// ID3DXAllocateHierarchy �� ����Ͽ� ���� �ļ� Ŭ������ ����Ͽ� �����ӵ��� �ε��Ѵ�.
	D3DXLoadMeshHierarchyFromX(sFullPath.c_str(), D3DXMESH_MANAGED, g_pD3DDevice,
		&alloc, NULL, &m_pRoot, &m_pAnimController);

	SetupBoneMatrixPtrs(m_pRoot); // ���� �ε尡 ���� �� ��Ʈ���� �����͵��� ����

    // �⺻�� IDLE�� ����
    SetAnimation(E_ANIMSET_IDLE, false);
}

void cSkinnedMesh::SetupBoneMatrixPtrs(LPD3DXFRAME pFrame)
{
    // �� �������� �޽� �����̳ʿ� �ִ� pSkinInfo�� �̿��Ͽ� ����޴� ��� 
    // �������� ��Ʈ������ ppBoneMatrixPtrs�� �����Ѵ�.
    // pSkinInfo->GetNumBones() ���� ����޴� ���� ������ ã��.
    // pSkinInfo->GetBoneName(i) �� i�� �������� �̸��� ã��
    // D3DXFrameFind(��Ʈ ������, ������ �̸�) �� �������� ã��.
    // ã�Ƽ� �����Ʈ������ �ɾ����.

    if (pFrame == NULL) return;

    ST_BONE_MESH* pBoneMesh = (ST_BONE_MESH*)pFrame->pMeshContainer;

    if (pBoneMesh && pBoneMesh->pSkinInfo)
    {
        DWORD dwNumBones = pBoneMesh->pSkinInfo->GetNumBones();

        for (DWORD i = 0; i < dwNumBones; ++i)
        {
            ST_BONE* pFindBoneFrame = (ST_BONE*)D3DXFrameFind(m_pRoot, pBoneMesh->pSkinInfo->GetBoneName(i));
            pBoneMesh->ppBoneMatrixPtrs[i] = &pFindBoneFrame->CombinedTrasformationMatrix; // �ּҰ��� ���Ḹ �Ѵ�!
        }
    }

    // ��������� ��� �����ӿ� ���ؼ� ����.
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

    //m_pAnimController->GetMaxNumAnimationSets(); // �� �ִϸ��̼� ����
    //m_pAnimController->GetAnimationSet(�ε���, �ִϸ��̼Ǽ�);// �ִϸ��̼� �� ��������
    //m_pAnimController->SetTrackAnimationSet(Ʈ����ȣ, �ִϸ��̼Ǽ�);//Ʈ���� �ִϸ��̼Ǽ� ���� -> ******�⺻�� 0������ �Ǿ�����*********
    //m_pAnimController->SetTrackPosition(Ʈ����ȣ, ������);// �ִϸ��̼� Ʈ�� ������ ���� -> ������ : �ִϸ��̼��� ��� ��ġ���� ������ ���� ���ϴ� ��!!

    // ����! -> ����ġ�� ���� �˾Ƽ� ������ ��  -> ����ġ�� 1�� ���� �� �˾Ƽ� ��Ȱ��ȭ�� �ϸ� �ȴ�.
    //m_pAnimController->SetTrackWeight(Ʈ����ȣ, ����ġ); // �ִϸ��̼� ����ġ ����
    //m_pAnimController->SetTrackEnable(Ʈ����ȣ, �Ұ�); // �ִϸ��̼� Ȱ��ȭ ����
    //m_pAnimController->GetTrackAnimationSet(Ʈ����ȣ, �ִϸ��̼Ǽ�); // Ư�� Ʈ���� �ִϸ��̼� ���� ������ �´�.
    //m_pAnimController->GetTrackDesc(Ʈ����ȣ, Ʈ����������ü); // Ʈ������ ��������
    //m_pAnimController->SetTrackDesc(Ʈ����ȣ, Ʈ����������ü); // Ʈ������ ����

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

    if (m_fPassedBlendTime < m_fBlendDuration) // ���� ���� ��
    {
        m_fPassedBlendTime += fDeltaTime; // == g_pTimeManager->GetDeltaTime();

        if (m_fPassedBlendTime < m_fBlendDuration) // ������ ���� ���� ���� ����
        {
            float fWeight = m_fPassedBlendTime / m_fBlendDuration;

            m_pAnimController->SetTrackWeight(0, fWeight);
            m_pAnimController->SetTrackWeight(1, 1.0f - fWeight);
        }
        else // ������ ���� ����
        {
            m_pAnimController->SetTrackWeight(0, 1);
            m_pAnimController->SetTrackEnable(1, false);
        }
    }
}

void cSkinnedMesh::Update(LPD3DXFRAME pFrame, LPD3DXFRAME pParent)
{
    if (pFrame == NULL) return;

    // ��Ų�� �޽��� ���� ��Ʈ����(�Ĺ��ε� Ʈ������ ��Ʈ����) ������Ʈ

    ST_BONE* pBone = (ST_BONE*)pFrame;

    pBone->CombinedTrasformationMatrix = pBone->TransformationMatrix;

    if (pParent)
    {
        ST_BONE* pBoneParent = (ST_BONE*)pParent;
        pBone->CombinedTrasformationMatrix *= pBoneParent->CombinedTrasformationMatrix;
    }

    // ��������� ��� �����ӿ� ���ؼ� ����.
    if (pFrame->pFrameSibling)
        Update(pFrame->pFrameSibling, pParent); // -> (����Ʈ������� �ƴϱ⶧����!!!!!) �Ǻ���(����)�� �θ� �ڽ��� �θ� ����!!!!! ����!!!!!!!

    if (pFrame->pFrameFirstChild)
        Update(pFrame->pFrameFirstChild, pFrame);
}

void cSkinnedMesh::UpdateSkinnedMesh(LPD3DXFRAME pFrame)
{
	// pCurrentBoneMatrices �� ����Ͻÿ�
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

			// �޽� ������Ʈ
			pSkinInfo->UpdateSkinnedMesh(pBoneMesh->pCurrentBoneMatrices, NULL, src, dest);

			pBoneMesh->pWorkMesh->UnlockVertexBuffer();
			pBoneMesh->pOrigMesh->UnlockVertexBuffer();
		}
	}

	// ��������� ��� �����ӿ� ���ؼ� ����.
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
		��Ʈ����(�׵����), �Ӽ�, �ؽ��� ����
		�޽� ��ο�
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

                pBoneMeshContainer->pWorkMesh->DrawSubset(i); // => ��ũ �޽��� ����Ͽ� ����
			}
		}
	}

	/*
	���: ����, �ڽ� �鵵 ������
	*/
	if (pFrame->pFrameSibling)
		Render(pFrame->pFrameSibling);

	if (pFrame->pFrameFirstChild)
		Render(pFrame->pFrameFirstChild);
}

void cSkinnedMesh::SetAnimation(int nIndex, bool isBlend)
{
    // ������ �ִϸ��̼Ǽ� ����
    LPD3DXANIMATIONSET pNextAnimSet = NULL;
    m_pAnimController->GetAnimationSet(nIndex, &pNextAnimSet);

    if (isBlend)
    {
        LPD3DXANIMATIONSET	pPrevAnimSet = NULL;
        D3DXTRACK_DESC	stTrackDesc;
        // ���� ������̴� �ִϸ��̼� ���� ��������
        m_pAnimController->GetTrackAnimationSet(0, &pPrevAnimSet);
        m_pAnimController->GetTrackDesc(0, &stTrackDesc);
        // 0�� Ʈ�� -> 1�� Ʈ��
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
