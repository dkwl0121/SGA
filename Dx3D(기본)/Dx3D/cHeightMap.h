#pragma once
#include "cObject.h"
#include "iMap.h"

#define VERTEX_CNT          257
#define TILE_CNT            (VERTEX_CNT - 1)
#define PART_CNT            4
#define PART_VERTEX_CNT     (TILE_CNT / PART_CNT)

class cMtlTex;

// 파트 나누기
struct tagPart {
    vector<D3DXVECTOR3>     vecVertex;      // 파트 삼각형 버텍스(2개)
    int                     nMinIndexZ;     // 맵 좌표 인덱스(minz)
    int                     nMinIndexX;     // 맵 좌표 인덱스(minx)
    int                     nMaxIndexZ;     // 맵 좌표 인덱스(maxz)
    int                     nMaxIndexX;     // 맵 좌표 인덱스(maxx)
};

class cHeightMap : public cObject, public iMap
{
private:
    vector<D3DXVECTOR3>     m_vecVertex;

    vector<tagPart>         m_vecStPart;
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
    virtual vector<D3DXVECTOR3> GetVertex() override;
	void Render();
};

/*
fopen 으로 읽어서 fgetc 를 사용하여 픽셀 하나씩 정보를 받는다.
char : 1byte, 8bit ==> 2^8 = 0 ~ 255

하이트맵 사이즈 257 * 257

버텍스 갯수 257 * 257
타일의 갯수 256 * 256

1. PNT	=> P : X, Z 읽어드린 순서대로 
			인덱스 번호 = z * 257 + x
			y = 색 정보 / 10.0f
		=> N : 0, 1, 0
		=> T : 0 ~ 1

2. 인덱스 구성 (시계 방향)
1--3	0 = z * 257 + x
|\ |	1 = (z + 1) * 257 + x
| \|	2 = z * 257 + x + 1
0--2	3 = (z + 1) * 257 + x + 1

3. 노말값 셋팅
---u---
|\ |\ |
| \| \|
L--n--r
|\ |\ |
| \| \|
---d---
du 벡터와 lr 벡터를 외적해서 현재 위치의 노말 값을 구한다.

4. 메쉬 생성 및 기록, 최적화

5. MtlTex 정보 셋팅

================================================================
GetHeight 함수
캐릭터의 높이 설정

1. 기준 페이스 선택
1--3	0의 x, z 좌표 찾기
|\ |	x = (int)캐릭터 위치x
| \|	z = (int)캐릭터 위치z
0--2
deltaX = 위치x - x
deltaZ = 위치z - z

deltaX + deltaZ < 1 : 아래쪽 삼각형
deltaX + deltaZ >= 1 : 윗쪽 삼각형

두 점 사이의 높이차이 계산
1--3
|\ |	아래쪽 삼각형일 경우 : 0.y + x 축 높이 차이 * 델타x + z 축 높이 차이 * 델타z
| \|	윗쪽 삼각형일 경우 : 3.y + x 축 높이 차이 * (1.0f - 델타x) + z 축 높이 차이 * (1.0f - 델타z)
0--2	
*/