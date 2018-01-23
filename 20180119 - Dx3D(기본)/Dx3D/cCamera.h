#pragma once

#define LOOKAT_POS 3.0f
#define LIMITED_ROT 0.45f

class cCamera
{
private:
	float				m_fDistance;			// 큐브 기준 카메라 거리
	D3DXVECTOR3			m_vEye;					// 카메라 위치
	D3DXVECTOR3			m_vLookAt;				// 바라보는 위치
	D3DXVECTOR3			m_vUp;					// 임의의 업 벡터
	D3DXVECTOR3			m_vPosition;			// 카메라 위치

	D3DXMATRIXA16		m_matView;
	D3DXMATRIXA16		m_matProj;

	float				m_fRotX;
	float				m_fRotY;
	bool				m_isRButtonDown;
	POINT				m_ptPrevMouse;

public:
	cCamera();
	~cCamera();

	void Setup();
	void Update(D3DXVECTOR3* pTarget = NULL);

	D3DXMATRIXA16 GetViewProjMatrix() { return m_matView * m_matProj; }
};

