#pragma once

#include "cObject.h"
#include "cUIButton.h"  // �������̽� ������.

class cUIDraw;

enum E_UI_KIND {
    E_UI_KIND_CHARINFO,             // ĳ���� ����
    E_UI_KIND_CHARSTAT,             // ĳ���� ����
    E_UI_KIND_LOADINFO,             // ��ã�� ����
    E_UI_KIND_LOADCNT               // ���� �� ī��Ʈ
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

    // ��ã�� ����
    D3DXVECTOR3     vStartPos;
    D3DXVECTOR3     vDestPos;
    bool            isAStar;

    // �� ī��Ʈ ����
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

    // UI�� ���� ���� ����
    void SetInfo(E_UI_KIND eKey, tagUIInfo stUIInfo);

    // ��ã�� UI ���� ����
    void SetUIFindLoad(tagUIInfo stUIInfo);
    // ��ã�� ��� ī��Ʈ ����
    void SetUILoadCnt(tagUIInfo stUIInfo);

    // ����
    bool GetIsClickStart() { return m_isClickStart; }
};

