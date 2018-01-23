#include "stdafx.h"
#include "D3DUtility.h"

namespace D3DUTILITY
{
	D3DMATERIAL9 InitMtrl(D3DXCOLOR a, D3DXCOLOR d, D3DXCOLOR s, D3DXCOLOR e, float p)
	{
		D3DMATERIAL9 mtrl;
		mtrl.Ambient = a;
		mtrl.Diffuse = d;
		mtrl.Specular = s;
		mtrl.Emissive = e;
		mtrl.Power = p;

		return mtrl;
	}

	void ComputeNormal(D3DXVECTOR3* out, D3DXVECTOR3* p0, D3DXVECTOR3* p1, D3DXVECTOR3* p2)
	{
		D3DXVECTOR3 v01 = *p1 - *p0;
		D3DXVECTOR3 v02 = *p2 - *p0;

		D3DXVec3Cross(out, &v01, &v02);
		D3DXVec3Normalize(out, out);
	}

	D3DLIGHT9 InitDirectional(D3DXVECTOR3* dir, D3DXCOLOR* c)
	{
		D3DLIGHT9 stLight;
		ZeroMemory(&stLight, sizeof(D3DLIGHT9));

		stLight.Type = D3DLIGHT_DIRECTIONAL;
		stLight.Ambient = *c * 0.4f;
		stLight.Diffuse = *c;
		stLight.Specular = *c * 0.6f;
		stLight.Direction = *dir;

		return stLight;
	}

	D3DLIGHT9 InitPoint(D3DXVECTOR3* pos, D3DXCOLOR* c)
	{
		D3DLIGHT9 stLight;
		ZeroMemory(&stLight, sizeof(D3DLIGHT9));

		stLight.Type = D3DLIGHT_POINT;
		stLight.Ambient = *c * 0.4f;
		stLight.Diffuse = *c;
		stLight.Specular = *c * 0.6f;
		stLight.Position = *pos;
		stLight.Range = 20.0f;
		stLight.Falloff = 1.0f;
		stLight.Attenuation0 = 0.005f;
		stLight.Attenuation1 = 0.005f;
		stLight.Attenuation2 = 0.005f;

		return stLight;
	}

	D3DLIGHT9 InitSpot(D3DXVECTOR3* dir, D3DXVECTOR3* pos, D3DXCOLOR* c)
	{
		D3DLIGHT9 stLight;
		ZeroMemory(&stLight, sizeof(D3DLIGHT9));

		stLight.Type = D3DLIGHT_SPOT;
		stLight.Ambient = *c * 0.4f;
		stLight.Diffuse = *c;
		stLight.Specular = *c * 0.6f;
		stLight.Direction = *dir;
		stLight.Position = *pos;
		stLight.Range = 50.0f;
		stLight.Falloff = 1.0f;
		stLight.Theta = 0.0f;
		stLight.Phi = D3DX_PI / 6;
		stLight.Attenuation0 = 0.005f;
		stLight.Attenuation1 = 0.005f;
		stLight.Attenuation2 = 0.005f;

		return stLight;
	}
}