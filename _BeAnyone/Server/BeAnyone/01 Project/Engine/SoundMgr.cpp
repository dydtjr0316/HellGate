#include "pch.h"
#include "SoundMgr.h"

#include "ResMgr.h"


FMOD_RESULT CHANNEL_CALLBACK(FMOD_CHANNELCONTROL* channelcontrol, FMOD_CHANNELCONTROL_TYPE controltype
	, FMOD_CHANNELCONTROL_CALLBACK_TYPE callbacktype
	, void* commanddata1, void* commanddata2)
{
	FMOD::Channel* cppchannel = (FMOD::Channel*)channelcontrol;
	SoundMgr* pSound = nullptr;

	switch (controltype)
	{
	case FMOD_CHANNELCONTROL_CALLBACK_END:
	{
		cppchannel->getUserData((void**)&pSound);
		pSound->RemoveChannel(cppchannel);
	}
	break;
	}

	return FMOD_OK;
}


FMOD::System* SoundMgr::g_pFMOD = nullptr;

SoundMgr::SoundMgr()
	: CResource(RES_TYPE::SOUND)
	, m_pSound(nullptr)
{
}

SoundMgr::~SoundMgr()
{
	if (nullptr != m_pSound)
	{
		m_pSound->release();
		m_pSound = nullptr;
	}
}

void SoundMgr::Play(int _iRoopCount, bool _bOverlap)
{
	if (_iRoopCount <= -1)
	{
		assert(nullptr);
	}

	// 중복 재생 허용 안할 경우
	if (!_bOverlap && !m_listChannel.empty())
	{
		return;
	}

	_iRoopCount -= 1;

	FMOD::Channel* pChannel = nullptr;
	g_pFMOD->playSound(m_pSound, nullptr, false, &pChannel);

	pChannel->setCallback(CHANNEL_CALLBACK);
	pChannel->setUserData(this);

	pChannel->setMode(FMOD_LOOP_NORMAL);
	pChannel->setLoopCount(_iRoopCount);

	m_listChannel.push_back(pChannel);

	//Stop();
}

void SoundMgr::Stop()
{
	list<FMOD::Channel*>::iterator iter;

	while (!m_listChannel.empty())
	{
		iter = m_listChannel.begin();
		(*iter)->stop();
	}
}

void SoundMgr::VolumControl(const bool& isUp) // true = up
{
	if (isUp)
	{
		if (m_fvolum < 0.95f)m_fvolum += 0.05f;
		else m_fvolum = 1.f;
	}
	else
	{
		if (m_fvolum > 0.05f)m_fvolum -= 0.05f;
		else m_fvolum = 0.f;
	}
	list<FMOD::Channel*>::iterator iter = m_listChannel.begin();
	for (; iter != m_listChannel.end(); ++iter)
	{
		(*iter)->setVolume(m_fvolum);
	}
}

void SoundMgr::RemoveChannel(FMOD::Channel* _pTargetChannel)
{
	list<FMOD::Channel*>::iterator iter = m_listChannel.begin();
	for (; iter != m_listChannel.end(); ++iter)
	{
		if (*iter == _pTargetChannel)
		{
			m_listChannel.erase(iter);
			return;
		}
	}
}

void SoundMgr::Load(const wstring& _strFilePath, const Sound_Type& type)
{
	string path(_strFilePath.begin(), _strFilePath.end());
	//if (type == Sound_Type::BGM)m_fvolum = 0.7f;
	//else m_fvolum = 0.5f;
	//list<FMOD::Channel*>::iterator iter = m_listChannel.begin();
	//(*iter)->setVolume(m_fvolum);
	
	if (FMOD_OK != g_pFMOD->createSound(path.c_str(), FMOD_DEFAULT, nullptr, &m_pSound))
	{
		assert(nullptr);
	}
}

