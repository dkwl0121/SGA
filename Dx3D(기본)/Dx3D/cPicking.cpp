#include "stdafx.h"
#include "cPicking.h"
#include "cRay.h"
#include "iMap.h"

cPicking::cPicking()
    : m_isPick(false)
{
    m_pRay = new cRay;
    g_pAutoReleasePool->AddObject(m_pRay);
}

cPicking::~cPicking()
{
}

void cPicking::Update()
{
    if (g_pKeyManager->isOnceKeyDown(VK_LBUTTON))
    {
        // �� �����̽� ��ǥ�� ��ȯ
        m_pRay->CalcPickingRay(g_ptMouse.x, g_ptMouse.y);
        // ���� �����̽� ��ǥ�� ��ȯ
        D3DXMATRIXA16 matView;
        g_pD3DDevice->GetTransform(D3DTS_VIEW, &matView);
        D3DXMatrixInverse(&matView, NULL, &matView);
        m_pRay->TransformRay(&matView);
        m_isPick = true;
    }
}

/*
������ ����ü �Ǵ� Ŭ���� ����
�� ����ü ����

���콺 Ŭ���� ���� ������ ����
��ũ�� => �� �����̽� (å ���� 15�� ��ŷ)
�� �����̽� => ���� �����̽�

�ٴ��� �����ϰ� �ٴڰ� �浹 ������ �� �׸���
(��� �������� �浹 �Լ� ��� : D3DXIntersectTri)

�������� �� �浹 (�� ����ü�� ��ŷ ���� �߰�)
�������� ���� ���� ����

/*
Q = �������� �����
V = �������� ����

������: P(t) = Q + Vt
==> P(x) = Qx + Vxt, P(y) = Qy + Vyt, P(z) = Qz + Vzt

��: (x - a)^2 + (y - b)^2 + (z - c)^2 = r^2
��: x^2 + y^2 + z^2 = r^2

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
