#include "stdafx.h"
#include "CGameObject.h"
#include "CClient.h"


// CClient g_clients[MAX_USER + NUM_NPC + MAX_MONSTER];

void CClient::Insert_Sector()
{
	m_tCector.x = m_iX / (WORLD_WIDTH / SECTOR_COL);
	m_tCector.z = m_iZ / (WORLD_WIDTH / SECTOR_COL);

	g_Sector[m_tCector.x][m_tCector.z].emplace(m_id);
}

void CClient::Change_Sector()
{
	_tSector old_Sector(m_tCector.x, m_tCector.z);
	m_tCector.x = m_iX / (WORLD_WIDTH / SECTOR_COL);
	m_tCector.z = m_iZ / (WORLD_WIDTH / SECTOR_COL);

	if (old_Sector.x != m_tCector.x || old_Sector.z != m_tCector.z)
	{
		g_Sector[old_Sector.x][old_Sector.z].erase(m_id);
		g_Sector[m_tCector.x][m_tCector.z].emplace(m_id);
	}
}

vector<unordered_set<int>>& CClient::Search_Sector()
{
    int x1;
    int x2;
    int y1;
    int y2;
    vector<unordered_set<int>> vSectors;

    if (m_iX<VIEW_LIMIT || m_iX>WORLD_WIDTH - VIEW_LIMIT)
    {
        x1 = m_iX / SECTOR_COL_Length;
        x2 = m_iX / SECTOR_COL_Length;
    }
    else
    {
        x1 = (m_iX - VIEW_LIMIT) / SECTOR_COL_Length;
        x2 = (m_iX + VIEW_LIMIT) / SECTOR_COL_Length;
    }

    if (m_iY<VIEW_LIMIT || m_iY>WORLD_WIDTH - VIEW_LIMIT)
    {
        y1 = m_iY / SECTOR_ROW_Length;
        y2 = m_iY / SECTOR_ROW_Length;
    }
    else
    {
        y1 = (m_iY - VIEW_LIMIT) / SECTOR_ROW_Length;
        y2 = (m_iY + VIEW_LIMIT) / SECTOR_ROW_Length;
    }

    if (x1 == x2 && y1 == y2) // 섹터 하나 검색
    {
        vSectors.push_back(g_Sector[x1][y1]);
    }
    else if (x1 == x2) // 상*하 검색
    {
        vSectors.push_back(g_Sector[x1][y1]);
        vSectors.push_back(g_Sector[x1][y2]);
    }
    else if (y1 == y2) // 좌*우 검색
    {
        vSectors.push_back(g_Sector[x1][y1]);
        vSectors.push_back(g_Sector[x2][y1]);
    }
    else // 사방향 검색
    {
        vSectors.push_back(g_Sector[x1][y2]);
        vSectors.push_back(g_Sector[x2][y2]);
        vSectors.push_back(g_Sector[x1][y1]);
        vSectors.push_back(g_Sector[x2][y1]);
    }
    
    return vSectors;
}
