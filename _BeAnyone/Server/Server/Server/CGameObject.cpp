#include "stdafx.h"
#include "CGameObject.h"

CGameObject::CGameObject()
{
    m_id = 0;
    m_iX = m_iY = m_iZ = 0.f; 
    m_prev_size = 0;
    m_iclinet_time = 0;
    m_status = OBJSTATUS::ST_FREE;
    m_s = 0;
    m_tSector;
    m_lock;
    m_Exover;
}

void CGameObject::Insert_Sector()
{
    SetSector((int)m_iX / SECTOR_ROW_Length, (int)m_iZ / SECTOR_COL_Length);
    g_Sector[m_tSector.x][m_tSector.z].emplace(m_id);

    if (m_id < 2)
    {
        cout << "좌표 -> " << m_iX << ", " << m_iZ << endl;
        cout << "섹터 -> " << m_tSector.x << ", " << m_tSector.z << endl;
    }
}

void CGameObject::Change_Sector(const _tSector& old_sector)
{
    SetSector((int)m_iX / SECTOR_COL_Length, (int)m_iZ / SECTOR_ROW_Length);

    if (old_sector.x != m_tSector.x || old_sector.z != m_tSector.z)
    {
        g_Sector[m_tSector.x][m_tSector.z].insert(m_id);
        // 
        g_Sector[old_sector.x][old_sector.z].erase(m_id);
    }
}

vector<unordered_set<int>> CGameObject::Search_Sector()
{
    vector<unordered_set<int>> vSectors;

    // sector로 전부 변경 불가?
    int x1 = ((int)m_iX - VIEW_LIMIT) / SECTOR_COL_Length;
    int z1 = ((int)m_iZ - VIEW_LIMIT) / SECTOR_ROW_Length;

    int x2 = ((int)m_iX + VIEW_LIMIT) / SECTOR_COL_Length;
    int z2 = ((int)m_iZ + VIEW_LIMIT) / SECTOR_ROW_Length;

    // 여기 int 형 으로 그냥 맵 반환해서 int 형 변수로 섹터 검샋해도될거같은대ㅔ?

    if (x1 == x2 && z1 == z2) // 섹터 하나 검색
    {
        vSectors.push_back(g_Sector[x1][z1]);
    }
    else if (x1 == x2) // 상*하 검색
    {
        vSectors.push_back(g_Sector[x1][z1]);
        vSectors.push_back(g_Sector[x1][z2]);
    }
    else if (z1 == z2) // 좌*우 검색
    {
        vSectors.push_back(g_Sector[x1][z1]);
        vSectors.push_back(g_Sector[x2][z1]);
    }
    else // 사방향 검색
    {
        vSectors.push_back(g_Sector[x1][z2]);
        vSectors.push_back(g_Sector[x2][z2]);
        vSectors.push_back(g_Sector[x1][z1]);
        vSectors.push_back(g_Sector[x2][z1]);
    }

    return vSectors;
}

