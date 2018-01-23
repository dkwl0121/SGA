#pragma once

#define g_pTextureManager cTextureManager::GetInstance()

class cTextureManager
{
private:
	map<string, LPDIRECT3DTEXTURE9> m_mapTexture;

public:
	cTextureManager();
	~cTextureManager();

	static cTextureManager* GetInstance()
	{
		static cTextureManager instance;
		return &instance;
	}

	void Destroy();

	void AddTexture(string key, string fileName);
	LPDIRECT3DTEXTURE9& GetTexture(string key);
};

