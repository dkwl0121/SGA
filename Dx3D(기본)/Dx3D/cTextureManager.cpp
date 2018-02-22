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

void cTextureManager::AddTexture(string key, string fileName, bool saveImageInfo/*= false*/)
{
	if (m_mapTexture[key] == NULL)
	{
        if (saveImageInfo)
            D3DXCreateTextureFromFileEx(g_pD3DDevice, fileName.c_str(), D3DX_DEFAULT_NONPOW2,
                D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED,
                D3DX_FILTER_NONE, D3DX_DEFAULT, 0, &m_mapImageInfo[key], NULL, &m_mapTexture[key]);

		else
            D3DXCreateTextureFromFile(g_pD3DDevice, fileName.c_str(), &m_mapTexture[key]);
        //wstring s;
        //s = wstring(fileName.begin(), fileName.end());
	}
}

LPDIRECT3DTEXTURE9& cTextureManager::GetTexture(string key, OUT D3DXIMAGE_INFO* pImageInfo)
{
	map<string, LPDIRECT3DTEXTURE9>::iterator iter;
	LPDIRECT3DTEXTURE9* ret = NULL;

	iter = m_mapTexture.find(key);

    if (iter != m_mapTexture.end())
    {
        if (pImageInfo)
        {
            map<string, D3DXIMAGE_INFO>::iterator iter2;
            iter2 = m_mapImageInfo.find(key);

            if (iter2 != m_mapImageInfo.end())
                *pImageInfo = iter2->second;
        }

        ret = &iter->second;
    }

	return *ret;
}
