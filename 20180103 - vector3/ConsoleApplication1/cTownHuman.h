#pragma once
class cTownHuman
{
private:
	map<int, string>			m_mTown;			// 마을 정보 맵
	map<int, vector<int>>		m_mHuman;			// 사람 정보 맵
	vector<int>					m_vHumanNum;		// 사람 번호 벡터(사이즈로 총 사람 명수를 알 수 있음)

	vector<int> HumanToken(string human);			// 사람정보 쪼개기
	void PushHuman(int num);						// 사람 있는지 체크하고 사람 넣기
public:
	cTownHuman();
	~cTownHuman();

	void SetTown(FILE* fp);
	void CountHuman();

	int GetHumanCnt() { return m_mHuman.size(); }
};

