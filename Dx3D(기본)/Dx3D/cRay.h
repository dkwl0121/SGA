#pragma once

#include "cObject.h"

class cRay : public cObject
{
private:
    D3DXVECTOR3 m_vOrigin;
    D3DXVECTOR3 m_vDirection;

public:
    cRay();
    ~cRay();

    void CalcPickingRay(int x, int y);
    void TransformRay(D3DXMATRIXA16* T);
    bool IntersectSphere(Sphere sphere);

    // == ╟ыем ==
    D3DXVECTOR3 GetOrigin() { return m_vOrigin; }
    D3DXVECTOR3 GetDirection() { return m_vDirection; }
};

