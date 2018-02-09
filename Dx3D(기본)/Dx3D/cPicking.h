#pragma once

#include "cObject.h"

class cRay;

class cPicking : public cObject
{
private:
    cRay*               m_pRay;

    //D3DXVECTOR3         m_vPickPos;
    
    bool                m_isPick;

public:
    cPicking();
    ~cPicking();

    void Update();

    // ����
    bool GetIsPick() { return m_isPick; }
    cRay* GetRay() { return m_pRay; }

    // ����
    void SetIsPick(bool pick) { m_isPick = pick; }
};

