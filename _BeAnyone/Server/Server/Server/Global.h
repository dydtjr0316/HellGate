#pragma once
#include "CNetMgr.h"
#include "CClient.h"
#include "CMonster.h"
#include "CQuadTree.h"
#include "CNPC.h"

// ��ü������ ���� ����
#define CLIENT_CAST dynamic_cast<CClient*>
// network
extern CNetMgr Netmgr;
// mutex
extern mutex timer_lock;
extern mutex wakeup_lock;
// sector ����
extern unordered_set<uShort> g_Sector[SECTOR_ROW][SECTOR_COL];
extern priority_queue<event_type>		timer_queue;

extern CQuadTree g_QuadTree;

extern SOCKET g_listenSocket;
extern HANDLE g_IocpHandle;

extern seconds monsterAutoMoveTimer;
extern seconds npcAutoMoveTimer;

