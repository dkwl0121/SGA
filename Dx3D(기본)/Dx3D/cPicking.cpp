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
    // 구 만들기                // 반지름, 삼각형(페이스) 개수
    D3DXCreateSphere(g_pD3DDevice, 0.5f, 10, 10, &m_pMeshSphere, NULL);
}

void cPicking::Update(vector<Sphere>& spheres)
{
    if (g_pKeyManager->isOnceKeyDown(VK_LBUTTON))
    {
        cRay Ray;
        // 뷰 스페이스 좌표로 변환
        Ray.CalcPickingRay(g_ptMouse.x, g_ptMouse.y);
        // 월드 스페이스 좌표로 변환
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

        // 구 충돌
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

    // 픽킹 했으면
    if (m_isPick)
    {
        g_pD3DDevice->SetTransform(D3DTS_WORLD, &mat);
        g_pD3DDevice->SetTexture(0, NULL);
        g_pD3DDevice->SetMaterial(&YELLOW_MTRL);
        m_pMeshSphere->DrawSubset(0);
    }
}


/*
반직선 구조체 또는 클래스 생성
구 구조체 생성

마우스 클릭에 의한 반직선 생성
스크린 => 뷰 스페이스 (책 참조 15장 픽킹)
뷰 스페이스 => 월드 스페이스

바닥을 생성하고 바닥과 충돌 지점에 구 그리기
(면과 반직선의 충돌 함수 사용 : D3DXIntersectTri)

반직선과 구 충돌 (구 구조체에 픽킹 정보 추가)
랜덤으로 구를 많이 띠우기

/*
Q = 반직선의 출발점
V = 반직선의 방향

반직선: P(t) = Q + Vt
==> P(x) = Qx + Vxt, P(y) = Qy + Vyt, P(z) = Qz + Vzt

구: (x - a)^2 + (y - b)^2 + (z - c)^2 = r^2
구: x^2 + y^2 + z^2 = r^2

구와 반직선의 연립 방정식: (Qx + Vxt)^2 + (Qy + Vyt)^2 + (Qz + Vzt)^2 = r^2

(Qx + Vxt)^2 => (Qx)^2 + 2(Qx)(Vxt) + (Vxt)^2
=> ((Vx)^2)(t^2) + 2(Qx)(Vx)t + (Qx)^2

연립 방정식을 t에 대한 2차 방적식으로 정리
(Vx + Vy + Vz)^2 * t^2 + 2(QxVx + QyVy + QzVz)t+ (Qx^2 + Qy^2 + Qz^2) - r^2 = 0
(VV)t^2 + 2(QV)t + QQ - rr = 0

판별식 = b^2 - 4ac ==> 4(QV)^2 - 4(VV)(QQ - rr) = QV^2 - (VV)(QQ - rr)
판별식 >= 0 교점이 존재한다(충돌)

구와 반직선의 충돌시 구의 중점을 원점(0, 0, 0)으로 처리하여 계산하는 것이 편하다.
*/
