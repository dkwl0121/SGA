#pragma once
#include "cObject.h"

class cMtlTex : public cObject
{
private:
	SYNTHESIZE_PASS_BY_REF(D3DMATERIAL9, m_stMtl, Mtl);
	SYNTHESIZE_ADD_REF(LPDIRECT3DTEXTURE9, m_pTexture, Texture);

	//LPDIRECT3DTEXTURE9 m_pTexture;

public:
	cMtlTex();
	virtual ~cMtlTex();

	//LPDIRECT3DTEXTURE9& GetTexture() { return m_pTexture; }

	//void SetTexture(LPDIRECT3DTEXTURE9& t)
	//{
	//	t->AddRef();
	//	m_pTexture = t;
	//}
};

