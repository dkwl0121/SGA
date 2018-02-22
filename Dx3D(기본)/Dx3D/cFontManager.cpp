#include "stdafx.h"
#include "cFontManager.h"


cFontManager::cFontManager()
{
}

cFontManager::~cFontManager()
{
}

LPD3DXFONT cFontManager::GetFont(eFontType e)
{
	// 찾는 폰트가 없는 경우 생성
	if (m_mapFont.find(e) == m_mapFont.end())
	{
		switch (e)
		{
		case cFontManager::E_NORMAL:
            D3DXCreateFont(g_pD3DDevice, 20, 0, FW_DONTCARE, 1, false, DEFAULT_CHARSET,
                OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, false, "도담9", &m_mapFont[e]);
			break;
		case cFontManager::E_CHAT:
			break;
		case cFontManager::E_QUEST:
			AddFontResource("UI/umberto.ttf");

			D3DXCreateFont(g_pD3DDevice, 20, 0, FW_DONTCARE, 1, false, DEFAULT_CHARSET,
				OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, false, "umberto", &m_mapFont[e]);

			//D3DXFONT_DESC fd;
			//fd.Height = 48;
			//fd.Width = 24;
			//fd.Weight = FW_DONTCARE;
			//fd.MipLevels = 1;
			//fd.Italic = false;
			//fd.CharSet = DEFAULT_CHARSET;
			//fd.OutputPrecision = OUT_DEFAULT_PRECIS;
			//fd.Quality = DEFAULT_QUALITY;
			//fd.PitchAndFamily = false;
			//strcpy_s(fd.FaceName, "궁서체");

			//D3DXCreateFontIndirect(g_pD3DDevice, &fd, &m_mapFont[e]);

			break;
		}
	}
	
	return m_mapFont[e];
}

void cFontManager::Destory()
{
	RemoveFontResource("umberto");

	for each (auto p in m_mapFont)
		SAFE_RELEASE(p.second);

	m_mapFont.clear();
}
