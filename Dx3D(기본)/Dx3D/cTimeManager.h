#pragma once

#define g_pTimeManager cTimeManager::GetInstance()

class cTimeManager
{
    SINGLETON(cTimeManager);

private:
	DWORD m_dwLastUpdatedTime;
	DWORD m_dwDeltaTime;

public:

	void Update();

	float GetDeltaTime();
};

