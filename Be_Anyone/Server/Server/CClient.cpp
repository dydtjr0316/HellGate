#include "stdafx.h"
#include "CClient.h"

extern CClient g_clients[MAX_USER];

// CClient g_clients[MAX_USER + NUM_NPC + MAX_MONSTER];

bool CClient::Is_SameStatus(ENUM_STATUS status)
{
    if (m_status == status)return true;
    else return false;
}

//void CClient::Insert_Sector()
//{
//	m_tCector.x = m_iX / SECTOR_ROW_Length;
//	m_tCector.z = m_iZ / SECTOR_COL_Length;
//
//	g_Sector[m_tCector.x][m_tCector.z].emplace(m_id);
//}
//
//void CClient::Change_Sector()
//{
//	_tSector old_Sector(m_tCector.x, m_tCector.z);
//	m_tCector.x = m_iX / SECTOR_ROW_Length;
//	m_tCector.z = m_iZ / SECTOR_COL_Length;
//
//	if (old_Sector.x != m_tCector.x || old_Sector.z != m_tCector.z)
//	{
//		g_Sector[old_Sector.x][old_Sector.z].erase(m_id);
//		g_Sector[m_tCector.x][m_tCector.z].emplace(m_id);
//	}
//}

//
//
//vector<unordered_set<int>>& CClient::Search_Sector()
//{
//    vector<unordered_set<int>> vSectors;
//
//    int x1 = (m_iX - VIEW_LIMIT) / SECTOR_COL_Length;
//    int z1 = (m_iZ - VIEW_LIMIT) / SECTOR_ROW_Length;
//
//    int x2 = (m_iX + VIEW_LIMIT) / SECTOR_COL_Length;
//    int z2 = (m_iZ + VIEW_LIMIT) / SECTOR_ROW_Length; 
//
//    // ���� int �� ���� �׳� �� ��ȯ�ؼ� int �� ������ ���� �˘��ص��ɰŰ������?
//
//    if (x1 == x2 && z1 == z2) // ���� �ϳ� �˻�
//    {
//        vSectors.push_back(g_Sector[x1][z1]);
//    }
//    else if (x1 == x2) // ��*�� �˻�
//    {
//        vSectors.push_back(g_Sector[x1][z1]);
//        vSectors.push_back(g_Sector[x1][z2]);
//    }
//    else if (z1 == z2) // ��*�� �˻�
//    {
//        vSectors.push_back(g_Sector[x1][z1]);
//        vSectors.push_back(g_Sector[x2][z1]);
//    }
//    else // ����� �˻�
//    {
//        vSectors.push_back(g_Sector[x1][z2]);
//        vSectors.push_back(g_Sector[x2][z2]);
//        vSectors.push_back(g_Sector[x1][z1]);
//        vSectors.push_back(g_Sector[x2][z1]);
//    }
//
//    return vSectors;
//}
