#include "stdafx.h"
#include "cObjLoader.h"
#include "cMtlTex.h"
#include "cGroup.h"


cObjLoader::cObjLoader()
{
}


cObjLoader::~cObjLoader()
{
	for each (auto p in m_mapMtlTex)
		SAFE_RELEASE(p.second);
}

void cObjLoader::Load(IN char* szFilePath, IN D3DXMATRIXA16* pMat, OUT vector<cGroup*>& vecGroup)
{
	vector<D3DXVECTOR3> vecP;
	vector<D3DXVECTOR3> vecN;
	vector<D3DXVECTOR2> vecT;
	vector<ST_PNT_VERTEX> vecPNTVertex;
	string sMtlName;

	FILE* fp = NULL;
	fopen_s(&fp, szFilePath, "r");

	while (!feof(fp))
	{
		char szTemp[1024];
		fgets(szTemp, 1024, fp);

		if (szTemp[0] == 'm')
		{
			char szMtlPath[1024];
			char szFullPath[1024];
			sscanf_s(szTemp, "%*s ./%s", szMtlPath, (unsigned)_countof(szMtlPath));
			sprintf_s(szFullPath, "obj/%s", szMtlPath);
			LoadMtlLib(szFullPath);
		}
		else if (szTemp[0] == 'v')
		{
			D3DXVECTOR3 v;
			sscanf_s(szTemp, "%*s %f %f %f", &v.x, &v.y, &v.z);

			if (szTemp[1] == 't')
			{
				D3DXVECTOR2 t;
				t.x = v.x;
				t.y = v.y;
				vecT.push_back(t);
			}
			else if (szTemp[1] == 'n')
				vecN.push_back(v);
			else
				vecP.push_back(v);
		}
		else if (szTemp[0] == 'u')
		{
			char szMtlName[1024];
            sscanf_s(szTemp, "%*s %s", szMtlName, (unsigned)_countof(szMtlName));
			sMtlName = szMtlName;
		}
		else if (szTemp[0] == 'f')
		{
			int aIndex[3][3];
            sscanf_s(szTemp, "%*s %d/%d/%d %d/%d/%d %d/%d/%d",
				&aIndex[0][0], &aIndex[0][1], &aIndex[0][2],
				&aIndex[1][0], &aIndex[1][1], &aIndex[1][2],
				&aIndex[2][0], &aIndex[2][1], &aIndex[2][2]);

			for (int i = 0; i < 3; ++i)
			{
				ST_PNT_VERTEX pnt;
				pnt.p = vecP[aIndex[i][0] - 1];
				pnt.t = vecT[aIndex[i][1] - 1];
				pnt.n = vecN[aIndex[i][2] - 1];

				if (pMat)
				{
					D3DXVec3TransformCoord(&pnt.p, &pnt.p, pMat);
					D3DXVec3TransformNormal(&pnt.n, &pnt.n, pMat);
				}
				vecPNTVertex.push_back(pnt);
			}
		}
		else if (szTemp[0] == 'g')
		{
			if (vecPNTVertex.empty()) continue;

			cGroup* pGroup = new cGroup;
            g_pAutoReleasePool->AddObject(pGroup);
            // 마테리얼 설정
            pGroup->SetMtlTex(m_mapMtlTex[sMtlName]);
      
            // 버텍스 버퍼 설정
            ST_PNT_VERTEX*		pPNTVertex;

            // 버텍스 버퍼 생성
            g_pD3DDevice->CreateVertexBuffer(vecPNTVertex.size() * sizeof(ST_PNT_VERTEX), NULL,
                ST_PNT_VERTEX::FVF, D3DPOOL_MANAGED, &pGroup->GetVertexBuffer(), NULL);

            // 버퍼 접근 시 락 설정!
            pGroup->GetVertexBuffer()->Lock(NULL, NULL, (LPVOID*)&pPNTVertex, NULL);

            
            for (int i = 0; i < vecPNTVertex.size(); ++i)
            {
                pPNTVertex[i] = vecPNTVertex[i];
            }

            pGroup->GetVertexBuffer()->Unlock(); // 언락!!


			vecGroup.push_back(pGroup);
			vecPNTVertex.clear();
		}
	}

	fclose(fp);
}

void cObjLoader::LoadMtlLib(char* szFilePath)
{
	FILE* fp = NULL;
	fopen_s(&fp, szFilePath, "r");

	string sMtlName;

	while (!feof(fp))
	{
		char szTemp[1024];
		fgets(szTemp, 1024, fp);

		if (szTemp[0] == 'n')
		{
			char szMtlName[1024];
            sscanf_s(szTemp, "%*s %s", szMtlName, (unsigned)_countof(szMtlName));
			sMtlName = szMtlName;

			SAFE_RELEASE(m_mapMtlTex[sMtlName]);
			m_mapMtlTex[sMtlName] = new cMtlTex;
		}
		else if (szTemp[0] == 'K')
		{
			D3DCOLORVALUE c;
            sscanf_s(szTemp, "%*s %f %f %f", &c.r, &c.g, &c.b);
			c.a = 1.0f;
			D3DMATERIAL9& stMtl = m_mapMtlTex[sMtlName]->GetMtl();

			if (szTemp[1] == 'a')
				stMtl.Ambient = c;
			else if (szTemp[1] == 'd')
				stMtl.Diffuse = c;
			else if (szTemp[1] == 's')
				stMtl.Specular = c;
		}
		else if (szTemp[0] == 'm')
		{
			char szTexturePath[1024];
			char szFullPath[1024];
            sscanf_s(szTemp, "%*s %s", szTexturePath, (unsigned)_countof(szTexturePath));
			sprintf_s(szFullPath, "obj/%s", szTexturePath);

			g_pTextureManager->AddTexture(szTexturePath, szFullPath);
			LPDIRECT3DTEXTURE9 lpTexture = g_pTextureManager->GetTexture(szTexturePath);
			m_mapMtlTex[sMtlName]->SetTexture(lpTexture);
		}
	}

	fclose(fp);
}