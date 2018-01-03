#pragma once
class cTownHuman
{
private:
	map<int, string>			m_mTown;			// ���� ���� ��
	map<int, vector<int>>		m_mHuman;			// ��� ���� ��
	vector<int>					m_vHumanNum;		// ��� ��ȣ ����(������� �� ��� ����� �� �� ����)

	vector<int> HumanToken(string human);			// ������� �ɰ���
	void PushHuman(int num);						// ��� �ִ��� üũ�ϰ� ��� �ֱ�
public:
	cTownHuman();
	~cTownHuman();

	void SetTown(FILE* fp);
	void CountHuman();

	int GetHumanCnt() { return m_mHuman.size(); }
};

