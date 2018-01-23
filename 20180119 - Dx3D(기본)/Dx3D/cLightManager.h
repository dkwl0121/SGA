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

	// 추가
	void AddLight(string str, D3DLIGHT9 light);
	// 겟터
	D3DLIGHT9 GetLight(string str);
	// 셋터
	void SetLight(string str, D3DLIGHT9 light);
	// 활성화
	void EnableLight(string str, bool b);
};

