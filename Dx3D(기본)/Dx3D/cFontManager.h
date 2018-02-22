#pragma once

#define g_pFontManager cFontManager::GetInstance()

class cFontManager
{
	SINGLETON(cFontManager);

public:
	enum eFontType
	{
		E_NORMAL,       // 기본
		E_CHAT,         // 채팅
		E_QUEST         // 퀘스트
	};

private:
	map<eFontType, LPD3DXFONT>	m_mapFont;

public:
	LPD3DXFONT GetFont(eFontType e);

    // 드로우 해주는 함수 생성하기

	void Destory();
};

