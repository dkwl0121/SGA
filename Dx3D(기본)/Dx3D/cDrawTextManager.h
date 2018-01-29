#pragma once

#define g_pDrawTextManager cDrawTextManager::GetInstance()

class cDrawTextManager
{
    SINGLETON(cDrawTextManager);

private:
    map<string, ID3DXFont*>         m_mapFont;
    ID3DXSprite*                    m_pSprite;
    string                          m_sDefault;

public:

    void Setup();

    void Destroy();

    void DrawTextOut(string sText, RECT rt, D3DXCOLOR color);
    void DrawTextOut(string sText, RECT rt, D3DXCOLOR color, string sFontName);
};

