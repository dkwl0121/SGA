#pragma once

#define g_pDeviceManager cDeviceManager::GetInstance()
#define g_pD3DDevice cDeviceManager::GetInstance()->GetDevice()

class cDeviceManager
{
private:
	LPDIRECT3D9				m_pD3D;			// D3D9 인터페이스로의 포인터를 담는다.
	LPDIRECT3DDEVICE9		m_pD3DDevice;	// 장치로 부터 D3D9 인터페이스를 만드는데 사용 된다.

public:
	cDeviceManager();
	~cDeviceManager();

	static cDeviceManager* GetInstance()
	{
		static cDeviceManager instance;
		return &instance;
	}

	void Destroy();

	LPDIRECT3DDEVICE9 GetDevice() { return m_pD3DDevice; }
};