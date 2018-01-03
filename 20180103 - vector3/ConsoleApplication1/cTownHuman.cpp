#include "stdafx.h"
#include "cTownHuman.h"


cTownHuman::cTownHuman()
{
	m_mTown.clear();
	m_mHuman.clear();
	m_vHumanNum.clear();
}


cTownHuman::~cTownHuman()
{
}

void cTownHuman::SetTown(FILE * fp)
{
	int cnt = 0;
	while (true)
	{
		if (feof(fp)) break; // ������ ������ �� �о����� ����!

		char szBuf[1024];
		fgets(szBuf, 1024, fp);		// ���پ� �д´�.
		
		m_mTown[cnt] = szBuf; // Ÿ�� ������ ���پ� �ִ´�.
		++cnt;
	}
}

// ��� �� ����
void cTownHuman::CountHuman()
{
	// ��� �ֱ�
	PushHuman(1); // 1�� ������ ���

	// ������� Ȯ�ε� ����� ����Ű�� ����鵵 ���Ϳ� �־��� (�ߺ�üũ�� ��!)
	for (int i = 0; i < m_vHumanNum.size(); ++i)
	{
		// ����� ����Ű�� ����� �־��� 
		vector<int> human = m_mHuman[m_vHumanNum[i]];

		// �� ������� �ߺ�üũ�ϰ� ������Ϳ� �־��ش�.
		for (auto iter = human.begin(); iter != human.end(); ++iter)
		{
			PushHuman(*iter); // ����� �ִ��� üũ�ϰ� �ֱ�
		}
	}
}

// ������� �ɰ���
vector<int> cTownHuman::HumanToken(string human)
{
	vector<int> vecHuman;
	string st = "";
	for (int i = 0; i < human.length(); ++i)
	{
		if (human[i] == ' ') // �����̸� st�� ���ִ� ��Ʈ�� ���� ���Ϳ� �־���
		{
			vecHuman.push_back(atoi(st.c_str()));
			st = ""; // ��Ʈ�� ����
		}
		else if (human[i] == '\n') // ���� ����
		{
			if (human[i - 1] != ' ') // ����1�̸�
				vecHuman.push_back(atoi(st.c_str()));
			break;
		}
		else // ������ ���
		{
			st += human[i];
		}
	}

	// �ڽ��� ���ڿ� �ڽ��� ����Ű�� �ִ� ����� ������ ����
	if (vecHuman.size() > 2)
	{
		vecHuman.erase(vecHuman.begin());
		vecHuman.erase(vecHuman.begin());
	}

	return vecHuman;
}

// ��� �ִ��� üũ�ϰ� ��� �ֱ�
void cTownHuman::PushHuman(int num)
{
	// ���� ������ ����� ������
	if (m_mHuman.find(num) == m_mHuman.end()) // �ѹ����� ���Ҵµ��� �� Ű���� ���� ���̸� ��ã�Ҿ�!!!
	{ 
		m_mHuman[num] = HumanToken(m_mTown[num]); // ����ֱ�(�����ȣ, ����̰���Ű�� �����)
		m_vHumanNum.push_back(num);
	}
}

