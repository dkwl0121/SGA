#pragma once
#include "cObject.h"

class cGroup;
class cMtlTex;

class cObjLoader : public cObject
{
private:
	map<string, cMtlTex*> m_mapMtlTex;

	void LoadMtlLib(char* szFilePath);

public:
	cObjLoader();
	~cObjLoader();

    LPD3DXMESH LoadMesh(IN char* szFilePath, IN D3DXMATRIXA16* pMat,
        OUT vector<cMtlTex*>& vecMtlTex);
};

