#pragma once


// 전체적으로 추후 수정


extern SOCKET l_socket;
extern HANDLE g_iocp;

// 클라이언트 작성후 extern CClient g_clients[MAX_USER + NUM_NPC + MAX_MONSTER];

// mutex
extern mutex timer_lock;

// sector 수정
extern unordered_set<int>				g_Sector[SECTOR_ROW][SECTOR_COL];
extern priority_queue<event_type>		timer_queue;