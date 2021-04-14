#pragma once
#include "../Engine/GameObject.h"
#include "CNetMgr.h"

typedef unordered_map<int, CGameObject*> OBJLIST;
typedef OBJLIST::iterator OBJITER;

extern CNetMgr g_netMgr;
extern OBJLIST g_Object;
extern int g_myid;
extern int testpacket;

// State Description
extern D3D12_RASTERIZER_DESC g_arrRSDesc[(UINT)RS_TYPE::END];
extern D3D12_BLEND_DESC g_arrBlendDesc[(UINT)BLEND_TYPE::END];
extern D3D12_DEPTH_STENCIL_DESC g_arrDepthStencilDesc[(UINT)DEPTH_STENCIL_TYPE::END];