#pragma once
#include "stdafx.h"

/*
�������̽� : ���� ���� �Լ��� ����
 - �������̽��� ��� ���� Ŭ�����鿡 ���� �ؾߵ� �Լ����� ���� ���� �Լ��� ������ ���´�.
*/

interface iMap
{
	virtual void Load(IN char* szFilePath, IN D3DXMATRIXA16* pMat) PURE;
	virtual bool GetHeight(IN const float& x, OUT float& y, IN const float& z) PURE;
	//virtual void Test() PURE;
};