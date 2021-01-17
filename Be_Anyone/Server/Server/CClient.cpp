#include "stdafx.h"
#include "CClient.h"

extern CClient g_clients[MAX_USER];

// CClient g_clients[MAX_USER + NUM_NPC + MAX_MONSTER];

bool CClient::Is_SameStatus(OBJSTATUS status)
{
    if (m_status == status)return true;
    else return false;
}
