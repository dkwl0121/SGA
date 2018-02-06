#include "stdafx.h"
#include "cPicking.h"
#include "cRay.h"

cPicking::cPicking()
    : m_pMeshSphere(NULL)
    , m_vPickPos(0, 0, 0)
    , m_isPick(false)
{
}

cPicking::~cPicking()
{
    SAFE_RELEASE(m_pMeshSphere);
}

void cPicking::Setup()
{
    // �� �����                // ������, �ﰢ��(���̽�) ����
    D3DXCreateSphere(g_pD3DDevice, 0.5f, 10, 10, &m_pMeshSphere, NULL);
}

void cPicking::Update(vector<Sphere>& spheres)
{
    if (g_pKeyManager->isOnceKeyDown(VK_LBUTTON))
    {
        cRay Ray;
        // �� �����̽� ��ǥ�� ��ȯ
        Ray.CalcPickingRay(g_ptMouse.x, g_ptMouse.y);
        // ���� �����̽� ��ǥ�� ��ȯ
        D3DXMATRIXA16 matView;
        g_pD3DDevice->GetTransform(D3DTS_VIEW, &matView);
        D3DXMatrixInverse(&matView, NULL, &matView);
        Ray.TransformRay(&matView);

        float fDistance;

        if (D3DXIntersectTri(&D3DXVECTOR3(-5, 0, -5), &D3DXVECTOR3(-5, 0, 5), &D3DXVECTOR3(5, 0, 5),
            &Ray.GetOrigin(), &Ray.GetDirection(), NULL, NULL, &fDistance))
        {
            D3DXVECTOR3 vDistance(Ray.GetDirection() * fDistance);
            m_vPickPos = Ray.GetOrigin() + vDistance;
            m_isPick = true;
        }
        else if (D3DXIntersectTri(&D3DXVECTOR3(-5, 0, -5), &D3DXVECTOR3(5, 0, 5), &D3DXVECTOR3(5, 0, -5),
            &Ray.GetOrigin(), &Ray.GetDirection(), NULL, NULL, &fDistance))
        {
            D3DXVECTOR3 vDistance(Ray.GetDirection() * fDistance);
            m_vPickPos = Ray.GetOrigin() + vDistance;
            m_isPick = true;
        }

        // �� �浹
        for (int i = 0; i < spheres.size(); ++i)
        {
            spheres[i].isPicking = Ray.IntersectSphere(spheres[i]);
        }
    }
}

void cPicking::Render()
{
    D3DXMATRIXA16 mat;
    D3DXMatrixTranslation(&mat, m_vPickPos.x, m_vPickPos.y, m_vPickPos.z);

    // ��ŷ ������
    if (m_isPick)
    {
        g_pD3DDevice->SetTransform(D3DTS_WORLD, &mat);
        g_pD3DDevice->SetTexture(0, NULL);
        g_pD3DDevice->SetMaterial(&YELLOW_MTRL);
        m_pMeshSphere->DrawSubset(0);
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
