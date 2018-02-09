#pragma once

#include "cObject.h"
#include "iObstacle.h"

class cCube;

class cPillar : public cObject, public iObstacle
{
private:
    vector<cCube*>      m_vecCube;

    vector<D3DXVECTOR3> m_vecVertex;

public:
    cPillar();
    ~cPillar();

    void Setup();
    void Render();

    virtual bool IsinObstacle(D3DXVECTOR3 vPos) override;
    virtual vector<D3DXVECTOR3> GetVertex() override;
};

