#pragma once

#define g_pFontManager cFontManager::GetInstance()

class cFontManager
{
	SINGLETON(cFontManager);

public:
	enum eFontType
	{
		E_NORMAL,       // �⺻
		E_CHAT,         // ä��
		E_QUEST         // ����Ʈ
	};

private:
	map<eFontType, LPD3DXFONT>	m_mapFont;

public:
	LPD3DXFONT GetFont(eFontType e);

    // ��ο� ���ִ� �Լ� �����ϱ�

	void Destory();
};

