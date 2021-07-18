#include "stdafx.h"
#include "CClient.h"
#include "CTimeMgr.h"

extern CClient g_clients[MAX_USER];

// CClient g_clients[MAX_USER + NUM_NPC + MAX_MONSTER];

bool CClient::Is_SameStatus(OBJSTATUS status)
{
    if (m_status == status)return true;
    else return false;
}

void CClient::SetFirstPos(const float& x, const float& y, const float& z)
{
    m_v3FirstPos.x = x;
    m_v3FirstPos.y = y;
    m_v3FirstPos.z = z;
}
