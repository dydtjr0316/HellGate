#include "pch.h"
#include "Sound.h"

#include "ResMgr.h"


void CSound::Play(Sound_Type type, const bool& isasync, const bool& isloop)
{
	wstring strFullPath = CPathMgr::GetResPath();
	switch (type)
	{
	case Sound_Type::BGM:
		strFullPath += L"Sound\\background.wav";
		break;
	case Sound_Type::HIT:
		strFullPath += L"Sound\\hit.wav";
		break;
	case Sound_Type::GET_COIN:
		strFullPath += L"Sound\\getcoin.wav";
		break;
	case Sound_Type::SET_COIN:
		strFullPath += L"Sound\\spendcoin.wav";
		break;
	default:
		break;
	}
<<<<<<< HEAD
	if (isasync && isloop)	PlaySound(strFullPath.data(), 0, SND_FILENAME | SND_ASYNC | SND_LOOP);
	else if (isloop)		PlaySound(strFullPath.data(), 0, SND_FILENAME | SND_ASYNC | SND_LOOP);
	else if (isasync)		PlaySound(strFullPath.data(), 0, SND_FILENAME | SND_ASYNC);
	else					PlaySound(strFullPath.data(), 0, SND_FILENAME | SND_ASYNC);
=======
	if (isasync && isloop)	PlaySound(strFullPath.data(), 0, SND_FILENAME | SND_SYNC | SND_LOOP);
	else if (isloop)		PlaySound(strFullPath.data(), 0, SND_FILENAME | SND_SYNC | SND_LOOP);
	else if (isasync)		PlaySound(strFullPath.data(), 0, SND_FILENAME | SND_SYNC);
	else					PlaySound(strFullPath.data(), 0, SND_FILENAME | SND_SYNC);
>>>>>>> 79b66a01aacbd4da9fe5bba2efb21a556fb944f3
}
