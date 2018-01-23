#include "stdafx.h"
#include "cKeyManager.h"

void cKeyManager::Setup()
{
	for (int i = 0; i < KEYMAX; ++i)
	{
		m_bitKeyUp.set(i, false);
		m_bitKeyDown.set(i, false);
	}
}

bool cKeyManager::isOnceKeyDown(int key)
{
	if (GetAsyncKeyState(key) & 0x8000)
	{
		// 키가 이전에 눌리지 않은 상태
		if (!m_bitKeyDown[key])
		{
			// 눌린 상태로 변경 하고 클릭 됐다고 트루 반환
			m_bitKeyDown.set(key, true);
			return true;
		}
	}
	else
	{
		m_bitKeyDown.set(key, false);
	}

	return false;
}

bool cKeyManager::isOnceKeyUp(int key)
{
	if (GetAsyncKeyState(key) & 0x8000)
	{
		m_bitKeyUp.set(key, true);
	}
	else
	{
		// 누르고 있다가 땠을 때
		if (m_bitKeyUp[key])
		{
			m_bitKeyUp.set(key, false);
			return true;
		}
	}

	return false;
}

bool cKeyManager::isStayKeyDown(int key)
{
	if (GetAsyncKeyState(key) & 0x8000) return true;
	return false;
}

bool cKeyManager::isToggleKey(int key)
{
	if (GetKeyState(key) & 0x0001) return true;
	return false;
}