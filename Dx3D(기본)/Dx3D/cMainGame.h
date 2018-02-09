#pragma once

class cCamera;
class cCharacterController;
class cMtlTex;
class cAseCharacter;
class cHeightMap;
class cGrid;
class cPicking;
class cPillar;

class cMainGame
{
private:
	cCamera*				m_pCamera;
	cCharacterController*	m_pController;
    cAseCharacter*          m_pAseCharacter;
    cHeightMap*             m_pHeightMap;
    cGrid*                  m_pGrid;
    cPicking*               m_pPicking;
    cPillar*                m_pPillar;

    LPD3DXMESH				m_pMeshMap;
    vector<cMtlTex*>		m_vecMtlTex;

	bool					m_IsTarget;		// 카메라가 타겟을 따라가야 하면

public:
	cMainGame();
	~cMainGame();

	void Setup();
	void Update();
	void Render();
};

