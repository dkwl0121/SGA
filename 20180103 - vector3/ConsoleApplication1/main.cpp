// ConsoleApplication1.cpp: 콘솔 응용 프로그램의 진입점을 정의합니다.
//

#include "stdafx.h"
#include "cVector3.h"
#include "cTownHuman.h"

void main()
{
	//ㅁㄴㅇㅁㄴㅇㄻㄴ

	// 벡터!!!!!!!!!!!!!!!
	//cVector3 pV1(1, 0, 0);
	//cVector3 pV2(0, 1, 0);
	//cVector3 pV3(0, 1, 0);

	////pV3 = pV1 + pV2;

	//if (pV1 == pV2)
	//	cout << "pV1와 pV2는 같다" << endl;
	//else
	//	cout << "pV1와 pV2는 같지 않다" << endl;

	//if (pV3 == pV2)
	//	cout << "pV3와 pV2는 같다" << endl;
	//else
	//	cout << "pV3와 pV2는 같지 않다" << endl;

	// 마을 사람 수 구하기
	cTownHuman* pTownHuman1 = new cTownHuman;

	// 첫번째 파읽 읽는 방법!!!!
	FILE* fp1 = NULL;
	fopen_s(&fp1, "마을1.txt", "r");

	pTownHuman1->SetTown(fp1);

	DWORD dwStart = GetTickCount();
	pTownHuman1->CountHuman();
	DWORD dwEnd = GetTickCount();

	cout << dwEnd - dwStart << endl;

	cout << "마을1 사람 수 : " << pTownHuman1->GetHumanCnt() << "" << endl;

	SAFE_DELETE(pTownHuman1);

	cTownHuman* pTownHuman2 = new cTownHuman;
	// 첫번째 파읽 읽는 방법!!!!
	FILE* fp2 = NULL;
	fopen_s(&fp2, "마을2.txt", "r");

	pTownHuman2->SetTown(fp2);

	pTownHuman1->SetTown(fp1);

	dwStart = GetTickCount();
	pTownHuman2->CountHuman();
	dwEnd = GetTickCount();

	cout << dwEnd - dwStart << endl;
	
	cout << "마을2 사람 수 : " << pTownHuman2->GetHumanCnt() << "" << endl;

	SAFE_DELETE(pTownHuman2);

	system("pause");
}