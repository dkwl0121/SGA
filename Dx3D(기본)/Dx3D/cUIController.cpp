#include "stdafx.h"
#include "cUIController.h"
#include "cUIDraw.h"
#include "cUIImageView.h"
#include "cUITextView.h"
#include "cUIButton.h"

cUIController::cUIController()
    : m_isClickStart(false)
{
}


cUIController::~cUIController()
{
}

void cUIController::Setup()
{
    cUIDraw* pUIDraw;
    D3DXIMAGE_INFO stImageInfo;
    ZeroMemory(&stImageInfo, sizeof(D3DXIMAGE_INFO));
    cUIImageView* pImageView;
    cUITextView* pTextView;
    cUIButton* pUIButton;


    // == 캐릭터 정보창 UI 셋팅 ===================================================================================
    g_pTextureManager->AddTexture("CharacterImage", "./UI/Icon.png", true);

    pUIDraw = new cUIDraw;
    g_pAutoReleasePool->AddObject(pUIDraw);
    // 캐릭터 이미지
    pImageView = new cUIImageView;
    g_pAutoReleasePool->AddObject(pImageView);
    pImageView->SetTexture(g_pTextureManager->GetTexture("CharacterImage", &stImageInfo));
    pImageView->SetSize(ST_SIZE(stImageInfo.Width, stImageInfo.Height));
    pImageView->SetLocalPos(D3DXVECTOR3(10, 10, 0));
    //pImageView->SetDebugRender(true);
    pUIDraw->SetRoot(pImageView);

    // 레드바
    g_pTextureManager->AddTexture("RedBarImage", "./UI/RedBar.png", true);
    pImageView = new cUIImageView;
    g_pAutoReleasePool->AddObject(pImageView);
    pImageView->SetTexture(g_pTextureManager->GetTexture("RedBarImage", &stImageInfo));
    pImageView->SetSize(ST_SIZE(stImageInfo.Width, stImageInfo.Height));
    pImageView->SetLocalPos(D3DXVECTOR3(60, 5, 0));
    //pImageView->SetDebugRender(true);
    pUIDraw->AddChild(pImageView);

    // 블루바
    g_pTextureManager->AddTexture("BlueBarImage", "./UI/BlueBar.png", true);
    pImageView = new cUIImageView;
    g_pAutoReleasePool->AddObject(pImageView);
    pImageView->SetTexture(g_pTextureManager->GetTexture("BlueBarImage", &stImageInfo));
    pImageView->SetSize(ST_SIZE(stImageInfo.Width, stImageInfo.Height));
    pImageView->SetLocalPos(D3DXVECTOR3(60, 30, 0));
    //pImageView->SetDebugRender(true);
    pUIDraw->AddChild(pImageView);

    m_mapUIDraw.insert(make_pair(E_UI_KIND_CHARINFO, pUIDraw));

    // == 길찾기 정보창 UI 셋팅 =================================================================================
    g_pTextureManager->AddTexture("panel-info", "./UI/panel-info.png", true);

    pUIDraw = new cUIDraw;
    g_pAutoReleasePool->AddObject(pUIDraw);
    // 길찾기 정보창 판넬
    pImageView = new cUIImageView;
    g_pAutoReleasePool->AddObject(pImageView);
    pImageView->SetTexture(g_pTextureManager->GetTexture("panel-info", &stImageInfo));
    pImageView->SetSize(ST_SIZE(stImageInfo.Width, stImageInfo.Height));
    pImageView->SetLocalPos(D3DXVECTOR3(20, 80, 0));
    pImageView->SetDebugRender(true);
    pImageView->SetMove(true);
    pImageView->SetActive(false);
    pUIDraw->SetRoot(pImageView);

    // - 폰트
    LPD3DXFONT pFont;
    D3DXCreateFont(g_pD3DDevice, 20, 0, FW_BOLD, 1, false,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY,
        DEFAULT_PITCH | FF_DONTCARE, "도담9", &pFont);

    // 출발 좌표
    pTextView = new cUITextView;
    g_pAutoReleasePool->AddObject(pTextView);
    pTextView->SetText("출발 지점");
    pTextView->SetSize(ST_SIZE(400, 20));
    pTextView->SetLocalPos(D3DXVECTOR3(30, 50, 0));
    pTextView->SetFont(pFont);
    //SAFE_RELEASE(pFont);
    //pTextView->SetDebugRender(true);
    pTextView->SetDrawTextFormat(DT_LEFT | DT_VCENTER | DT_WORDBREAK | DT_NOCLIP);
    pTextView->SetColor(D3DCOLOR_XRGB(255, 255, 255));
    pTextView->SetTag(E_UI_TAG_TEXT_LOAD_START);
    pUIDraw->AddChild(pTextView);

    // 도착 좌표
    pTextView = new cUITextView;
    g_pAutoReleasePool->AddObject(pTextView);
    pTextView->SetText("도착 지점");
    pTextView->SetSize(ST_SIZE(400, 20));
    pTextView->SetLocalPos(D3DXVECTOR3(30, 120, 0));
    pTextView->SetFont(pFont);
    //SAFE_RELEASE(pFont);
    //pTextView->SetDebugRender(true);
    pTextView->SetDrawTextFormat(DT_LEFT | DT_VCENTER | DT_WORDBREAK | DT_NOCLIP);
    pTextView->SetColor(D3DCOLOR_XRGB(255, 255, 255));
    pTextView->SetTag(E_UI_TAG_TEXT_LOAD_DEST);
    pUIDraw->AddChild(pTextView);

    // 에이스타 여부
    pTextView = new cUITextView;
    g_pAutoReleasePool->AddObject(pTextView);
    pTextView->SetText("에이스타");
    pTextView->SetSize(ST_SIZE(300, 20));
    pTextView->SetLocalPos(D3DXVECTOR3(30, 190, 0));
    pTextView->SetFont(pFont);
    //SAFE_RELEASE(pFont);
    //pTextView->SetDebugRender(true);
    pTextView->SetDrawTextFormat(DT_LEFT | DT_VCENTER | DT_WORDBREAK | DT_NOCLIP);
    pTextView->SetColor(D3DCOLOR_XRGB(255, 255, 255));
    pTextView->SetTag(E_UI_TAG_ASTAR_ON_OFF);
    pUIDraw->AddChild(pTextView);

    // 출발 버튼
    g_pTextureManager->AddTexture("Green_Button", "./UI/Green_Button.png", true);

    pUIButton = new cUIButton;
    g_pAutoReleasePool->AddObject(pUIButton);
    pUIButton->SetTexture("./UI/btn-med-up.png", "./UI/btn-med-over.png", "./UI/btn-med-down.png");
    pUIButton->SetLocalPos(D3DXVECTOR3(135, 330, 0));
    pUIButton->SetUIButton(this);
    pUIButton->SetTag(E_UI_TAG_BTN_LOAD_START);
    //pImageView->SetDebugRender(true);
    pUIDraw->AddChild(pUIButton);

    // 출발 버튼 텍스트
    pTextView = new cUITextView;
    g_pAutoReleasePool->AddObject(pTextView);
    pTextView->SetText("출  발");
    pTextView->SetSize(ST_SIZE(250, 60));
    pTextView->SetLocalPos(D3DXVECTOR3(135, 330, 0));
    pTextView->SetFont(pFont);
    //SAFE_RELEASE(pFont);
    pTextView->SetDebugRender(true);
    pTextView->SetDrawTextFormat(DT_CENTER | DT_VCENTER | DT_WORDBREAK | DT_NOCLIP);
    pTextView->SetColor(D3DCOLOR_XRGB(255, 255, 255));
    pUIDraw->AddChild(pTextView);

    m_mapUIDraw.insert(make_pair(E_UI_KIND_LOADINFO, pUIDraw));

    // == 캐릭터 길찾기 남은 카운트 셋팅 ============================================================================

    pUIDraw = new cUIDraw;
    g_pAutoReleasePool->AddObject(pUIDraw);

    // 캐릭터남은 카운트 텍스트
    pTextView = new cUITextView;
    g_pAutoReleasePool->AddObject(pTextView);
    pTextView->SetText("0");
    pTextView->SetSize(ST_SIZE(50, 20));
    //pTextView->SetLocalPos(D3DXVECTOR3(0, 0, 0));
    pTextView->SetFont(pFont);
    //SAFE_RELEASE(pFont);
    pTextView->SetDebugRender(true);
    pTextView->SetDrawTextFormat(DT_CENTER | DT_VCENTER | DT_WORDBREAK | DT_NOCLIP);
    pTextView->SetColor(D3DCOLOR_XRGB(255, 255, 255));
    pTextView->SetTag(E_UI_TAG_TEXT_LOAD_CNT);
    pUIDraw->SetRoot(pTextView);

    m_mapUIDraw.insert(make_pair(E_UI_KIND_LOADCNT, pUIDraw));

    //===================================================================================================================
    SAFE_RELEASE(pFont); // 지역 변수이기에 릴리즈 여기서 시켜주는 것임

    // == 셋업 =====
    for each (auto iter in m_mapUIDraw)
        iter.second->Setup();
}

void cUIController::Update()
{
    for each (auto iter in m_mapUIDraw)
        iter.second->Update();
}

void cUIController::Render()
{
    for each (auto iter in m_mapUIDraw)
        iter.second->Render();
}

void cUIController::OnClick(cUIButton* pSender)
{
    // 로드 스타트 버튼을 눌렀으면
    if (pSender->GetTag() == E_UI_TAG_BTN_LOAD_START)
    {
        cUIDraw* pUIDraw = m_mapUIDraw[E_UI_KIND_LOADINFO];

        if (pUIDraw)
        {
            m_isClickStart = true;
            pUIDraw->GetRoot()->SetActive(false);
        }
    }
}

// UI 정보 셋팅
void cUIController::SetInfo(E_UI_KIND eKey, tagUIInfo stUIInfo)
{
    switch (eKey)
    {
    case E_UI_KIND_CHARINFO:
        break;
    case E_UI_KIND_CHARSTAT:
        break;
    case E_UI_KIND_LOADINFO:
        SetUIFindLoad(stUIInfo);
        break;
    case E_UI_KIND_LOADCNT:
        SetUILoadCnt(stUIInfo);
        break;
    }
}

// 길찾기 UI 정보 셋팅
void cUIController::SetUIFindLoad(tagUIInfo stUIInfo)
{
    cUIDraw* pUIDraw = m_mapUIDraw[E_UI_KIND_LOADINFO];

    if (pUIDraw)
    {
        pUIDraw->GetRoot()->SetActive(true);
        m_isClickStart = false;

        // 출발점
        cUITextView* pUIText = (cUITextView*)pUIDraw->GetRoot()->GetChildByTag(E_UI_TAG_TEXT_LOAD_START);
        char s[80];
        sprintf_s(s, "%0.3f, ", stUIInfo.vStartPos.x);
        string str = "출발점 : " + (string)s;
        sprintf_s(s, "%0.3f, ", stUIInfo.vStartPos.y);
        str += s;
        sprintf_s(s, "%0.3f", stUIInfo.vStartPos.z);
        str += s;
        pUIText->SetText(str);

        // 도착점
        pUIText = (cUITextView*)pUIDraw->GetRoot()->GetChildByTag(E_UI_TAG_TEXT_LOAD_DEST);
        sprintf_s(s, "%0.3f, ", stUIInfo.vDestPos.x);
        str = "도착점 : " + (string)s;
        sprintf_s(s, "%0.3f, ", stUIInfo.vDestPos.y);
        str += s;
        sprintf_s(s, "%0.3f", stUIInfo.vDestPos.z);
        str += s;
        pUIText->SetText(str);

        // 에이스타 여부
        pUIText = (cUITextView*)pUIDraw->GetRoot()->GetChildByTag(E_UI_TAG_ASTAR_ON_OFF);
        str;
        if (stUIInfo.isAStar)
            str = "에이스타 : ON ";
        else
            str = "에이스타 : OFF ";
        pUIText->SetText(str);
    }
}

void cUIController::SetUILoadCnt(tagUIInfo stUIInfo)
{
    cUIDraw* pUIDraw = m_mapUIDraw[E_UI_KIND_LOADCNT];

    if (pUIDraw)
    {
        cUITextView* pUIText = (cUITextView*)pUIDraw->GetRoot()->GetChildByTag(E_UI_TAG_TEXT_LOAD_CNT);
        stUIInfo.vCurrPos.y += 8.0f;

        D3DXVECTOR3 screenPos = stUIInfo.vCurrPos;
        D3DXMATRIXA16 matView, matProj, matVP;

        g_pD3DDevice->GetTransform(D3DTS_VIEW, &matView);
        g_pD3DDevice->GetTransform(D3DTS_PROJECTION, &matProj);

        D3DVIEWPORT9 vp;
        g_pD3DDevice->GetViewport(&vp);
        D3DXMatrixIdentity(&matVP);
        matVP._11 = vp.Width / 2.0f;
        matVP._22 = -(vp.Height / 2.0f);
        matVP._33 = vp.MaxZ - vp.MinZ;
        matVP._41 = vp.X + vp.Width / 2.0f;
        matVP._42 = vp.Y + vp.Height / 2.0f;
        matVP._43 = vp.MinZ;

        D3DXVec3TransformCoord(&screenPos, &screenPos, &(matView * matProj * matVP));

        pUIText->SetLocalPos(D3DXVECTOR3(screenPos.x - 25.0f, screenPos.y, 0.0));

        char s[80];
        sprintf_s(s, "%d", stUIInfo.nCount);
        pUIText->SetText((string)s);
    }
}
