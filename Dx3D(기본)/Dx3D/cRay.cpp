#include "stdafx.h"
#include "cRay.h"


cRay::cRay()
    : m_vOrigin(0, 0, 0)
    , m_vDirection(0, 0, 0)
{
}


cRay::~cRay()
{
}

// ��ŷ���� ��ȯ (��ũ�� �����̽����� -> �� �����̽��� ��ȯ)
void cRay::CalcPickingRay(int x, int y) // ���콺 ��ǥ
{
    float px = 0.0f;
    float py = 0.0f;

    D3DVIEWPORT9 vp;
    g_pD3DDevice->GetViewport(&vp);

    D3DXMATRIXA16 proj;
    g_pD3DDevice->GetTransform(D3DTS_PROJECTION, &proj);

    px = (((2.0f * x) / vp.Width) - 1.0f) / proj(0, 0);
    py = (((-2.0f * y) / vp.Height) + 1.0f) / proj(1, 1);

    m_vOrigin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    m_vDirection = D3DXVECTOR3(px, py, 1.0f);
}

// ������ ��ȯ (�� �Լ��� �� �����̽����� ���� �����̽��� ��ȯ�� �� ����) ->T���� �佺���̽� ��Ʈ������ ����� �϶�
void cRay::TransformRay(D3DXMATRIXA16 * T)
{
    // ������ ������ ��ȯ
    D3DXVec3TransformCoord(&m_vOrigin, &m_vOrigin, T);
    // ������ ������ ��ȯ
    D3DXVec3TransformNormal(&m_vDirection, &m_vDirection, T);
    // ���� ����ȭ
    D3DXVec3Normalize(&m_vDirection, &m_vDirection);
}

// ������ ��ü�� ������ ���
bool cRay::IntersectSphere(Sphere sphere)
{
    // ���� ������ -> ���� ������ (0, 0, 0)���� �ű�� ��ŭ ������ �������� �̵�
    D3DXVECTOR3 vQ = m_vOrigin - sphere.vPos;
    // ���� ����
    D3DXVECTOR3 vV = m_vDirection;

    float QV = D3DXVec3Dot(&vQ, &vV);
    float QQ = D3DXVec3Dot(&vQ, &vQ);
    float VV = D3DXVec3Dot(&vV, &vV);

    // �� �浹�� ������
    if ((QV * QV) - (VV * (QQ - (sphere.fRadius * sphere.fRadius))) >= 0)
        return true;
    else
        return false;

    //4(QV) ^ 2 - 4(VV)(QQ - rr) = QV ^ 2 - (VV)(QQ - rr)

    /*
    ������: P(t) = Q + Vt
    ==> P(x) = Qx + Vxt,
    P(y) = Qy + Vyt,
    P(z) = Qz + Vzt

    ��: (x - a)^2 + (y - b)^2 + (z - c)^2 = r^2   --> ���� ������ (0, 0, 0)���� �̵� ������ ��
    ��: x^2 + y^2 + z^2 = r^2                     --> ���� ������ (0, 0, 0)���� �̵����� ��

    ���� �������� ���� ������: (Qx + Vxt)^2 + (Qy + Vyt)^2 + (Qz + Vzt)^2 = r^2

    (Qx + Vxt)^2 => (Qx)^2 + 2(Qx)(Vxt) + (Vxt)^2
    => ((Vx)^2)(t^2) + 2(Qx)(Vx)t + (Qx)^2

    ���� �������� t�� ���� 2�� ���������� ����
    (Vx + Vy + Vz)^2 * t^2 + 2(QxVx + QyVy + QzVz)t+ (Qx^2 + Qy^2 + Qz^2) - r^2 = 0
    (VV)t^2 + 2(QV)t + QQ - rr = 0

    �Ǻ��� = b^2 - 4ac ==> 4(QV)^2 - 4(VV)(QQ - rr) = QV^2 - (VV)(QQ - rr)
    �Ǻ��� >= 0 ������ �����Ѵ�(�浹)

    ���� �������� �浹�� ���� ������ ����(0, 0, 0)���� ó���Ͽ� ����ϴ� ���� ���ϴ�.
    */
}
