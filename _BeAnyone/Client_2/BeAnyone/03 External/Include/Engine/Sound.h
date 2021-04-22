#pragma once
#include "../../03 External/Include/FMOD/fmod.h"
#include "../../03 External/Include/FMOD/fmod.hpp"
#include "../../03 External/Include/FMOD/fmod_codec.h"

#ifdef _DEBUG
#pragma comment(lib, "../../03 External/Library/FMOD/x64/fmodL64_vc.lib")
#else
#pragma comment(lib, "../../03 External/Library/FMOD/x64/fmod64_vc.lib")
#endif

#include "Resource.h"
class CSound
{
public:
	CSound();
	virtual ~CSound();
	int a = 0;
};
//class CSound :
//	public CResource
//{
//public:
//	static FMOD::System* g_pFMOD;
//
//private:
//	FMOD::Sound* m_pSound;
//	list<FMOD::Channel*>	m_listChannel;
//
//
//
//public:
//	// 0 (무한반복) 
//	void Play( int _iRoopCount, bool _bOverlap = false );
//	void Stop();
//
//	void RemoveChannel( FMOD::Channel* _pTargetChannel );
//
//public:
//	virtual void Load( const wstring& _strFilePath );
//
//public:
//	CSound();
//	virtual ~CSound();
//};

