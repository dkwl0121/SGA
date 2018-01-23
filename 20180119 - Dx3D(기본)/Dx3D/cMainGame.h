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

	bool					m_IsTarget;		// 카메라가 타겟을 따라가야 하면

public:
	cMainGame();
	~cMainGame();

	void Setup();
	void Update();
	void Render();
};

