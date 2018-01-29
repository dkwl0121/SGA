#include "stdafx.h"
#include "cDrawTextManager.h"


cDrawTextManager::cDrawTextManager()
    : m_pSprite(NULL)
{
}

cDrawTextManager::~cDrawTextManager()
{
}

void cDrawTextManager::Setup()
{
    ID3DXFont* font;
    m_sDefault = "����";
    D3DXCreateFont(g_pD3DDevice, 50, 0, FW_BOLD, 1, false,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY,
        DEFAULT_PITCH | FF_DONTCARE, (LPCSTR)m_sDefault.c_str(), &font);

    m_mapFont[m_sDefault] = font;

    D3DXCreateSprite(g_pD3DDevice, &m_pSprite);
}

void cDrawTextManager::Destroy()
{
    for each(auto iter in m_mapFont)
    {
        SAFE_RELEASE(iter.second);
    }
    SAFE_RELEASE(m_pSprite);
}

void cDrawTextManager::DrawTextOut(string sText, RECT rt, D3DXCOLOR color)
{
    if (m_pSprite)
    {
        // 3D����� �ϰ� ���������ڸ� D3DXSPRITE_OBJECTSPACE�� �ٲ��� �� D3DXMatrixTranslation���� ��ġ ���� �� ����ϸ� ��
        m_pSprite->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE); 
        m_mapFont[m_sDefault]->DrawTextA(m_pSprite, (LPCSTR)sText.c_str(), -1, &rt, DT_NOCLIP, color);
        m_pSprite->End();
    }
}

void cDrawTextManager::DrawTextOut(string sText, RECT rt, D3DXCOLOR color, string sFontName)
{
    if (m_pSprite)
    {
        // ���� ��Ʈ�� ��Ʈ ����
        if (m_mapFont[sFontName] == NULL)
        {
            D3DXCreateFont(g_pD3DDevice, 50, 0, FW_BOLD, 1, false,
                DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY,
                DEFAULT_PITCH | FF_DONTCARE, (LPCSTR)sFontName.c_str(), &m_mapFont[sFontName]);
        }
        
        m_pSprite->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE);
        m_mapFont[sFontName]->DrawTextA(m_pSprite, (LPCSTR)sText.c_str(), -1, &rt, DT_NOCLIP, color);
        m_pSprite->End();
    }
}