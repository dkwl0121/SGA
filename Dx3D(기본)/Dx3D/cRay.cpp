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

// 픽킹레이 변환 (스크린 스페이스에서 -> 뷰 스페이스로 변환)
void cRay::CalcPickingRay(int x, int y) // 마우스 좌표
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

// 광선의 변환 (이 함수는 뷰 스페이스에서 월드 스페이스로 변환할 때 쓰임) ->T값이 뷰스페이스 메트릭스의 역행렬 일때
void cRay::TransformRay(D3DXMATRIXA16 * T)
{
    // 광선의 원점을 변환
    D3DXVec3TransformCoord(&m_vOrigin, &m_vOrigin, T);
    // 광선의 방향을 변환
    D3DXVec3TransformNormal(&m_vDirection, &m_vDirection, T);
    // 방향 정규화
    D3DXVec3Normalize(&m_vDirection, &m_vDirection);
}

// 광선이 구체를 교차할 경우
bool cRay::IntersectSphere(Sphere sphere)
{
    // 레이 오리진 -> 구의 중점을 (0, 0, 0)으로 옮기는 만큼 레이의 오리진도 이동
    D3DXVECTOR3 vQ = m_vOrigin - sphere.vPos;
    // 레이 방향
    D3DXVECTOR3 vV = m_vDirection;

    float QV = D3DXVec3Dot(&vQ, &vV);
    float QQ = D3DXVec3Dot(&vQ, &vQ);
    float VV = D3DXVec3Dot(&vV, &vV);

    // 구 충돌을 했으면
    if ((QV * QV) - (VV * (QQ - (sphere.fRadius * sphere.fRadius))) >= 0)
        return true;
    else
        return false;

    //4(QV) ^ 2 - 4(VV)(QQ - rr) = QV ^ 2 - (VV)(QQ - rr)

    /*
    반직선: P(t) = Q + Vt
    ==> P(x) = Qx + Vxt,
    P(y) = Qy + Vyt,
    P(z) = Qz + Vzt

    구: (x - a)^2 + (y - b)^2 + (z - c)^2 = r^2   --> 구의 중점을 (0, 0, 0)으로 이동 안했을 때
    구: x^2 + y^2 + z^2 = r^2                     --> 구의 중점을 (0, 0, 0)으로 이동했을 때

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
}
