#pragma once

#include "cObject.h"

class cPicking : public cObject
{
private:
    LPD3DXMESH          m_pMeshSphere;

    D3DXVECTOR3         m_vPickPos;
    
    bool                m_isPick;

public:
    cPicking();
    ~cPicking();

    void Setup();
    void Update(vector<Sphere>& spheres);
    void Render();
};

