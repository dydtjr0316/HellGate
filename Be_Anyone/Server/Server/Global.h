#pragma once


// 전체적으로 추후 수정


SOCKET l_socket;
HANDLE g_iocp;

// 클라이언트 작성후 extern CClient g_clients[MAX_USER + NUM_NPC + MAX_MONSTER];

// mutex
mutex timer_lock;

// sector 수정
unordered_set<int>				sector[SECTOR_ROW][SECTOR_COL];
priority_queue<event_type>		timer_queue;