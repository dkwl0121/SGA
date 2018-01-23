#pragma once

#define g_pLightManager cLightManager::GetInstance()

class cLightManager
{
private:
	map<string, int>			m_mapLightIndex;
	vector<D3DLIGHT9>			m_vecLight;

public:

	static cLightManager* GetInstance()
	{
		static cLightManager instance;
		return &instance;
	}

	// �߰�
	void AddLight(string str, D3DLIGHT9 light);
	// ����
	D3DLIGHT9 GetLight(string str);
	// ����
	void SetLight(string str, D3DLIGHT9 light);
	// Ȱ��ȭ
	void EnableLight(string str, bool b);
};

