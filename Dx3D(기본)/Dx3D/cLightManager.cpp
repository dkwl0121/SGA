#include "stdafx.h"
#include "cLightManager.h"

cLightManager::cLightManager()
{
}

cLightManager::~cLightManager()
{
}

void cLightManager::Destroy()
{
    //vector<D3DLIGHT9>::iterator iter;

    //for (iter = m_vecLight.begin(); iter != m_vecLight.end(); ++iter)
    //    SAFE_RELEASE(*iter);

    m_vecLight.clear();
}

void cLightManager::AddLight(string str, D3DLIGHT9 light)
{
	if (m_mapLightIndex[str] == NULL) // 스트링있는지 확인 (없으면 할당이 됨)
	{
		m_vecLight.push_back(light);

		int index = m_vecLight.size() - 1;

		g_pD3DDevice->SetLight(index, &light);
		g_pD3DDevice->LightEnable(index, true);

		m_mapLightIndex[str] = index;
	}
}

D3DLIGHT9 cLightManager::GetLight(string str)
{
	if (m_mapLightIndex.find(str) != m_mapLightIndex.end())
	{
		return m_vecLight[m_mapLightIndex[str]];
	}

	return D3DLIGHT9();
}

void cLightManager::SetLight(string str, D3DLIGHT9 light)
{
	if (m_mapLightIndex.find(str) != m_mapLightIndex.end())
	{
		m_vecLight[m_mapLightIndex[str]] = light;
		g_pD3DDevice->SetLight(m_mapLightIndex[str], &light);
	}
}

void cLightManager::EnableLight(string str, bool b)
{
	if (m_mapLightIndex.find(str) != m_mapLightIndex.end())
	{
		g_pD3DDevice->LightEnable(m_mapLightIndex[str], b);
	}
}
