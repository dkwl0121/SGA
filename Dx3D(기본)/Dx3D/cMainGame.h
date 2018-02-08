#pragma once

class cCamera;
class cCharacterController;
class cObjMap;
class cMtlTex;
class cAseCharacter;
class cHeightMap;
class cGrid;
class cPicking;
class cAster;

class cMainGame
{
private:
	cCamera*				m_pCamera;
	cCharacterController*	m_pController;
    cAseCharacter*          m_pAseCharacter;
    cObjMap*				m_pObjMap;
    cHeightMap*             m_pHeightMap;
    cGrid*                  m_pGrid;
    cPicking*               m_pPicking;
    cAster*                 m_pAster;

    LPD3DXMESH				m_pMeshMap;
    vector<cMtlTex*>		m_vecMtlTex;

	bool					m_IsTarget;		// ī�޶� Ÿ���� ���󰡾� �ϸ�

public:
	cMainGame();
	~cMainGame();

	void Setup();
	void Update();
	void Render();
};

