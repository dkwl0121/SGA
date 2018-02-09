#pragma once

#include "cObject.h"

class cRay : public cObject
{
public:
    D3DXVECTOR3 m_vOrigin;
    D3DXVECTOR3 m_vDirection;

    cRay();
    ~cRay();

    void CalcPickingRay(int x, int y);
    void TransformRay(D3DXMATRIXA16* T);
    bool IntersectSphere(tagSphere sphere);

    // == ╟ыем ==
    D3DXVECTOR3 GetOrigin() { return m_vOrigin; }
    D3DXVECTOR3 GetDirection() { return m_vDirection; }
};

