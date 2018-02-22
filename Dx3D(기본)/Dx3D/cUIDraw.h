#pragma once
#include "cObject.h"
#include "cUIObject.h"

class cUIDraw : public cObject
{
private:
	LPD3DXSPRITE		m_pSprite;
	D3DXIMAGE_INFO		m_stImageInfo;
    cUIObject*          m_pUIRoot;

public:
	cUIDraw();
	~cUIDraw();

	void Setup();
	void Update(D3DXVECTOR3* pos = NULL);
	void Render();

    // ╪бем
    void SetRoot(cUIObject* root) { m_pUIRoot = root; }
    void AddChild(cUIObject* child) { if (m_pUIRoot) m_pUIRoot->AddChild(child); }

    // ╟ыем
    cUIObject* GetRoot() { return m_pUIRoot; }
};

