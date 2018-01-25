#pragma once
#include "cObject.h"
#include "cMtlTex.h"

class cGroup : public cObject
{
private:
    SYNTHESIZE_PASS_BY_REF(IDirect3DVertexBuffer9*, m_vb, VertexBuffer);
    //SYNTHESIZE_PASS_BY_REF(IDirect3DIndexBuffer9*, m_ib, IndexBuffer);
	SYNTHESIZE_ADD_REF(cMtlTex*, m_pMtlTex, MtlTex);

public:
	cGroup();
	~cGroup();

	void Render();
};

