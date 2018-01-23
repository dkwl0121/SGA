#pragma once
#include <bitset>

using namespace std;

#define KEYMAX 256

#define g_pKeyManager cKeyManager::GetInstance()

class cKeyManager
{
private:
	bitset<KEYMAX> m_bitKeyUp;
	bitset<KEYMAX> m_bitKeyDown;

public:

	static cKeyManager* GetInstance()
	{
		static cKeyManager instance;
		return &instance;
	}

	// Ű �Ŵ��� �ʱ�ȭ
	void Setup();

	// Ű�� �ѹ� ����
	bool isOnceKeyDown(int key);
	// Ű�� ���ȴ� ��
	bool isOnceKeyUp(int key);
	// Ű�� ������ ����
	bool isStayKeyDown(int key);
	// ���Ű(Ű�� ���� Ű)
	bool isToggleKey(int key);
};