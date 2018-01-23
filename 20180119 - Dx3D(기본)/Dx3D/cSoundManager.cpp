#include "stdafx.h"
#include "cSoundManager.h"


cSoundManager::cSoundManager()
{
	// 사운드 시스템 생성
	System_Create(&m_pSystem);

	// 사운드 채널수 설정
	m_pSystem->init(TOTALSOUNDBUFFER, FMOD_INIT_NORMAL, NULL);

	// 사운드, 채널 동적 할당
	m_ppSound = new Sound*[TOTALSOUNDBUFFER];
	m_ppChannel = new Channel*[TOTALSOUNDBUFFER];

	// 메모리 초기화
	memset(m_ppSound, 0, sizeof(Sound*) * TOTALSOUNDBUFFER);
	memset(m_ppChannel, 0, sizeof(Channel*) * TOTALSOUNDBUFFER);
}


cSoundManager::~cSoundManager()
{
	// 사운드 및 채널 삭제
	if (m_ppChannel != NULL || m_ppSound != NULL)
	{
		for (int i = 0; i < TOTALSOUNDBUFFER; ++i)
		{
			if (m_ppChannel != NULL)
			{
				if (m_ppChannel[i])
					m_ppChannel[i]->stop();
			}

			if (m_ppSound != NULL)
			{
				if (m_ppSound[i])
					m_ppSound[i]->release();
			}
		}
	}

	// 메모리 지우기
	SAFE_DELETE(m_ppChannel);
	SAFE_DELETE(m_ppSound);

	// 사운드 시스템 닫기
	if (m_pSystem != NULL)
	{
		m_pSystem->release();
		m_pSystem->close();
	}
}

void cSoundManager::Update()
{
	// 사운드 시스템을 계속 업데이트 해줘야
	// 볼륨이 바뀌거나, 재생이 끝난 사운드를 채널에서 빼는 등의
	// 다양한 작업을 자동으로 해준다.
	m_pSystem->update();
}

void cSoundManager::AddSound(string key, string fileName, bool bgm, bool loop)
{
	if (loop)
	{
		if (bgm) // 배경음이냐?
		{
			m_pSystem->createStream(fileName.c_str(), FMOD_LOOP_NORMAL, NULL, &m_ppSound[m_mTotalSounds.size()]);
		}
		else // 이펙트냐?
		{
			m_pSystem->createSound(fileName.c_str(), FMOD_LOOP_NORMAL, NULL, &m_ppSound[m_mTotalSounds.size()]);
		}
	}
	else
	{
		m_pSystem->createSound(fileName.c_str(), FMOD_DEFAULT, NULL, &m_ppSound[m_mTotalSounds.size()]);
	}

	// 맵에 사운드를 추가 한다.
	m_mTotalSounds.insert(make_pair(key, &m_ppSound[m_mTotalSounds.size()]));
}

void cSoundManager::Play(string key, float volume)
{
	int count = 0;
	arrSoundIter iter = m_mTotalSounds.begin();
	for (; iter != m_mTotalSounds.end(); ++iter, ++count)
	{
		if (key == iter->first)
		{
			// 사운드 플레이
			m_pSystem->playSound(FMOD_CHANNEL_FREE, *iter->second, false, &m_ppChannel[count]);
			// 볼륨 셋팅
			m_ppChannel[count]->setVolume(volume);
			break;
		}
	}
}

void cSoundManager::Stop(string key)
{
	int count = 0;
	arrSoundIter iter = m_mTotalSounds.begin();
	for (; iter != m_mTotalSounds.end(); ++iter, ++count)
	{
		if (key == iter->first)
		{
			m_ppChannel[count]->stop();
			break;
		}
	}
}

void cSoundManager::Pause(string key)
{
	int count = 0;
	arrSoundIter iter = m_mTotalSounds.begin();
	for (; iter != m_mTotalSounds.end(); ++iter, ++count)
	{
		if (key == iter->first)
		{
			m_ppChannel[count]->setPaused(true);
			break;
		}
	}
}

void cSoundManager::Resume(string key)
{
	int count = 0;
	arrSoundIter iter = m_mTotalSounds.begin();
	for (; iter != m_mTotalSounds.end(); ++iter, ++count)
	{
		if (key == iter->first)
		{
			m_ppChannel[count]->setPaused(false);
			break;
		}
	}
}

bool cSoundManager::IsPlaySound(string key)
{
	bool isPlay = false;
	int count = 0;
	arrSoundIter iter = m_mTotalSounds.begin();
	for (; iter != m_mTotalSounds.end(); ++iter, ++count)
	{
		if (key == iter->first)
		{
			m_ppChannel[count]->isPlaying(&isPlay);
			break;
		}
	}

	return isPlay;
}

bool cSoundManager::IsPauseSound(string key)
{
	bool isPause = false;
	int count = 0;
	arrSoundIter iter = m_mTotalSounds.begin();
	for (; iter != m_mTotalSounds.end(); ++iter, ++count)
	{
		if (key == iter->first)
		{
			m_ppChannel[count]->getPaused(&isPause);
			break;
		}
	}

	return isPause;
}

void cSoundManager::VolumeUp(string key)
{
	float volume;
	int count = 0;
	arrSoundIter iter = m_mTotalSounds.begin();
	for (; iter != m_mTotalSounds.end(); ++iter, ++count)
	{
		if (key == iter->first)
		{
			m_ppChannel[count]->getVolume(&volume);
			m_ppChannel[count]->setVolume(volume + 0.1f);
			break;
		}
	}
}

void cSoundManager::VolumeDown(string key)
{
	float volume;
	int count = 0;
	arrSoundIter iter = m_mTotalSounds.begin();
	for (; iter != m_mTotalSounds.end(); ++iter, ++count)
	{
		if (key == iter->first)
		{
			m_ppChannel[count]->getVolume(&volume);
			m_ppChannel[count]->setVolume(volume - 0.1f);
			break;
		}
	}
}
