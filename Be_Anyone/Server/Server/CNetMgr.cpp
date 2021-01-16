#include "stdafx.h"
#include "CNetMgr.h"

bool CNetMgr::is_near(const OBJID& p1eID, const int& p1,const OBJID& p2eID, const int& p2)
{
    int dist = (Find(p1eID, p1)->GetX() - Find(p2eID, p2)->GetX()) * (Find(p1eID, p1)->GetX() - Find(p2eID, p2)->GetX());
    dist += (Find(p1eID, p1)->GetY() - Find(p2eID, p2)->GetY()) * (Find(p1eID, p1)->GetY() - Find(p2eID, p2)->GetY());

    return dist <= (VIEW_LIMIT * VIEW_LIMIT);
}

void CNetMgr::Add(CGameObject* pObj, const OBJID& eID, const int& id)
{
    if (pObj != nullptr)
        m_ObjLst[eID].emplace(id, pObj);
}

CGameObject* CNetMgr::Find(const OBJID& eID, const int& id)
{
    if (!m_ObjLst[eID].empty())
    {
        OBJITER iter;
        iter = m_ObjLst[eID].find(id);
        return (iter->second);
    }

    return nullptr;

}

void CNetMgr::Delete_lst(const OBJID& eID)
{
    for (auto& objlist : m_ObjLst[eID])
    {
        delete (objlist).second;
        (objlist).second = nullptr;
    }

    m_ObjLst[eID].clear();

}

void CNetMgr::Delete_Obj(const OBJID& eID, const int& id)
{
    if (!m_ObjLst[eID].empty())
    {
        SafeDelete(m_ObjLst[eID].find(id)->second);
        m_ObjLst[eID].erase(id);
    }
}

int CNetMgr::Count(const OBJID& eID, const int& id)
{
    if (!m_ObjLst[eID].empty())
    {
        return m_ObjLst[eID].count(id);
    }

    return 0;
}

int CNetMgr::Size(const OBJID& eID)
{
    return m_ObjLst[eID].size();
}

void CNetMgr::Send_Packet(const OBJID& eID, const int& id, void* packet)
{
    unsigned char* buf = reinterpret_cast<unsigned char*>(packet);

    EXOVER* exover = new EXOVER;
    ZeroMemory(&exover->io_buf, sizeof(exover->io_buf));
    memcpy(exover->io_buf, buf, buf[0]);

    exover->op = ENUMOP::OP_SEND;
    exover->wsabuf.buf = reinterpret_cast<char*>(exover->io_buf);
    exover->wsabuf.len = buf[0];
    ZeroMemory(&exover->over, sizeof(exover->over));

    Find(eID, id)->GetLock().lock();
    WSASend(Find(eID, id)->GetSocket(), &exover->wsabuf, 1, NULL, 0,
        &exover->over, NULL);
    Find(eID, id)->GetLock().unlock();
    
}

void CNetMgr::Send_LevelUP_Packet(const int& id)
{
    sc_packet_level_up p;
    p.id = id;
    p.size = sizeof(sc_packet_level_up);
    p.type = SC_PACKET_LEVEL_UP;

    
    p.max_hp = Find(OBJID::CLIENT, id)->GetMaxHP();
    p.level = dynamic_cast<CClient*>(Find(OBJID::CLIENT, id))->GetLevel();
    p.attack_damage = Find(OBJID::CLIENT, id)->GetAttackDamage();
    p.hp = Find(OBJID::CLIENT, id)->GetHP();
    p.exp = dynamic_cast<CClient*>(Find(OBJID::CLIENT, id))->GetEXP();
    p.max_exp = dynamic_cast<CClient*>(Find(OBJID::CLIENT, id))->GetMaxEXP();;

    Send_Packet(OBJID::CLIENT, id, &p);
}

void CNetMgr::Send_Attacked_Packet_Monster(const const int& id)
{
    sc_packet_attack p;
    p.id = id;
    p.size = sizeof(sc_packet_attack);
    p.type = SC_PACKET_ATTACK;

    p.hp = Find(OBJID::MONSTER, id)->GetHP();

    Send_Packet(OBJID::MONSTER, id, &p);
}

void CNetMgr::Send_Attacked_Packet_Client(const const int& id)
{
    sc_packet_attack p;
    p.id = id;
    p.size = sizeof(sc_packet_attack);
    p.type = SC_PACKET_ATTACK;

    p.hp = Find(OBJID::CLIENT, id)->GetHP();

    Send_Packet(OBJID::CLIENT, id, &p);
}

void CNetMgr::Send_LoginOK_Packet(const int& user_id)
{
    sc_packet_login_ok p;
    p.id = user_id;
    p.size = sizeof(sc_packet_login_ok);
    p.type = SC_PACKET_LOGIN_OK;

    p.exp =dynamic_cast<CClient*>(Find(OBJID::CLIENT, user_id))->GetEXP();
    p.hp =Find(OBJID::CLIENT, user_id)->GetHP();
    p.level = dynamic_cast<CClient*>(Find(OBJID::CLIENT, user_id))->GetLevel();
    p.x =Find(OBJID::CLIENT, user_id)->GetX();
    p.y =Find(OBJID::CLIENT, user_id)->GetY();

    p.iMax_exp = dynamic_cast<CClient*>(Find(OBJID::CLIENT, user_id))->GetMaxEXP();
    p.Attack_Damage = Find(OBJID::CLIENT, user_id)->GetAttackDamage();

    Send_Packet(OBJID::CLIENT, user_id, &p);
}

void CNetMgr::Send_Enter_Packet(const OBJID& user_eID, const int& user_id, const OBJID& other_eID, const int& other_id)
{
    sc_packet_enter p;
    p.id = other_id;
    p.size = sizeof(p);
    p.type = SC_PACKET_ENTER;
    p.x = Find(other_eID, other_id)->GetX();
    p.y = Find(other_eID, other_id)->GetY();

    strcpy_s(p.name, Find(other_eID, other_id)->GetName());    // data race???
    p.o_type = O_PLAYER;

    Send_Packet(user_eID, user_id, &p);
}

void CNetMgr::Send_Leave_Packet(const OBJID& user_eID, const int& user_id, const int& other_id)
{
    sc_packet_leave p;
    p.id = other_id;
    p.size = sizeof(p);
    p.type = SC_PACKET_LEAVE;

    Send_Packet(user_eID, user_id, &p);
}

void CNetMgr::Send_Move_Packet(const OBJID& user_eID, const int& user_id, const OBJID& mover_eID, const int& mover_id)
{
    sc_packet_move p;
    p.id = mover_id;
    p.size = sizeof(p);
    p.type = SC_PACKET_MOVE;
    p.x = Find(mover_eID, mover_id)->GetX();
    p.y = Find(mover_eID, mover_id)->GetY();

    p.move_time = Find(mover_eID, mover_id)->GetTime();

    Send_Packet(user_eID, user_id, &p);
}

void CNetMgr::Random_Move_NPC(const int& id)
{
    CGameObject* NPCObj = Find(NPC, id);

    int x = NPCObj->GetX();
    int y = NPCObj->GetY();
    

    // 체인지 섹터 함수 호출
    _tSector oldSector;
    oldSector = NPCObj->GetSector();

    switch (rand() % 4)
    {
    case 0: if (x > 0)x--; break;
    case 1: if (x < WORLD_WIDTH - 1)x++; break;
    case 2: if (y > 0)y--; break;
    case 3: if (y < WORLD_HEIGHT - 1)y++; break;
    }

    NPCObj->SetX(x);
    NPCObj->SetY(y);
    
    NPCObj->SetSector(NPCObj->GetX() / SECTOR_COL_Length, NPCObj->GetY() / SECTOR_ROW_Length);

    

    if (oldSector.x != NPCObj->GetSector().x || oldSector.y != NPCObj->GetSector().y)
    {
        g_Sector[NPC][NPCObj->GetSector().x][NPCObj->GetSector().y].emplace(id);

        g_Sector[NPC][oldSector.x][oldSector.y].erase(id);
    }

 
    for (auto& sec : g_Sector[CLIENT][NPCObj->GetSector().x][NPCObj->GetSector().y])
    {
        CGameObject* ClientObj = Find(CLIENT, sec);

        if (ClientObj->GetStatus() != ENUM_STATUS::ST_ACTIVE)continue;
        if (is_near(OBJID::CLIENT, sec, OBJID::NPC, id))
        {
            ClientObj->GetLock().lock();
            if (0 == dynamic_cast<CClient*>(ClientObj)->GetViewList(NPC).count(id))  // map으로 바꾸기
            {
                ClientObj->GetLock().unlock();
                Send_Enter_Packet(CLIENT, sec, NPC, id);
            }
            else
            {
                ClientObj->GetLock().unlock();
                Send_Move_Packet(CLIENT, sec, NPC, id);
            }
        }
        else
        {
            ClientObj->GetLock().lock();
            if (0 == dynamic_cast<CClient*>(ClientObj)->GetViewList(NPC).count(id))
            {
                ClientObj->GetLock().unlock();
            }
            else
            {
                ClientObj->GetLock().unlock();
                Send_Leave_Packet(OBJID::CLIENT, sec, id);
            }
        }
    }
}

void CNetMgr::Random_Move_Monster(const int& id)
{
    CGameObject* MonsterObj = Find(MONSTER, id);

    int x = MonsterObj->GetX();
    int y = MonsterObj->GetY();


    // 체인지 섹터 함수 호출
    _tSector oldSector;
    oldSector = MonsterObj->GetSector();

    switch (rand() % 4)
    {
    case 0: if (x > 0)x--; break;
    case 1: if (x < WORLD_WIDTH - 1)x++; break;
    case 2: if (y > 0)y--; break;
    case 3: if (y < WORLD_HEIGHT - 1)y++; break;
    }

    MonsterObj->SetX(x);
    MonsterObj->SetY(y);

    MonsterObj->SetSector(MonsterObj->GetX() / SECTOR_COL_Length, MonsterObj->GetY() / SECTOR_ROW_Length);



    if (oldSector.x != MonsterObj->GetSector().x || oldSector.y != MonsterObj->GetSector().y)
    {
        g_Sector[MONSTER][MonsterObj->GetSector().x][MonsterObj->GetSector().y].emplace(id);

        g_Sector[MONSTER][oldSector.x][oldSector.y].erase(id);
    }


    for (auto& sec : g_Sector[CLIENT][MonsterObj->GetSector().x][MonsterObj->GetSector().y])
    {
        CGameObject* ClientObj = Find(CLIENT, sec);

        if (ClientObj->GetStatus() != ENUM_STATUS::ST_ACTIVE)continue;
        if (is_near(OBJID::CLIENT, sec, OBJID::MONSTER, id))
        {
            ClientObj->GetLock().lock();
            if (0 == dynamic_cast<CClient*>(ClientObj)->GetViewList(MONSTER).count(id))  // map으로 바꾸기
            {
                ClientObj->GetLock().unlock();
                Send_Enter_Packet(CLIENT, sec, MONSTER, id);
            }
            else
            {
                ClientObj->GetLock().unlock();
                Send_Move_Packet(CLIENT, sec, MONSTER, id);
            }
        }
        else
        {
            ClientObj->GetLock().lock();
            if (0 == dynamic_cast<CClient*>(ClientObj)->GetViewList(MONSTER).count(id))
            {
                ClientObj->GetLock().unlock();
            }
            else
            {
                ClientObj->GetLock().unlock();
                Send_Leave_Packet(OBJID::CLIENT, sec, id);
            }
        }
    }
}

void CNetMgr::Do_Attack(const int& user_id)
{
    /// <summary>
    ///  추가구현
    /// </summary>
    /// <param name="user_id"></param>
    for (auto& obj : m_ObjLst[MONSTER])
    {
        
    }
}

void CNetMgr::Do_Move(const int& user_id, const int& dir)
{
    // 클라이언트의 서치 섹터 보기
    // 서치 섹터도 수정해야함 그냥 시발이건뭐 처음부터 코딩하는중
}
