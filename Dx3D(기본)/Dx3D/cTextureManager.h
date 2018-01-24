#pragma once

#define g_pTextureManager cTextureManager::GetInstance()

class cTextureManager
{
    SINGLETON(cTextureManager);

private:
	map<string, LPDIRECT3DTEXTURE9> m_mapTexture;

public:

	void Destroy();

	void AddTexture(string key, string fileName);
	LPDIRECT3DTEXTURE9& GetTexture(string key);
};

