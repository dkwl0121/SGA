#include "stdafx.h"
#include "cPillar.h"
#include "cCube.h"

cPillar::cPillar()
{
}

cPillar::~cPillar()
{
}

void cPillar::Setup()
{
    cCube* pCube;
    D3DXMATRIXA16 matS, matT, matST;
    D3DXMatrixScaling(&matS, 10.0f, 40.0f, 10.0f);
    for (int i = 0; i < 20; ++i)
    {
        pCube = new cCube;
        g_pAutoReleasePool->AddObject(pCube);
        int x = rand() % 257;
        int z = rand() % 257;
        D3DXMatrixTranslation(&matT, x, 20.0f, z);
        matST = matS * matT;
        pCube->Setup(NULL, "", BLUE_MTRL, &matST);
        m_vecCube.push_back(pCube);

        vector<D3DXVECTOR3> vertex = pCube->GetVertex();
        for each (auto iter in vertex)
        {
            m_vecVertex.push_back(iter);
        }
    }
}

void cPillar::Render()
{
    for each (auto iter in m_vecCube)
        iter->Render();
}

bool cPillar::IsinObstacle(D3DXVECTOR3 vPos)
{
    bool isIn = false;

    for each (auto iter in m_vecCube)
    {
        if (iter->CollisionPos(vPos))
        {
            isIn = true;
            break;
        }
    }

    return isIn;
}

vector<D3DXVECTOR3> cPillar::GetVertex()
{
    return m_vecVertex;
}
