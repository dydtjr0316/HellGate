#include "stdafx.h"
#include "CGameObject.h"
mutex SectorLock;
CGameObject::CGameObject()
{
    m_id = 0;
    m_v3LocalPosVector = Vector3(0.f, 0.f, 0.f);
    m_v3RotateVector = Vector3(0.f, 0.f, 0.f);
    m_v3DirVector = Vector3(0.f, 0.f, 0.f);
    m_prev_size = 0;
    m_iclinet_time = 0;
    m_status = OBJSTATUS::ST_FREE;
    m_s = 0;
    m_deadReckoning_Packet = nullptr;
}

void CGameObject::SetDeadReckoningPacket(cs_packet_move* p)
{
    m_deadReckoning_Packet = p;
    packetRotateY = p->rotateY;
    packetDirVec = p->DirVec;
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
    g_QuadTree.Insert(this);
}

void CGameObject::Change_Sector()
{
    g_QuadTree.Delete(this);
    g_QuadTree.Insert(this);
}

