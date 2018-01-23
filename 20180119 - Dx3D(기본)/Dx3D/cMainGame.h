#pragma once

class cCamera;
class cGrid;
class cCharacterController;
class cCubeMan;

class cMainGame
{
private:
	cCamera*				m_pCamera;
	cGrid*					m_pGrid;
	cCharacterController*	m_pController;
	cCubeMan*				m_pCubeMan;

	bool					m_IsTarget;		// ī�޶� Ÿ���� ���󰡾� �ϸ�

public:
	cMainGame();
	~cMainGame();

	void Setup();
	void Update();
	void Render();
};

