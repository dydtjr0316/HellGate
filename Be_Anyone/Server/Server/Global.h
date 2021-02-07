#pragma once
#include "CClient.h"
#include "CMonster.h"
#include "CNPC.h"

// ��ü������ ���� ����

extern SOCKET l_socket;
extern HANDLE g_iocp;

// Ŭ���̾�Ʈ �ۼ��� extern CClient g_clients[MAX_USER + NUM_NPC + MAX_MONSTER];

typedef unordered_map<int, CGameObject*> OBJLIST;
typedef OBJLIST::iterator OBJITER;

extern OBJLIST g_Object;

// mutex
extern mutex timer_lock;
extern mutex wakeup_lock;

// sector ����
extern unordered_set<int> g_Sector[SECTOR_ROW][SECTOR_COL];
extern priority_queue<event_type>		timer_queue;

