#pragma once
#include "cObject.h"

class cMtlTex : public cObject
{
private:
    SYNTHESIZE_PASS_BY_PT(D3DMATERIAL9, m_stMtl, Mtl);
	SYNTHESIZE_ADD_REF(LPDIRECT3DTEXTURE9, m_pTexture, Texture);
    SYNTHESIZE(DWORD, m_nMtlRef, MtlRef);

public:
	cMtlTex();
	virtual ~cMtlTex();
};

