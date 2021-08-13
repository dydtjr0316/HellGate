#pragma once
#include <iostream>
#include <Windows.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

class CSound
{
private:
	CSound() {};
	~CSound() {};
public:
	static CSound* GetInst()
	{
		static CSound mgr;
		return &mgr;
	}
public:
	void Play(Sound_Type type, const bool& isasync = true, const bool& isloop = false);

};

