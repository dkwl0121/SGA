#pragma once
#include "cObject.h"

enum E_ANIMSET { 
    E_ANIMSET_ATT1,
    E_ANIMSET_ATT2,
    E_ANIMSET_ATT3,
    E_ANIMSET_RUN,
    E_ANIMSET_IDLE
};

class cSkinnedMesh : public cObject
{
private:
	LPD3DXFRAME					m_pRoot;
	LPD3DXANIMATIONCONTROLLER	m_pAnimController;

    float						m_fBlendDuration;       // ���� ���� ���ؽð�
    float						m_fPassedBlendTime;     // ������ �ð�

public:
	cSkinnedMesh();
	~cSkinnedMesh();

	void Load(char* szFolder, char* szFile);
	void SetupBoneMatrixPtrs(LPD3DXFRAME pFrame);			// ppBoneMatrixPtrs ����(���� ó��)
	void Update();											// �ִϸ��̼��� ������ ���ؼ� ������Ʈ�� �ʿ� �ϴ�.
	void Update(LPD3DXFRAME pFrame, LPD3DXFRAME pParent);	// ��Ʈ���� ������Ʈ(�θ��� ������ �޾Ƽ� ������Ʈ)
	void UpdateSkinnedMesh(LPD3DXFRAME pFrame);				// �������� ���� ��ũ �޽� ����
	void Render(LPD3DXFRAME pFrame = NULL);
    void SetAnimation(int nIndex, bool isBlend);            // �ִϸ��̼� ��ȯ �Լ�
};

