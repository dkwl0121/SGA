#pragma once

struct ST_BONE : public D3DXFRAME
{
	D3DXMATRIX CombinedTrasformationMatrix;
};

struct ST_BONE_MESH : public D3DXMESHCONTAINER
{
	LPD3DXMESH			pOrigMesh;				// ���� �޽�
	LPD3DXMESH			pWorkMesh;				// �۾� �޽� CloneMeshFVF �� ����
	D3DXMATRIX**		ppBoneMatrixPtrs;		// �� �޽��� ������ �ִ� ������'��'�� ���� ��Ʈ���� ������ �迭
	D3DXMATRIX*			pBoneOffsetMatrixces;	// ���� �޽��� ���ý����̽��� ������ ��Ʈ������
	D3DXMATRIX*			pCurrentBoneMatrices;	// �� ���� ��� �� �����Ʈ������
};

class cAllocateHierarchy : public ID3DXAllocateHierarchy
{
private:
	SYNTHESIZE_PASS_BY_REF(string, m_sFolder, Folder);

public:
	cAllocateHierarchy();
	~cAllocateHierarchy();

	STDMETHOD(CreateFrame)(THIS_ LPCSTR Name,
		LPD3DXFRAME *ppNewFrame) override;
	STDMETHOD(CreateMeshContainer)(THIS_
		LPCSTR Name,
		CONST D3DXMESHDATA *pMeshData,
		CONST D3DXMATERIAL *pMaterials,
		CONST D3DXEFFECTINSTANCE *pEffectInstances,
		DWORD NumMaterials,
		CONST DWORD *pAdjacency,
		LPD3DXSKININFO pSkinInfo,
		LPD3DXMESHCONTAINER *ppNewMeshContainer) override;
	STDMETHOD(DestroyFrame)(THIS_ LPD3DXFRAME pFrameToFree) override;
	STDMETHOD(DestroyMeshContainer)(THIS_ LPD3DXMESHCONTAINER pMeshContainerToFree) override;
};

