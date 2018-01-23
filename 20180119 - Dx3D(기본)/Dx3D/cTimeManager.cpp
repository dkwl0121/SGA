#include "stdafx.h"
#include "cTimeManager.h"


cTimeManager::cTimeManager()
{
	m_dwLastUpdatedTime = GetTickCount();
}


cTimeManager::~cTimeManager()
{
}

void cTimeManager::Update()
{
	DWORD dwCurrUpdateTime = GetTickCount();
	m_dwDeltaTime = dwCurrUpdateTime - m_dwLastUpdatedTime;
	m_dwLastUpdatedTime = dwCurrUpdateTime;
}

float cTimeManager::GetDeltaTime()
{
	return m_dwDeltaTime / 1000.0f;
}
