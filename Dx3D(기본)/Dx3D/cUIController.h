#pragma once

#include "cObject.h"
#include "cUIButton.h"  // 인터페이스 때문에.

class cUIDraw;

enum E_UI_KIND {
    E_UI_KIND_CHARINFO,             // 캐릭터 정보
    E_UI_KIND_CHARSTAT,             // 캐릭터 상태
    E_UI_KIND_LOADINFO,             // 길찾기 정보
    E_UI_KIND_LOADCNT               // 남은 길 카운트
};

enum E_UI_TAG {
    E_UI_TAG_BASS = -1,
    E_UI_TAG_BTN_LOAD_START,
    E_UI_TAG_TEXT_LOAD_START,
    E_UI_TAG_TEXT_LOAD_DEST,
    E_UI_TAG_ASTAR_ON_OFF,
    E_UI_TAG_TEXT_LOAD_CNT
};

struct tagUIInfo {

    // 길찾기 정보
    D3DXVECTOR3     vStartPos;
    D3DXVECTOR3     vDestPos;
    bool            isAStar;

    // 길 카운트 정보
    D3DXVECTOR3     vCurrPos;
    int             nCount;

    tagUIInfo() {}
    tagUIInfo(D3DXVECTOR3 start, D3DXVECTOR3 dest, bool astar) : vStartPos(start), vDestPos(dest), isAStar(astar) {}
    tagUIInfo(D3DXVECTOR3 pos, int cnt) : vCurrPos(pos), nCount(cnt) {}
};

class cUIController : public cObject, public iUIButton
{
private:
    map<E_UI_KIND, cUIDraw*>            m_mapUIDraw;

    bool                                m_isClickStart;

public:
    cUIController();
    ~cUIController();

    void Setup();
    void Update();
    void Render();

    // iUIButton override
    virtual void OnClick(cUIButton* pSender) override;

    // UI에 따른 정보 셋팅
    void SetInfo(E_UI_KIND eKey, tagUIInfo stUIInfo);

    // 길찾기 UI 정보 셋팅
    void SetUIFindLoad(tagUIInfo stUIInfo);
    // 길찾기 노드 카운트 셋팅
    void SetUILoadCnt(tagUIInfo stUIInfo);

    // 겟터
    bool GetIsClickStart() { return m_isClickStart; }
};

