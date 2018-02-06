#pragma once

#include "cObject.h"

class cSpheres : public cObject
{
private:
    LPD3DXMESH              m_pMeshSphere;

    vector<Sphere>          m_vecSpheres;

public:
    cSpheres();
    ~cSpheres();

    void Setup();
    void Render();

    // == ���� ==
    vector<Sphere>& GetSphere() { return m_vecSpheres; }
};

