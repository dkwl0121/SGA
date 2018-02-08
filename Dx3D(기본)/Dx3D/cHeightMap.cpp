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

    // == MtlTex ���� ����
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
    fopen_s(&pFile, szFilePath, "rb");  //b -> ���̳ʸ� (������)
    
    while (true)
    {
        int nY = fgetc(pFile);	        // �� ���ھ� �д´�.

        if (feof(pFile)) break;			// ���� �� ������ �����ٸ� ����

        // == ������ ����
        D3DXVECTOR3 vPos;
        vPos.x = (float)nX * m_fSizeX;
        vPos.z = (float)nZ * m_fSizeZ;
        vPos.y = (float)nY / 10.0f;
        m_vecVertex.push_back(vPos);

        // == PNT ���ؽ� ����
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
    // == �ε��� ����
    for (int z = 0; z < 256; ++z)
    {
        for (int x = 0; x < 256; ++x)
        {
            // 0 1 2  1 3 2
            //0 = z * 257 + x
            //1 = (z + 1) * 257 + x
            //2 = z * 257 + x + 1
            //3 = (z + 1) * 257 + x + 1

            // �ϴ�
            vecIndex.push_back(z * 257 + x);
            vecIndex.push_back((z + 1) * 257 + x);
            vecIndex.push_back(z * 257 + (x + 1));

            // ���
            vecIndex.push_back((z + 1) * 257 + x);
            vecIndex.push_back((z + 1) * 257 + (x + 1));
            vecIndex.push_back(z * 257 + (x + 1));
        }
    }

    // �븻�� ���� -> �� ���̵� ���� (0, 1, 0)���� �⺻ ���� == �ٲ��� ������ �⺻ ����
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

    // == �Ž� ����, ���, ����ȭ
    // ����
    D3DXCreateMeshFVF(vecIndex.size() / 3, vecPNT.size(), D3DXMESH_MANAGED | D3DXMESH_32BIT,
        ST_PNT_VERTEX::FVF, g_pD3DDevice, &m_pMesh);

    // ���ؽ� ���� ���
    ST_PNT_VERTEX* pV = NULL;
    m_pMesh->LockVertexBuffer(NULL, (LPVOID*)&pV);
    memcpy(pV, &vecPNT[0], vecPNT.size() * sizeof(ST_PNT_VERTEX));
    m_pMesh->UnlockVertexBuffer();

    // �ε��� ���� ���
    DWORD* pI = NULL;
    m_pMesh->LockIndexBuffer(NULL, (LPVOID*)&pI);
    memcpy(pI, &vecIndex[0], vecIndex.size() * sizeof(DWORD));
    m_pMesh->UnlockIndexBuffer();

    // �Ӽ� ���� ���
    DWORD* pA = NULL;
    m_pMesh->LockAttributeBuffer(NULL, &pA);
    for (int i = 0; i < vecIndex.size() / 3; ++i) // ���̽����� �ϳ��� ���
        pA[i] = m_pMtlTex->GetMtlRef(); // �޽��� �ϳ���.
    m_pMesh->UnlockAttributeBuffer();

    // �޽� ����ȭ : ���ؽ� ���� ��ŭ ���������� ���� ������ Ȯ��
    vector<DWORD> vecAdjBuf(m_pMesh->GetNumFaces() * 3);

    m_pMesh->GenerateAdjacency(D3DX_16F_EPSILON, &vecAdjBuf[0]);

    m_pMesh->OptimizeInplace(D3DXMESHOPT_COMPACT | D3DXMESHOPT_ATTRSORT | D3DXMESHOPT_VERTEXCACHE,
        &vecAdjBuf[0], 0, 0, 0);

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


    */
}

// ĳ���� ���� üũ
bool cHeightMap::GetHeight(IN const float& x, OUT float& y, IN const float& z)
{
    int nX = int(x / m_fSizeX);
    int nZ = int(z / m_fSizeZ);

    // ĳ���Ͱ� ������ �ȿ� ������
    if (nX < 0 && nZ < 0 && nX >= 256 && nZ >= 256)
        return false;

    float fDeltaX = (x / m_fSizeX) - (float)nX;
    float fDeltaZ = (z / m_fSizeZ) - (float)nZ;
    // �Ʒ��� �ﰢ��
    if (fDeltaX + fDeltaZ < 1)
    {
        float fX_Y = (m_vecVertex[nZ * 257 + (nX + 1)].y - m_vecVertex[nZ * 257 + nX].y) * fDeltaX;     // X�� ���� Y��
        float fZ_Y = (m_vecVertex[(nZ + 1) * 257 + nX].y - m_vecVertex[nZ * 257 + nX].y) * fDeltaZ;     // Z�� ���� Y��
        y = m_vecVertex[nZ * 257 + nX].y + fX_Y + fZ_Y;
    }
    // ���� �ﰢ��
    else
    {
        float fX_Y = (m_vecVertex[(nZ + 1) * 257 + nX].y - m_vecVertex[(nZ + 1) * 257 + (nX + 1)].y) * (1.0f - fDeltaX);
        float fZ_Y = (m_vecVertex[nZ * 257 + (nX + 1)].y - m_vecVertex[(nZ + 1) * 257 + (nX + 1)].y) * (1.0f - fDeltaZ);
        y = m_vecVertex[(nZ + 1) * 257 + (nX + 1)].y + fX_Y + fZ_Y;
    }

    return true;

    //== == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == ==
    //    GetHeight �Լ�
    //    ĳ������ ���� ����

    //    1. ���� ���̽� ����
    //    1--3	0�� x, z ��ǥ ã��
    //    | \ | x = (int)ĳ���� ��ġx
    //    | \ | z = (int)ĳ���� ��ġz
    //    0--2
    //    deltaX = ��ġx - x
    //    deltaZ = ��ġz - z

    //    deltaX + deltaZ < 1 : �Ʒ��� �ﰢ��
    //    deltaX + deltaZ >= 1 : ���� �ﰢ��

    //    �� �� ������ �������� ���
    //    1--3
    //    | \ | �Ʒ��� �ﰢ���� ��� : 0.y + x �� ���� ���� * ��Ÿx + z �� ���� ���� * ��Ÿz
    //    | \ | ���� �ﰢ���� ��� : 3.y + x �� ���� ���� * (1.0f - ��Ÿx) + z �� ���� ���� * (1.0f - ��Ÿz)
    //    0--2
}

// ��ŷ üũ
bool cHeightMap::ColisionRay(IN D3DXVECTOR3* vOrigin, IN D3DXVECTOR3* vDir, OUT D3DXVECTOR3& vPos)
{
    float fMinDist = D3DX_16F_MAX;
    float fDistance;
    bool ret = false;

    // �ﰢ���� ���� ��ŷ üũ
    for (int z = 0; z < 257 - 1; ++z)
    {
        for (int x = 0; x < 257 - 1; ++x)
        {
            int index = z * 257 + x;
            // �ϴ� �ﰢ��
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
            // ��� �ﰢ��
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
    // �޽��� �׸���
    D3DXMATRIXA16 matW;
    D3DXMatrixIdentity(&matW);
    g_pD3DDevice->SetTransform(D3DTS_WORLD, &matW);
    g_pD3DDevice->SetMaterial(m_pMtlTex->GetMtl());
    g_pD3DDevice->SetTexture(0, m_pMtlTex->GetTexture());
    m_pMesh->DrawSubset(m_pMtlTex->GetMtlRef());
}