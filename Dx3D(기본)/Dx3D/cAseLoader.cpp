#include "stdafx.h"
#include "cAseLoader.h"
#include "Asciitok.h"
#include "cMtlTex.h"
#include "cFrame.h"

cAseLoader::cAseLoader()
    : m_fp(NULL)
    , m_pRoot(NULL)
{
}

cAseLoader::~cAseLoader()
{
}

char* cAseLoader::GetToken()
{
	bool isQuote = false;
	int nReadCount = 0;

	while (true)
	{
		unsigned char c = fgetc(m_fp);	// 한 문자씩 읽는다.

		if (feof(m_fp)) break;			// 오픈 한 파일이 끝났다면 종료

		if (!isQuote && IsWhiteSpace(c))// "가 이전에 나온적이 없고 공백을 만난 경우
		{
			if (nReadCount == 0)		// 현재까지 읽어드린 문자가 없다면 다음 문자를 읽어라
				continue;
			else
				break;					// 현재까지 읽어드린 문자가 있을 경우 한문장 읽기 끝
		}

		if (c == '\"')					// "를 읽은 경우
		{
			if (isQuote)				// 현재 읽은 "가 클로즈인 경우 문장 읽기 끝
				break;

			isQuote = true;				// 현재 읽은 "가 오픈인 경우 문장 읽기 시작(다음 문자 부터)
			continue;
		}

		m_szToken[nReadCount++] = c;
	}

	if (nReadCount == 0) return NULL;

	m_szToken[nReadCount] = '\0';	// 마지막에 문장의 끝을 알리는 널 문자 추가

	return m_szToken;	// 읽어드린 문자를 반환
}

int cAseLoader::GetInteger()
{
	return atoi(GetToken());
}

float cAseLoader::GetFloat()
{
	return (float)atof(GetToken());
}

bool cAseLoader::IsWhiteSpace(char c)
{
	return c < 33;
}

bool cAseLoader::IsEqual(char* str1, char* str2)
{
	return strcmp(str1, str2) == 0;
}

cFrame* cAseLoader::Load(IN char * szFullPath, OUT ST_ASE_SCENE* pScene)
{
	fopen_s(&m_fp, szFullPath, "r");

	while (true)
	{
		char* szToken = GetToken();
		if (szToken == NULL) break;

		if (IsEqual(szToken, ID_SCENE))
		{
            ProcessSCENE(pScene);
		}
		else if (IsEqual(szToken, ID_MATERIAL_LIST))
		{
			ProcessMATERIAL_LIST();
		}
		else if (IsEqual(szToken, ID_GEOMETRY))
		{
			cFrame* pFrame = ProcessGEOMOBJECT();
			if (m_pRoot == NULL)
				m_pRoot = pFrame;
		}
	}

	fclose(m_fp);

	return m_pRoot;
}

void cAseLoader::ProcessSCENE(OUT ST_ASE_SCENE* pScene)
{
    int nLevel = 0;
    do
    {
        char* szToken = GetToken();

        if (IsEqual(szToken, "{"))
            ++nLevel;
        else if (IsEqual(szToken, "}"))
            --nLevel;
        else if (IsEqual(szToken, ID_FIRSTFRAME))
        {
            pScene->nFirstFrame = GetInteger();
        }
        else if (IsEqual(szToken, ID_LASTFRAME))
        {
            pScene->nLastFrame = GetInteger();
        }
        else if (IsEqual(szToken, ID_FRAMESPEED))
        {
            pScene->nFrameSpeed = GetInteger();
        }
        else if (IsEqual(szToken, ID_TICKSPERFRAME))
        {
            pScene->nTicksPerFrame = GetInteger();
        }

    } while (nLevel > 0);
}

void cAseLoader::ProcessMATERIAL_LIST()
{
	int nLevel = 0;
	do
	{
		char* szToken = GetToken();

		if (IsEqual(szToken, "{"))
			++nLevel;
		else if (IsEqual(szToken, "}"))
			--nLevel;
		else if (IsEqual(szToken, ID_MATERIAL_COUNT))
		{
			if (!m_vecMtlTex.empty())
			{
				for each (auto p in m_vecMtlTex)
					SAFE_RELEASE(p);
				m_vecMtlTex.clear();
			}
			m_vecMtlTex.resize(GetInteger());
		}
		else if (IsEqual(szToken, ID_MATERIAL))
		{
			int nMtlRef = GetInteger();
			SAFE_RELEASE(m_vecMtlTex[nMtlRef]);
			m_vecMtlTex[nMtlRef] = new cMtlTex;
			g_pAutoReleasePool->AddObject(m_vecMtlTex[nMtlRef]);
			ProcessMATERIAL(m_vecMtlTex[nMtlRef]);
		}

	} while (nLevel > 0);
}

void cAseLoader::ProcessMATERIAL(OUT cMtlTex* pMtlTex)
{
	int nLevel = 0;
	do
	{
		char* szToken = GetToken();

		if (IsEqual(szToken, "{"))
			++nLevel;
		else if (IsEqual(szToken, "}"))
			--nLevel;
		else if (IsEqual(szToken, ID_AMBIENT))
		{
			D3DMATERIAL9* stMtl = pMtlTex->GetMtl();
			stMtl->Ambient.r = GetFloat();
			stMtl->Ambient.g = GetFloat();
			stMtl->Ambient.b = GetFloat();
			stMtl->Ambient.a = 1.0f;
		}
		else if (IsEqual(szToken, ID_DIFFUSE))
		{
			D3DMATERIAL9* stMtl = pMtlTex->GetMtl();
			stMtl->Diffuse.r = GetFloat();
			stMtl->Diffuse.g = GetFloat();
			stMtl->Diffuse.b = GetFloat();
			stMtl->Diffuse.a = 1.0f;
		}
		else if (IsEqual(szToken, ID_SPECULAR))
		{
			D3DMATERIAL9* stMtl = pMtlTex->GetMtl();
			stMtl->Specular.r = GetFloat();
			stMtl->Specular.g = GetFloat();
			stMtl->Specular.b = GetFloat();
			stMtl->Specular.a = 1.0f;
		}
		else if (IsEqual(szToken, ID_MAP_DIFFUSE))
		{
			ProcessMAP_DIFFUSE(pMtlTex);
		}
	} while (nLevel > 0);
}

void cAseLoader::ProcessMAP_DIFFUSE(OUT cMtlTex* pMtlTex)
{
	int nLevel = 0;
	do
	{
		char* szToken = GetToken();

		if (IsEqual(szToken, "{"))
			++nLevel;
		else if (IsEqual(szToken, "}"))
			--nLevel;
		else if (IsEqual(szToken, ID_BITMAP))
		{
			char szFullPath[1024];
			sprintf_s(szFullPath, "%s", GetToken());
            sscanf_s(szFullPath, "%*c%*c%s", szFullPath, (unsigned)_countof(szFullPath));

			g_pTextureManager->AddTexture(szFullPath, szFullPath);
            //if (g_pTextureManager->GetTexture(szFullPath))
			pMtlTex->SetTexture(g_pTextureManager->GetTexture(szFullPath));
		}
	} while (nLevel > 0);
}

cFrame* cAseLoader::ProcessGEOMOBJECT()
{
    cFrame* pFrame = new cFrame;
    g_pAutoReleasePool->AddObject(pFrame);

    int nLevel = 0;
    do
    {
        char* szToken = GetToken();

        if (IsEqual(szToken, "{"))
            ++nLevel;
        else if (IsEqual(szToken, "}"))
            --nLevel;
        else if (IsEqual(szToken, ID_NODE_NAME))
        {
            string MyName = GetToken();
            //SAFE_RELEASE(m_mapFrame[MyName]); //-> NODE_NAME이 밑에 한 번 더 있어서 셋팅 한걸 지워버리게 됨.
            m_mapFrame[MyName] = pFrame;
        }
        else if (IsEqual(szToken, ID_NODE_PARENT))
        {
            string ParentName = GetToken();
            m_mapFrame[ParentName]->AddChild(pFrame);
        }
        else if (IsEqual(szToken, ID_NODE_TM))
        {
            ProcessNODE_TM(pFrame);
        }
        else if (IsEqual(szToken, ID_MESH))
        {
            ProcessMESH(pFrame);
        }
        else if (IsEqual(szToken, ID_TM_ANIMATION)) //애니메이션 정보)) // 애니메이션 정보가 아예 없는 경우도 있음
        {
            ProcessTM_ANIMATION(pFrame);
        }
        else if (IsEqual(szToken, ID_MATERIAL_REF))
        {
            pFrame->SetMtlTex(m_vecMtlTex[GetInteger()]);
        }
    } while (nLevel > 0);

    return pFrame;
}

void cAseLoader::ProcessNODE_TM(OUT cFrame* pFrame)
{
    D3DXMATRIXA16 matW;
    D3DXMatrixIdentity(&matW);

    int nLevel = 0;
    do
    {
        char* szToken = GetToken();

        if (IsEqual(szToken, "{"))
            ++nLevel;
        else if (IsEqual(szToken, "}"))
            --nLevel;
        else if (IsEqual(szToken, ID_TM_ROW0))
        {
            matW._11 = GetFloat();
            matW._13 = GetFloat();
            matW._12= GetFloat();
        }
        else if (IsEqual(szToken, ID_TM_ROW1))
        {
            matW._31 = GetFloat();
            matW._33 = GetFloat();
            matW._32 = GetFloat();
        }
        else if (IsEqual(szToken, ID_TM_ROW2))
        {
            matW._21 = GetFloat();
            matW._23 = GetFloat();
            matW._22 = GetFloat();
        }
        else if (IsEqual(szToken, ID_TM_ROW3))
        {
            matW._41 = GetFloat();
            matW._43 = GetFloat();
            matW._42 = GetFloat();
        }

    } while (nLevel > 0);

    pFrame->SetWorldTM(matW);
}

void cAseLoader::ProcessMESH(OUT cFrame* pFrame)
{
    vector<D3DXVECTOR3> vecV;
    vector<D3DXVECTOR2> vecT;
    vector<ST_PNT_VERTEX> vecPNT;

    int nLevel = 0;
    do
    {
        char* szToken = GetToken();

        if (IsEqual(szToken, "{"))
            ++nLevel;
        else if (IsEqual(szToken, "}"))
            --nLevel;
        else if (IsEqual(szToken, ID_MESH_NUMVERTEX))
        {
            vecV.resize(GetInteger());
        }
        else if (IsEqual(szToken, ID_MESH_NUMFACES))
        {
            vecPNT.resize(GetInteger() * 3);
        }
        else if (IsEqual(szToken, ID_MESH_VERTEX_LIST))
        {
            ProcessMESH_VERTEX_LIST(vecV);
        }
        else if (IsEqual(szToken, ID_MESH_FACE_LIST))
        {
            ProcessMESH_FACE_LIST(vecV, vecPNT);
        }
        else if (IsEqual(szToken, ID_MESH_NUMTVERTEX))
        {
            vecT.resize(GetInteger());
        }
        else if (IsEqual(szToken, ID_MESH_TVERTLIST))
        {
            ProcessMESH_TVERTLIST(vecT);
        }
        else if (IsEqual(szToken, ID_MESH_TFACELIST))
        {
            ProcessMESH_TFACELIST(vecT, vecPNT);
        }
        else if (IsEqual(szToken, ID_MESH_NORMALS))
        {
            ProcessMESH_NORMALS(vecPNT);
        }

    } while (nLevel > 0);

    //=========================== 월드좌표로 셋팅이 되어있기 때문에 순수로컬좌표로 변경해준다!!!!! ==================================
    D3DXMATRIXA16 matInvNodeTM;
    D3DXMatrixInverse(&matInvNodeTM, NULL, &pFrame->GetWorldTM());
    for (int i = 0; i < vecPNT.size(); ++i)
    {
        D3DXVec3TransformCoord(&vecPNT[i].p, &vecPNT[i].p, &matInvNodeTM);
        D3DXVec3TransformNormal(&vecPNT[i].n, &vecPNT[i].n, &matInvNodeTM);
    }
    //==========================================================================================================================

    pFrame->SetPNTVertex(vecPNT);
}

void cAseLoader::ProcessMESH_VERTEX_LIST(OUT vector<D3DXVECTOR3>& vecV)
{
    int nLevel = 0;
    do
    {
        char* szToken = GetToken();

        if (IsEqual(szToken, "{"))
            ++nLevel;
        else if (IsEqual(szToken, "}"))
            --nLevel;
        else if (IsEqual(szToken, ID_MESH_VERTEX))
        {
            int index = GetInteger();
            vecV[index].x = GetFloat();
            vecV[index].z = GetFloat();
            vecV[index].y = GetFloat();
        }

    } while (nLevel > 0);

}

void cAseLoader::ProcessMESH_FACE_LIST(IN vector<D3DXVECTOR3>& vecV, OUT vector<ST_PNT_VERTEX>& vecPNTVertex)
{
    vector<int> vecF;

    int nLevel = 0;
    do
    {
        char* szToken = GetToken();

        if (IsEqual(szToken, "{"))
            ++nLevel;
        else if (IsEqual(szToken, "}"))
            --nLevel;
        else if (IsEqual(szToken, ID_MESH_FACE))
        {
            GetToken();
            GetToken();
            vecF.push_back(GetInteger());
            GetToken();
            vecF.push_back(GetInteger());
            GetToken();
            vecF.push_back(GetInteger());
        }

    } while (nLevel > 0);

    for (int i = 0; i < vecF.size(); i += 3)
    {
        vecPNTVertex[i + 0].p = vecV[vecF[i + 0]];
        vecPNTVertex[i + 1].p = vecV[vecF[i + 2]];
        vecPNTVertex[i + 2].p = vecV[vecF[i + 1]];
    }
}

void cAseLoader::ProcessMESH_TVERTLIST(OUT vector<D3DXVECTOR2>& vecT)
{
    int nLevel = 0;
    do
    {
        char* szToken = GetToken();

        if (IsEqual(szToken, "{"))
            ++nLevel;
        else if (IsEqual(szToken, "}"))
            --nLevel;
        else if (IsEqual(szToken, ID_MESH_TVERT))
        {
            int index = GetInteger();
            vecT[index].x = GetFloat();
            vecT[index].y = 1.0f - GetFloat();
        }

    } while (nLevel > 0);

}   

void cAseLoader::ProcessMESH_TFACELIST(IN vector<D3DXVECTOR2>& vecT, OUT vector<ST_PNT_VERTEX>& vecPNTVertex)
{
    vector<int> vecF;

    int nLevel = 0;
    do
    {
        char* szToken = GetToken();

        if (IsEqual(szToken, "{"))
            ++nLevel;
        else if (IsEqual(szToken, "}"))
            --nLevel;
        else if (IsEqual(szToken, ID_MESH_TFACE))
        {
            GetToken();
            vecF.push_back(GetInteger());
            vecF.push_back(GetInteger());
            vecF.push_back(GetInteger());
        }

    } while (nLevel > 0);

    for (int i = 0; i < vecF.size(); i += 3)
    {
        vecPNTVertex[i + 0].t = vecT[vecF[i + 0]];
        vecPNTVertex[i + 1].t = vecT[vecF[i + 2]];
        vecPNTVertex[i + 2].t = vecT[vecF[i + 1]];
    }
}

void cAseLoader::ProcessMESH_NORMALS(OUT vector<ST_PNT_VERTEX>& vecPNTVertex)
{
    vector<D3DXVECTOR3> vecN;
    
    int nLevel = 0;
    do
    {
        char* szToken = GetToken();
    
        if (IsEqual(szToken, "{"))
            ++nLevel;
        else if (IsEqual(szToken, "}"))
            --nLevel;
        else if (IsEqual(szToken, ID_MESH_VERTEXNORMAL))
        {
            GetToken();
            D3DXVECTOR3 n;
            n.x = GetFloat();
            n.z = GetFloat();
            n.y = GetFloat();
            vecN.push_back(n);
        }
    
    } while (nLevel > 0);
    
    for (int i = 0; i < vecN.size(); i += 3)
    {
        vecPNTVertex[i + 0].n = vecN[i + 0];
        vecPNTVertex[i + 1].n = vecN[i + 2];
        vecPNTVertex[i + 2].n = vecN[i + 1];
    }
}

void cAseLoader::ProcessTM_ANIMATION(OUT cFrame* pFrame)
{
    int nLevel = 0;
    do
    {
        char* szToken = GetToken();

        if (IsEqual(szToken, "{"))
            ++nLevel;
        else if (IsEqual(szToken, "}"))
            --nLevel;
        else if (IsEqual(szToken, ID_POS_TRACK))
        {
            ProcessCONTROL_POS_TRACK(pFrame);
        }
        else if (IsEqual(szToken, ID_ROT_TRACK))
        {
            ProcessCONTROL_ROT_TRACK(pFrame);
        }

    } while (nLevel > 0);
}

void cAseLoader::ProcessCONTROL_POS_TRACK(OUT cFrame* pFrame)
{
    int nLevel = 0;
    do
    {
        char* szToken = GetToken();

        if (IsEqual(szToken, "{"))
            ++nLevel;
        else if (IsEqual(szToken, "}"))
            --nLevel;
        else if (IsEqual(szToken, ID_POS_SAMPLE))
        {
            ST_POS_SAMPLE stPos;
            stPos.n = GetInteger();
            stPos.v.x = GetFloat();
            stPos.v.z = GetFloat();
            stPos.v.y = GetFloat();
            pFrame->GetPosTrack().push_back(stPos);
        }

    } while (nLevel > 0);
}

void cAseLoader::ProcessCONTROL_ROT_TRACK(OUT cFrame* pFrame)
{
    int nLevel = 0;
    do
    {
        char* szToken = GetToken();

        if (IsEqual(szToken, "{"))
            ++nLevel;
        else if (IsEqual(szToken, "}"))
            --nLevel;
        else if (IsEqual(szToken, ID_ROT_SAMPLE))
        {
            ST_ROT_SAMPLE stRot;
            stRot.n = GetInteger();
            stRot.q.x = GetFloat();
            stRot.q.z = GetFloat();
            stRot.q.y = GetFloat();
            stRot.q.w = GetFloat();

            //// 밑에 방식과 같은 결과 (쿼터니움 값으로 변환)
            //D3DXVECTOR3 vrot(stRot.q.x, stRot.q.y, stRot.q.z);
            //D3DXQUATERNION rot;
            //D3DXQuaternionRotationAxis(&rot, &vrot, stRot.q.w);
            //stRot.q = rot;

            stRot.q.x *= sinf(stRot.q.w / 2.0f);
            stRot.q.z *= sinf(stRot.q.w / 2.0f);
            stRot.q.y *= sinf(stRot.q.w / 2.0f);
            stRot.q.w = cosf(stRot.q.w / 2.0f);

            // 이전 로테이션 샘플이 있으면 -> 마지막값과 곱하기
            if (!pFrame->GetRotTrack().empty())
            {
                stRot.q = pFrame->GetRotTrack().back().q * stRot.q; // 쿼터니언 값은 곱하기의 순서가 중요함!!(기준을 앞으로)
            }
            pFrame->GetRotTrack().push_back(stRot);
        }

    } while (nLevel > 0);
}

