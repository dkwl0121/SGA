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
		if (feof(fp)) break; // 파일을 끝까지 다 읽었으면 종료!

		char szBuf[1024];
		fgets(szBuf, 1024, fp);		// 한줄씩 읽는다.
		
		m_mTown[cnt] = szBuf; // 타운 정보에 한줄씩 넣는다.
		++cnt;
	}
}

// 사람 수 세기
void cTownHuman::CountHuman()
{
	// 사람 넣기
	PushHuman(1); // 1은 무조건 사람

	// 사람으로 확인된 사람이 가리키는 사람들도 벡터에 넣어줌 (중복체크도 함!)
	for (int i = 0; i < m_vHumanNum.size(); ++i)
	{
		// 사람이 가리키는 사람들 넣어줌 
		vector<int> human = m_mHuman[m_vHumanNum[i]];

		// 그 사람들을 중복체크하고 사람벡터에 넣어준다.
		for (auto iter = human.begin(); iter != human.end(); ++iter)
		{
			PushHuman(*iter); // 사람이 있는지 체크하고 넣기
		}
	}
}

// 사람정보 쪼개기
vector<int> cTownHuman::HumanToken(string human)
{
	vector<int> vecHuman;
	string st = "";
	for (int i = 0; i < human.length(); ++i)
	{
		if (human[i] == ' ') // 공백이면 st에 들어가있는 스트링 값을 벡터에 넣어줌
		{
			vecHuman.push_back(atoi(st.c_str()));
			st = ""; // 스트링 비우기
		}
		else if (human[i] == '\n') // 줄이 끝남
		{
			if (human[i - 1] != ' ') // 마을1이면
				vecHuman.push_back(atoi(st.c_str()));
			break;
		}
		else // 숫자인 경우
		{
			st += human[i];
		}
	}

	// 자신의 숫자와 자신이 가리키고 있는 사람의 개수는 빼줌
	if (vecHuman.size() > 2)
	{
		vecHuman.erase(vecHuman.begin());
		vecHuman.erase(vecHuman.begin());
	}

	return vecHuman;
}

// 사람 있는지 체크하고 사람 넣기
void cTownHuman::PushHuman(int num)
{
	// 같은 숫자의 사람이 없으면
	if (m_mHuman.find(num) == m_mHuman.end()) // 한바퀴를 돌았는데도 그 키값을 가진 아이를 못찾았어!!!
	{ 
		m_mHuman[num] = HumanToken(m_mTown[num]); // 사람넣기(사람번호, 사람이가리키는 사람들)
		m_vHumanNum.push_back(num);
	}
}

