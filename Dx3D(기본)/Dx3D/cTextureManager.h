#pragma once

#define g_pTextureManager cTextureManager::GetInstance()

class cTextureManager
{
    SINGLETON(cTextureManager);

private:
	map<string, LPDIRECT3DTEXTURE9> m_mapTexture;
    map<string, D3DXIMAGE_INFO>		m_mapImageInfo;

public:

	void Destroy();

	void AddTexture(string key, string fileName, bool saveImageInfo = false);
	LPDIRECT3DTEXTURE9& GetTexture(string key, OUT D3DXIMAGE_INFO* pImageInfo = NULL);
};

