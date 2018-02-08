#pragma once

#include "cObject.h"

class iMap;

class cPicking : public cObject
{
private:
    //LPD3DXMESH          m_pMeshSphere;
    D3DXVECTOR3         m_vPickPos;
    
    bool                m_isPick;

public:
    cPicking();
    ~cPicking();

    void Setup();
    void Update(iMap* pMap = NULL);
    //void Render();

    // ╟ыем
    bool GetIsPick() { return m_isPick; }
    D3DXVECTOR3 GetPickPos() { return m_vPickPos; }

    // ╪бем
    void SetIsPick(bool pick) { m_isPick = pick; }
};

