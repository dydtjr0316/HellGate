#pragma once


// ��ü������ ���� ����


SOCKET l_socket;
HANDLE g_iocp;

// Ŭ���̾�Ʈ �ۼ��� extern CClient g_clients[MAX_USER + NUM_NPC + MAX_MONSTER];

// mutex
mutex timer_lock;

// sector ����
unordered_set<int>				sector[SECTOR_ROW][SECTOR_COL];
priority_queue<event_type>		timer_queue;