#pragma once
#include "CClient.h"
#include "CMonster.h"
#include "CNPC.h"

// ��ü������ ���� ����

extern CClient g_clients[MAX_USER];
extern CMonster g_monsters[MAX_MONSTER];
extern CNPC g_npcs[MAX_NPC];


extern SOCKET l_socket;
extern HANDLE g_iocp;

// Ŭ���̾�Ʈ �ۼ��� extern CClient g_clients[MAX_USER + NUM_NPC + MAX_MONSTER];

// mutex
extern mutex timer_lock;
extern mutex wakeup_lock;

// sector ����
extern unordered_multiset<int>				g_Sector[OBJID::END][SECTOR_ROW][SECTOR_COL];
extern priority_queue<event_type>		timer_queue;

