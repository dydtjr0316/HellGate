#include "stdafx.h"
#include "CGameObject.h"


//void CGameObject::Insert_Sector()
//{
//	m_tCector.x = m_iX / SECTOR_ROW_Length;
//	m_tCector.z = m_iZ / SECTOR_COL_Length;
//
//	g_Sector[m_tCector.x][m_tCector.z].emplace(m_id);
//}
//
//void CGameObject::Change_Sector()
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