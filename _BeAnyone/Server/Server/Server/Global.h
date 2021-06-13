#pragma once
#include "CNetMgr.h"
#include "CClient.h"
#include "CMonster.h"
#include "CNPC.h"

// ��ü������ ���� ����
#define CLIENT_CAST dynamic_cast<CClient*>


extern CNetMgr Netmgr;

extern SOCKET l_socket;
extern HANDLE g_iocp;

// Ŭ���̾�Ʈ �ۼ��� extern CClient g_clients[MAX_USER + NUM_NPC + MAX_MONSTER];

typedef unordered_map<uShort, CGameObject*> OBJLIST;
typedef OBJLIST::iterator OBJITER;

extern OBJLIST g_Object;

typedef unordered_set<uShort> RECKONER;
typedef RECKONER::iterator RECKONER_ITER;

extern RECKONER g_Reckoner;


// mutex
extern mutex timer_lock;
extern mutex wakeup_lock;

// sector ����
extern unordered_set<uShort> g_Sector[SECTOR_ROW][SECTOR_COL];
extern priority_queue<event_type>		timer_queue;

