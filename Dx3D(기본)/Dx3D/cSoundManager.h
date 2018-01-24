#pragma once

#include "inc/fmod.hpp"
#pragma comment(lib, "lib/fmodex_vc.lib")	// lib 링크
using namespace FMOD;						// FMOD 네임스페이스

											// 사운드 갯수 설정
#define SOUNDBUFFER (20)
											// 채널 버퍼
#define EXTRACHANNELBUFFER (5)
											// 총 사운드 버퍼 : 버퍼가 작아서 씹히는 문제를 해결하기 위함
#define TOTALSOUNDBUFFER (SOUNDBUFFER + EXTRACHANNELBUFFER)

#define g_pSoundManager cSoundManager::GetInstance()

class cSoundManager
{
    SINGLETON(cSoundManager);

private:
	typedef map<string, Sound**> arrSounds;
	typedef map<string, Sound**>::iterator arrSoundIter;

private:
	System*		m_pSystem;		// 시스템 클래스 포인터
	Sound**		m_ppSound;		// 사운드 클래스 포인터
	Channel**	m_ppChannel;	// 채널 클래스 포인터

	arrSounds	m_mTotalSounds;	// 전체 사운드 목록

public:

	void Update();
	void AddSound(string key, string fileName, bool bgm = false, bool loop = false);

	void Play(string key, float volume = 1.0f); // 볼륨 (0.0f ~ 1.0f)
	void Stop(string key);
	void Pause(string key);
	void Resume(string key);
	bool IsPlaySound(string key);
	bool IsPauseSound(string key);
	void VolumeUp(string key);
	void VolumeDown(string key);
};

