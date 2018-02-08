#include "stdafx.h"
#include "cHeightMap.h"
#include "cMtlTex.h"

cHeightMap::cHeightMap()
    : m_pMesh(NULL)
    , m_pMtlTex(NULL)
{
}

cHeightMap::~cHeightMap()
{
    SAFE_RELEASE(m_pMesh);
}

void cHeightMap::Load(IN char* szFilePath, IN D3DXMATRIXA16* pMat)
{
    if (pMat)
    {
        m_fSizeX = pMat->_11;
        m_fSizeZ = pMat->_33;
    }

    // == MtlTex 정보 셋팅
    m_pMtlTex = new cMtlTex;
    g_pAutoReleasePool->AddObject(m_pMtlTex);
    m_pMtlTex->SetMtlRef(0);
    m_pMtlTex->SetMtl(D3DMATERIAL9(WHITE_MTRL));
    g_pTextureManager->AddTexture("HeightMapData/terrain.jpg", "HeightMapData/terrain.jpg");
    m_pMtlTex->SetTexture(g_pTextureManager->GetTexture("HeightMapData/terrain.jpg"));

    int nX = 0;
    int nZ = 0;
    vector<ST_PNT_VERTEX> vecPNT;

    FILE* pFile;
    fopen_s(&pFile, szFilePath, "rb");  //b -> 바이너리 (이진수)
    
    while (true)
    {
        int nY = fgetc(pFile);	        // 한 문자씩 읽는다.

        if (feof(pFile)) break;			// 오픈 한 파일이 끝났다면 종료

        // == 포지션 셋팅
        D3DXVECTOR3 vPos;
        vPos.x = (float)nX * m_fSizeX;
        vPos.z = (float)nZ * m_fSizeZ;
        vPos.y = (float)nY / 10.0f;
        m_vecVertex.push_back(vPos);

        // == PNT 버텍스 셋팅
        ST_PNT_VERTEX stPNT;
        stPNT.p = vPos;
        stPNT.n = D3DXVECTOR3(0, 1, 0);
        stPNT.t = D3DXVECTOR2((float)nX / 256, (float)nZ / 256);
        vecPNT.push_back(stPNT);

        if (++nX > 256)
        {
            nX = 0;
            ++nZ;
        }
    }

    fclose(pFile);
    
    vector<DWORD> vecIndex;
    // == 인덱스 구성
    for (int z = 0; z < 256; ++z)
    {
        for (int x = 0; x < 256; ++x)
        {
            // 0 1 2  1 3 2
            //0 = z * 257 + x
            //1 = (z + 1) * 257 + x
            //2 = z * 257 + x + 1
            //3 = (z + 1) * 257 + x + 1

            // 하단
            vecIndex.push_back(z * 257 + x);
            vecIndex.push_back((z + 1) * 257 + x);
            vecIndex.push_back(z * 257 + (x + 1));

            // 상단
            vecIndex.push_back((z + 1) * 257 + x);
            vecIndex.push_back((z + 1) * 257 + (x + 1));
            vecIndex.push_back(z * 257 + (x + 1));
        }
    }

    // 노말값 셋팅 -> 양 사이드 값은 (0, 1, 0)으로 기본 셋팅 == 바꾸지 않으면 기본 셋팅
    for (int z = 1; z < 257 - 1; ++z)
    {
        for (int x = 1; x < 257 - 1; ++x)
        {
            int index = z * 257 + x;
            D3DXVECTOR3 vCol, vRow, vNormal;    
            vCol = vecPNT[index + 257].p - vecPNT[index - 257].p;
            vRow = vecPNT[index + 1].p - vecPNT[index - 1].p;
            D3DXVec3Cross(&vNormal, &vCol, &vRow);
            D3DXVec3Normalize(&vNormal, &vNormal);
            vecPNT[index].n = vNormal;
        }
    }

    // == 매쉬 생성, 기록, 최적화
    // 생성
    D3DXCreateMeshFVF(vecIndex.size() / 3, vecPNT.size(), D3DXMESH_MANAGED | D3DXMESH_32BIT,
        ST_PNT_VERTEX::FVF, g_pD3DDevice, &m_pMesh);

    // 버텍스 버퍼 기록
    ST_PNT_VERTEX* pV = NULL;
    m_pMesh->LockVertexBuffer(NULL, (LPVOID*)&pV);
    memcpy(pV, &vecPNT[0], vecPNT.size() * sizeof(ST_PNT_VERTEX));
    m_pMesh->UnlockVertexBuffer();

    // 인덱스 버퍼 기록
    DWORD* pI = NULL;
    m_pMesh->LockIndexBuffer(NULL, (LPVOID*)&pI);
    memcpy(pI, &vecIndex[0], vecIndex.size() * sizeof(DWORD));
    m_pMesh->UnlockIndexBuffer();

    // 속성 버퍼 기록
    DWORD* pA = NULL;
    m_pMesh->LockAttributeBuffer(NULL, &pA);
    for (int i = 0; i < vecIndex.size() / 3; ++i) // 페이스별로 하나씩 기록
        pA[i] = m_pMtlTex->GetMtlRef(); // 메쉬가 하나임.
    m_pMesh->UnlockAttributeBuffer();

    // 메쉬 최적화 : 버텍스 개수 만큼 인접정보를 담을 공간을 확보
    vector<DWORD> vecAdjBuf(m_pMesh->GetNumFaces() * 3);

    m_pMesh->GenerateAdjacency(D3DX_16F_EPSILON, &vecAdjBuf[0]);

    m_pMesh->OptimizeInplace(D3DXMESHOPT_COMPACT | D3DXMESHOPT_ATTRSORT | D3DXMESHOPT_VERTEXCACHE,
        &vecAdjBuf[0], 0, 0, 0);

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


    */
}

// 캐릭터 지형 체크
bool cHeightMap::GetHeight(IN const float& x, OUT float& y, IN const float& z)
{
    int nX = int(x / m_fSizeX);
    int nZ = int(z / m_fSizeZ);

    // 캐릭터가 지형물 안에 없으면
    if (nX < 0 && nZ < 0 && nX >= 256 && nZ >= 256)
        return false;

    float fDeltaX = (x / m_fSizeX) - (float)nX;
    float fDeltaZ = (z / m_fSizeZ) - (float)nZ;
    // 아래쪽 삼각형
    if (fDeltaX + fDeltaZ < 1)
    {
        float fX_Y = (m_vecVertex[nZ * 257 + (nX + 1)].y - m_vecVertex[nZ * 257 + nX].y) * fDeltaX;     // X쪽 보간 Y값
        float fZ_Y = (m_vecVertex[(nZ + 1) * 257 + nX].y - m_vecVertex[nZ * 257 + nX].y) * fDeltaZ;     // Z쪽 보간 Y값
        y = m_vecVertex[nZ * 257 + nX].y + fX_Y + fZ_Y;
    }
    // 위쪽 삼각형
    else
    {
        float fX_Y = (m_vecVertex[(nZ + 1) * 257 + nX].y - m_vecVertex[(nZ + 1) * 257 + (nX + 1)].y) * (1.0f - fDeltaX);
        float fZ_Y = (m_vecVertex[nZ * 257 + (nX + 1)].y - m_vecVertex[(nZ + 1) * 257 + (nX + 1)].y) * (1.0f - fDeltaZ);
        y = m_vecVertex[(nZ + 1) * 257 + (nX + 1)].y + fX_Y + fZ_Y;
    }

    return true;

    //== == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == ==
    //    GetHeight 함수
    //    캐릭터의 높이 설정

    //    1. 기준 페이스 선택
    //    1--3	0의 x, z 좌표 찾기
    //    | \ | x = (int)캐릭터 위치x
    //    | \ | z = (int)캐릭터 위치z
    //    0--2
    //    deltaX = 위치x - x
    //    deltaZ = 위치z - z

    //    deltaX + deltaZ < 1 : 아래쪽 삼각형
    //    deltaX + deltaZ >= 1 : 윗쪽 삼각형

    //    두 점 사이의 높이차이 계산
    //    1--3
    //    | \ | 아래쪽 삼각형일 경우 : 0.y + x 축 높이 차이 * 델타x + z 축 높이 차이 * 델타z
    //    | \ | 윗쪽 삼각형일 경우 : 3.y + x 축 높이 차이 * (1.0f - 델타x) + z 축 높이 차이 * (1.0f - 델타z)
    //    0--2
}

// 픽킹 체크
bool cHeightMap::ColisionRay(IN D3DXVECTOR3* vOrigin, IN D3DXVECTOR3* vDir, OUT D3DXVECTOR3& vPos)
{
    float fMinDist = D3DX_16F_MAX;
    float fDistance;
    bool ret = false;

    // 삼각형에 따른 픽킹 체크
    for (int z = 0; z < 257 - 1; ++z)
    {
        for (int x = 0; x < 257 - 1; ++x)
        {
            int index = z * 257 + x;
            // 하단 삼각형
            if (D3DXIntersectTri(&m_vecVertex[index], &m_vecVertex[index + 257], &m_vecVertex[index + 1],
                vOrigin, vDir, NULL, NULL, &fDistance))
            {
                if (fDistance < fMinDist)
                {
                    fMinDist = fDistance;
                    D3DXVECTOR3 vDistance(*vDir * fDistance);
                    vPos = *vOrigin + vDistance;
                    ret = true;
                }
            }
            // 상단 삼각형
            else if (D3DXIntersectTri(&m_vecVertex[index + 257], &m_vecVertex[index + 257 + 1], &m_vecVertex[index + 1],
                vOrigin, vDir, NULL, NULL, &fDistance))
            {
                if (fDistance < fMinDist)
                {
                    fMinDist = fDistance;
                    D3DXVECTOR3 vDistance(*vDir * fDistance);
                    vPos = *vOrigin + vDistance;
                    ret = true;
                }
            }

        }
    }

    return ret;
}

void cHeightMap::Render()
{
    // 메쉬로 그리기
    D3DXMATRIXA16 matW;
    D3DXMatrixIdentity(&matW);
    g_pD3DDevice->SetTransform(D3DTS_WORLD, &matW);
    g_pD3DDevice->SetMaterial(m_pMtlTex->GetMtl());
    g_pD3DDevice->SetTexture(0, m_pMtlTex->GetTexture());
    m_pMesh->DrawSubset(m_pMtlTex->GetMtlRef());
}