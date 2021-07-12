#pragma once

template <typename T>
void SafeDelete(T& ptr)
{
    if (ptr)
    {
        delete ptr;
        ptr = nullptr;
    }
}

template <typename T>
void SafeDeleteArr(T& ptr)
{
    if (ptr)
    {
        delete[] ptr;
        ptr = nullptr;
    }
}

#define SINGLE(type) private: type(); ~type();\
public:\
static type* GetInst()\
{\
	static type mgr;\
	return &mgr;\
}

#define DeltaTime CTimeMgr::GetInst()->GetDeltaTime()
#define CAST_CLIENT dynamic_cast<CClient*>
#define CAST_MONSTER dynamic_cast<CMonster*>
#define CAST_NPC dynamic_cast<CNPC*>