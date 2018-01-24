#pragma once

#define g_pLightManager cLightManager::GetInstance()

class cLightManager
{
    SINGLETON(cLightManager);

private:
	map<string, int>			m_mapLightIndex;
	vector<D3DLIGHT9>			m_vecLight;

public:

    void Destroy();

	// 추가
	void AddLight(string str, D3DLIGHT9 light);
	// 겟터
	D3DLIGHT9 GetLight(string str);
	// 셋터
	void SetLight(string str, D3DLIGHT9 light);
	// 활성화
	void EnableLight(string str, bool b);
};

