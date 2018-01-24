#pragma once

class cCamera;
class cGrid;
class cCharacterController;
class cCubeMan;
class cGroup;
class cObjMap;

class cMainGame
{
private:
	cCamera*				m_pCamera;
	cGrid*					m_pGrid;
	cCharacterController*	m_pController;
	cCubeMan*				m_pCubeMan;
    cObjMap*				m_pObjMap;

    vector<cGroup*>			m_vecGroup;

	bool					m_IsTarget;		// ī�޶� Ÿ���� ���󰡾� �ϸ�

public:
	cMainGame();
	~cMainGame();

	void Setup();
	void Update();
	void Render();
};

