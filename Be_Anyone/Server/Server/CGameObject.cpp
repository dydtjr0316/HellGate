#include "stdafx.h"
#include "CGameObject.h"

void CGameObject::Insert_Sector()
{
    SetSector(m_iX / SECTOR_ROW_Length, m_iY / SECTOR_COL_Length);
    g_Sector[m_tSector.x][m_tSector.y].emplace(m_id);
}

void CGameObject::Change_Sector(const _tSector& old_sector)
{
    SetSector(m_iX / SECTOR_COL_Length, m_iY / SECTOR_ROW_Length);

    if (old_sector.x != m_tSector.x || old_sector.y != m_tSector.y)
    {
        g_Sector[m_tSector.x][m_tSector.y].insert(m_id);
        // 
        g_Sector[old_sector.x][old_sector.y].erase(m_id);
    }
}

vector<unordered_set<int>> CGameObject::Search_Sector()
{
    vector<unordered_set<int>> vSectors;

    int x1 = (m_iX - VIEW_LIMIT) / SECTOR_COL_Length;
    int z1 = (m_iY - VIEW_LIMIT) / SECTOR_ROW_Length;

    int x2 = (m_iX + VIEW_LIMIT) / SECTOR_COL_Length;
    int z2 = (m_iY + VIEW_LIMIT) / SECTOR_ROW_Length;

    // ���� int �� ���� �׳� �� ��ȯ�ؼ� int �� ������ ���� �˘��ص��ɰŰ������?

    if (x1 == x2 && z1 == z2) // ���� �ϳ� �˻�
    {
        vSectors.push_back(g_Sector[x1][z1]);
    }
    else if (x1 == x2) // ��*�� �˻�
    {
        vSectors.push_back(g_Sector[x1][z1]);
        vSectors.push_back(g_Sector[x1][z2]);
    }
    else if (z1 == z2) // ��*�� �˻�
    {
        vSectors.push_back(g_Sector[x1][z1]);
        vSectors.push_back(g_Sector[x2][z1]);
    }
    else // ����� �˻�
    {
        vSectors.push_back(g_Sector[x1][z2]);
        vSectors.push_back(g_Sector[x2][z2]);
        vSectors.push_back(g_Sector[x1][z1]);
        vSectors.push_back(g_Sector[x2][z1]);
    }

    return vSectors;
}

