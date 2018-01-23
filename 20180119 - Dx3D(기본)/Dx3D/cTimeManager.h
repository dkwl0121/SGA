#pragma once

#define g_pTimeManager cTimeManager::GetInstance()

class cTimeManager
{
private:
	DWORD m_dwLastUpdatedTime;
	DWORD m_dwDeltaTime;

public:
	cTimeManager();
	~cTimeManager();

	static cTimeManager* GetInstance()
	{
		static cTimeManager instance;
		return &instance;
	}

	void Update();

	float GetDeltaTime();
};

