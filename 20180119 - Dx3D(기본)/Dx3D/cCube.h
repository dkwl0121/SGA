#pragma once

#define BLANK_DISTANCE 1.0f

class cCube
{
private:
	D3DXMATRIXA16			m_matWorld;
	LPDIRECT3DTEXTURE9		m_pTexture;
	D3DMATERIAL9			m_stMaterial;

	vector<ST_PN_VERTEX>	m_vecPNVertex;
	vector<ST_PNT_VERTEX>	m_vecPNTVertex;
	vector<cCube*>			m_vecChild;

	D3DXVECTOR3				m_vLocalPos;
	float					m_fRotX;
	float					m_fRotXSpeed;
	float					m_fRotY;
	float					m_fRotYSpeed;

	bool					m_isMoving;
	D3DXVECTOR3				m_vCenter;		// ���� ���������� ������ ����

	void SetPNVertex(vector<D3DXVECTOR3>& vecV, vector<int>& vecI);
	void SetPNTVertex(vector<D3DXVECTOR3>& vecV, vector<int>& vecI,
		vector<D3DXVECTOR2>* vecT);

public:
	cCube();
	~cCube();

	void Setup(vector<D3DXVECTOR2>* vecT, string key, D3DMATERIAL9 material, D3DXMATRIXA16* pMat = NULL);
	void Update(D3DXMATRIXA16* pParent = NULL);
	void Render();
	void AddChild(cCube* pCube);
	void Release();

	void SetLocalPos(D3DXVECTOR3 v) { m_vLocalPos = v; }
	void SetRotSpeedX(float speed) { m_fRotXSpeed = speed; }
	void SetRotSpeedY(float speed) { m_fRotYSpeed = speed; }

	// ����
	// �����ӿ� ���� ������ �ڽĵ鵵 �Ȱ��� �����ϹǷ� ��� ������ �����.
	void SetIsMoving(bool move);
	void SetPosition(D3DXVECTOR3& pos) { m_vCenter = pos; }

	// ����
	D3DXVECTOR3 GetPosition() { return m_vCenter; }

	// �浹 ó��
	bool Collision(D3DXVECTOR3& center);

};

