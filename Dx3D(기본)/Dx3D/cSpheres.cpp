#include "stdafx.h"
#include "cSpheres.h"


cSpheres::cSpheres()
    : m_pMeshSphere(NULL)
{
}


cSpheres::~cSpheres()
{
    SAFE_RELEASE(m_pMeshSphere);
}

void cSpheres::Setup()
{
    Sphere stSphere;
    stSphere.fRadius = 1.0f;
    stSphere.isPicking = false;
    D3DXCreateSphere(g_pD3DDevice, stSphere.fRadius, 10, 10, &m_pMeshSphere, NULL);

    for (int i = 0; i < 30; ++i)
    {
        int nX = rand() % 40 - 20;
        int nY = rand() % 40 - 20;
        int nZ = rand() % 40 - 20;
        stSphere.vPos = D3DXVECTOR3(nX, nY, nZ);
        m_vecSpheres.push_back(stSphere);
    }
}

void cSpheres::Render()
{
    for each (auto iter in m_vecSpheres)
    {
        if (iter.isPicking)
            g_pD3DDevice->SetMaterial(&RED_MTRL);
        else
            g_pD3DDevice->SetMaterial(&WHITE_MTRL);

        D3DXMATRIXA16 matW;
        D3DXMatrixTranslation(&matW, iter.vPos.x, iter.vPos.y, iter.vPos.z);
        g_pD3DDevice->SetTransform(D3DTS_WORLD, &matW);
        g_pD3DDevice->SetTexture(0, NULL);
        m_pMeshSphere->DrawSubset(0);
    }
}
