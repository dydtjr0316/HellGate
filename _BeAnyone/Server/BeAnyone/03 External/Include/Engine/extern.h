#pragma once
#include <unordered_map>
#include "GameObject.h"
#include "CNetMgr.h"
#include "SoundMgr.h"

typedef unordered_map<int, CGameObject*> OBJLIST;
typedef OBJLIST::iterator OBJITER;

typedef unordered_map<int, CGameObject*> CAMOBJLIST;
typedef OBJLIST::iterator CAMOBJITER;

extern unordered_map<Sound_Type, SoundMgr*> g_SoundList;

extern CNetMgr g_netMgr;
extern OBJLIST g_Object;
extern CAMOBJLIST g_CamObject;
extern int g_myid;
extern int testpacket;
extern int frameCnt;
extern int movePacketSendCnt;

extern system_clock::time_point MoveStart;
extern system_clock::time_point MoveEnd;

//temp
extern int ctnt;
 extern int anicnt;
// 
// State Description
extern D3D12_RASTERIZER_DESC g_arrRSDesc[(UINT)RS_TYPE::END];
extern D3D12_BLEND_DESC g_arrBlendDesc[(UINT)BLEND_TYPE::END];
extern D3D12_DEPTH_STENCIL_DESC g_arrDepthStencilDesc[(UINT)DEPTH_STENCIL_TYPE::END];
extern tGlobalValue g_global;