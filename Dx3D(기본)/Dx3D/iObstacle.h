#pragma once
#include "stdafx.h"

/*
�������̽� : ���� ���� �Լ��� ����
- �������̽��� ��� ���� Ŭ�����鿡 ���� �ؾߵ� �Լ����� ���� ���� �Լ��� ������ ���´�.
*/

interface iObstacle
{
    virtual bool IsinObstacle(D3DXVECTOR3 vPos) PURE;
    virtual vector<D3DXVECTOR3> GetVertex() PURE;
};