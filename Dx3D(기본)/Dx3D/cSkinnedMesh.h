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

    float						m_fBlendDuration;       // 블랜딩 지속 기준시간
    float						m_fPassedBlendTime;     // 블랜딩한 시간

public:
	cSkinnedMesh();
	~cSkinnedMesh();

	void Load(char* szFolder, char* szFile);
	void SetupBoneMatrixPtrs(LPD3DXFRAME pFrame);			// ppBoneMatrixPtrs 셋팅(연결 처리)
	void Update();											// 애니메이션을 돌리기 위해서 업데이트가 필요 하다.
	void Update(LPD3DXFRAME pFrame, LPD3DXFRAME pParent);	// 매트릭스 업데이트(부모의 영향을 받아서 업데이트)
	void UpdateSkinnedMesh(LPD3DXFRAME pFrame);				// 렌더링을 위한 워크 메쉬 변형
	void Render(LPD3DXFRAME pFrame = NULL);
    void SetAnimation(int nIndex, bool isBlend);            // 애니메이션 전환 함수
};

