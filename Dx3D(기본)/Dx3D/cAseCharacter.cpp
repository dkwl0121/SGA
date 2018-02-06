#include "stdafx.h"
#include "cAseCharacter.h"
#include "cAseLoader.h"


cAseCharacter::cAseCharacter()
	: m_pRootFrame(NULL)
{
}


cAseCharacter::~cAseCharacter()
{
}

void cAseCharacter::Load(char* szFullPath)
{
	cAseLoader* pAseLoader = new cAseLoader;
	m_pRootFrame = pAseLoader->Load(szFullPath, &m_stScene);
	SAFE_RELEASE(pAseLoader);

    if (m_pRootFrame)
        m_pRootFrame->CalcLocalTM();

    m_nFirstTick = m_stScene.nFirstFrame * m_stScene.nTicksPerFrame;
    m_nLastTick = m_stScene.nLastFrame * m_stScene.nTicksPerFrame;
    m_nTickSpeed = m_stScene.nFrameSpeed * m_stScene.nTicksPerFrame;
}

void cAseCharacter::Update(D3DXMATRIXA16* matWorld/*=NULL*/)
{
	// 키프레임 계산
	int nKeyFrame = (m_nTickSpeed * GetTickCount() / 1000) % (m_nLastTick - m_nFirstTick) + m_nFirstTick;
	// 640 ~ 3199 반복

	// 캐릭터 크기, 초기 바라보는 방향 설정이 필요하면 여기서 한다.
    D3DXMATRIXA16 matW, matS, matR;
    D3DXMatrixScaling(&matS, 5.0f, 5.0f, 5.0f);
    D3DXMatrixRotationY(&matR, D3DX_PI);

    matW = matS * matR;
    
    if (matWorld)
        matW *= *matWorld;

	if (m_pRootFrame)
		m_pRootFrame->Update(nKeyFrame, &matW);
}

void cAseCharacter::Render()
{
	if (m_pRootFrame)
		m_pRootFrame->Render();
}
