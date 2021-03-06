#include "stdafx.h"
#include "CGameObject.h"

CGameObject::CGameObject()
{
    m_id = 0;
    m_v3LocalPosVector = Vector3(0.f, 0.f, 0.f);
    m_v3DirVector = Vector3(0.f, 0.f, 0.f);
    m_prev_size = 0;
    m_iclinet_time = 0;
    m_status = OBJSTATUS::ST_FREE;
    m_s = 0;
    m_lock;
    m_Exover;
    m_deadReckoning_Packet = nullptr;
}

void CGameObject::SetDeadReckoningPacket(cs_packet_move* p)
{
    m_deadReckoning_Packet = p;
}

void CGameObject::SetPosV(const float& x, const float& y, const float& z)
{
    m_v3LocalPosVector.x = x;
    m_v3LocalPosVector.y = y;
    m_v3LocalPosVector.z = z;
}

void CGameObject::SetDirV(const float& x, const float& y, const float& z)
{
    m_v3DirVector.x = x;
    m_v3DirVector.y = y;
    m_v3DirVector.z = z;
}

void CGameObject::SetRotateV(const float& x, const float& y, const float& z)
{
    m_v3RotateVector.x = x;
    m_v3RotateVector.y = y;
    m_v3RotateVector.z = z;
}

void CGameObject::SetIsMoving(const bool& isMoving)
{
    m_bisMoving = isMoving;
}

void CGameObject::Insert_Sector()
{
    //SetSector((int)m_v3LocalPosVector.x / SECTOR_ROW_Length, (int)m_v3LocalPosVector.z / SECTOR_COL_Length);
    g_QuadTree.Insert(this);
    
   // CSectorMgr::GetInst()->Emplace(m_tSector.x, m_tSector.z, m_id);
}

void CGameObject::Change_Sector(const _tSector& old_sector)
{
    //uShort x = (uShort)m_v3LocalPosVector.x / SECTOR_ROW_Length;
    //uShort y = (uShort)m_v3LocalPosVector.z / SECTOR_COL_Length;
    //
    //x = x < 0 ? 0 : x;
    //x = x > SECTOR_ROW - 1 ? SECTOR_ROW - 1 : x;

    //y = y < 0 ? 0 : y;
    //y = y > SECTOR_ROW - 1 ? SECTOR_ROW - 1 : y;
    //
    //SetSector(x, y);

    //if (old_sector.x != m_tSector.x || old_sector.z != m_tSector.z)
    //{

    //   // CSectorMgr::GetInst()->Emplace(m_tSector.x, m_tSector.z, m_id);
    //    //CSectorMgr::GetInst()->Erase(old_sector.x, old_sector.z, m_id);
    //}   
}

void CGameObject::Change_Sector()
{
    g_QuadTree.Delete(this);
    g_QuadTree.Insert(this);
}

