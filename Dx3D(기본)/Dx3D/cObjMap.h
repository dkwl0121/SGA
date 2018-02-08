#pragma once
#include "cObject.h"
#include "iMap.h"

/*
인터페이스를 상속 받은 클래스는 인터페이스에 구현 되어 있는 순수 가상함수들을
'무조건' 직접 구현 해야한다.
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

