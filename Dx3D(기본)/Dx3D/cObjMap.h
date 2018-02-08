#pragma once
#include "cObject.h"
#include "iMap.h"

/*
�������̽��� ��� ���� Ŭ������ �������̽��� ���� �Ǿ� �ִ� ���� �����Լ�����
'������' ���� ���� �ؾ��Ѵ�.
*/

class cObjMap :	public cObject, public iMap
{
private:
	vector<D3DXVECTOR3> m_vecVertex;

public:
	cObjMap();
	~cObjMap();

	virtual void Load(IN char* szFilePath, IN D3DXMATRIXA16* pMat) override;
	virtual bool GetHeight(IN const float& x, OUT float& y, IN const float& z) override;
    virtual bool ColisionRay(IN D3DXVECTOR3* vOrigin, IN D3DXVECTOR3* vDir, OUT D3DXVECTOR3& vPos) override;
};

