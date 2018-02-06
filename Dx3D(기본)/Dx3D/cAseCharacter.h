#pragma once
#include "cObject.h"
#include "cFrame.h"

class cAseCharacter : public cObject
{
private:
	ST_ASE_SCENE	m_stScene;
	cFrame*			m_pRootFrame;

    int             m_nFirstTick;
    int             m_nLastTick;
    int             m_nTickSpeed;

public:
	cAseCharacter();
	~cAseCharacter();

	void Load(char* szFullPath);
	void Update(D3DXMATRIXA16* matWorld = NULL);
	void Render();
};

