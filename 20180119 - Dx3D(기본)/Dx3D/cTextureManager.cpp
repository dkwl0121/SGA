#include "stdafx.h"
#include "cTextureManager.h"


cTextureManager::cTextureManager()
{
}


cTextureManager::~cTextureManager()
{
}

void cTextureManager::Destroy()
{
	map<string, LPDIRECT3DTEXTURE9>::iterator iter;

	for (iter = m_mapTexture.begin(); iter != m_mapTexture.end(); ++iter)
		SAFE_RELEASE(iter->second);

	m_mapTexture.clear();
}

void cTextureManager::AddTexture(string key, string fileName)
{
	if (m_mapTexture[key] == NULL)
	{
		//wstring s;
		//s = wstring(fileName.begin(), fileName.end());
		D3DXCreateTextureFromFile(g_pD3DDevice, fileName.c_str(), &m_mapTexture[key]);
	}
}

LPDIRECT3DTEXTURE9& cTextureManager::GetTexture(string key)
{
	map<string, LPDIRECT3DTEXTURE9>::iterator iter;
	LPDIRECT3DTEXTURE9 ret = NULL;

	iter = m_mapTexture.find(key);

	if (iter != m_mapTexture.end())
		ret = iter->second;

	return ret;
}
