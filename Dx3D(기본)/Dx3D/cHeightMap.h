#pragma once
#include "cObject.h"
#include "iMap.h"

class cMtlTex;

class cHeightMap : public cObject, public iMap
{
    SYNTHESIZE_PASS_BY_REF(vector<D3DXVECTOR3>, m_vecVertex, Vertex);

private:
	LPD3DXMESH				m_pMesh;
	cMtlTex*				m_pMtlTex;
    
	float					m_fSizeX;
	float					m_fSizeZ;
    
public:
	cHeightMap();
	~cHeightMap();

	virtual void Load(IN char* szFilePath, IN D3DXMATRIXA16* pMat) override;
	virtual bool GetHeight(IN const float& x, OUT float& y, IN const float& z) override;
    virtual bool ColisionRay(IN D3DXVECTOR3* vOrigin, IN D3DXVECTOR3* vDir, OUT D3DXVECTOR3& vPos) override;
	void Render();
};

/*
fopen ���� �о fgetc �� ����Ͽ� �ȼ� �ϳ��� ������ �޴´�.
char : 1byte, 8bit ==> 2^8 = 0 ~ 255

����Ʈ�� ������ 257 * 257

���ؽ� ���� 257 * 257
Ÿ���� ���� 256 * 256

1. PNT	=> P : X, Z �о�帰 ������� 
			�ε��� ��ȣ = z * 257 + x
			y = �� ���� / 10.0f
		=> N : 0, 1, 0
		=> T : 0 ~ 1

2. �ε��� ���� (�ð� ����)
1--3	0 = z * 257 + x
|\ |	1 = (z + 1) * 257 + x
| \|	2 = z * 257 + x + 1
0--2	3 = (z + 1) * 257 + x + 1

3. �븻�� ����
---u---
|\ |\ |
| \| \|
L--n--r
|\ |\ |
| \| \|
---d---
du ���Ϳ� lr ���͸� �����ؼ� ���� ��ġ�� �븻 ���� ���Ѵ�.

4. �޽� ���� �� ���, ����ȭ

5. MtlTex ���� ����

================================================================
GetHeight �Լ�
ĳ������ ���� ����

1. ���� ���̽� ����
1--3	0�� x, z ��ǥ ã��
|\ |	x = (int)ĳ���� ��ġx
| \|	z = (int)ĳ���� ��ġz
0--2
deltaX = ��ġx - x
deltaZ = ��ġz - z

deltaX + deltaZ < 1 : �Ʒ��� �ﰢ��
deltaX + deltaZ >= 1 : ���� �ﰢ��

�� �� ������ �������� ���
1--3
|\ |	�Ʒ��� �ﰢ���� ��� : 0.y + x �� ���� ���� * ��Ÿx + z �� ���� ���� * ��Ÿz
| \|	���� �ﰢ���� ��� : 3.y + x �� ���� ���� * (1.0f - ��Ÿx) + z �� ���� ���� * (1.0f - ��Ÿz)
0--2	
*/