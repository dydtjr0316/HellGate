#include "stdafx.h"
#include "CNetMgr.h"

void CNetMgr::Send_Packet(const int& id, void* packet)
{
    unsigned char* buf = reinterpret_cast<unsigned char*>(packet);

    EXOVER* exover = new EXOVER;
    ZeroMemory(&exover->io_buf, sizeof(exover->io_buf));
    memcpy(exover->io_buf, buf, buf[0]);

    exover->op = ENUMOP::OP_SEND;
    exover->wsabuf.buf = reinterpret_cast<char*>(exover->io_buf);
    exover->wsabuf.len = buf[0];
    ZeroMemory(&exover->over, sizeof(exover->over));

    g_clients[id].GetClientLock().lock();

    WSASend(g_clients[id].GetSocket(), &exover->wsabuf, 1, NULL, 0,
        &exover->over, NULL);

    g_clients[id].GetClientLock().unlock();
}

void CNetMgr::Send_Chat_Packet(const int& toClient, const int& id, const char* mess, const size_t& size)
{
    sc_packet_chat p;
    p.id = id;
    p.size = sizeof(sc_packet_chat) * 2 + 2;
    p.type = SC_PACKET_CHAT;
    strcpy_s(p.message, mess);

    Send_Packet(toClient, &p);
}

void CNetMgr::Send_LevelUP_Packet(const int& id)
{
    sc_packet_level_up p;
    p.id = id;
    p.size = sizeof(sc_packet_level_up);
    p.type = SC_PACKET_LEVEL_UP;

    p.hp = g_clients[id].GetHP();
    p.max_hp = g_clients[id].GetMaxHP();
    
    p.attack_damage = g_clients[id].GetAttackDamage();
    
    p.level = g_clients[id].GetLevel();

    p.exp = g_clients[id].GetEXP();
    p.max_exp = g_clients[id].GetMaxEXP();

    Send_Packet(id, &p);
}

void CNetMgr::Send_Attack_Packet(const int& id)
{
    sc_packet_attack p;
    p.id = id;
    p.size = sizeof(sc_packet_attack);
    p.type = SC_PACKET_ATTACK;

    p.hp = g_clients[id].GetHP();

    Send_Packet(id, &p);
}

void CNetMgr::Send_LoginOK_Packet(const int& id)
{
    sc_packet_login_ok p;
    p.id = id;
    p.size = sizeof(sc_packet_login_ok);
    p.type = SC_PACKET_LOGIN_OK;

    p.exp = g_clients[id].GetEXP();
    p.hp = g_clients[id].GetHP();
    p.level = g_clients[id].GetLevel();

    p.x = g_clients[id].GetX();
    p.y = g_clients[id].GetY();
    p.z = g_clients[id].GetZ();

    p.iMax_exp = g_clients[id].GetMaxEXP();
    p.Attack_Damage = g_clients[id].GetAttackDamage();

    Send_Packet(id, &p);
}

void CNetMgr::Send_Enter_Packet(const int& user_id, const int& other_id)
{
    sc_packet_enter p;
    p.id = other_id;
    p.size = sizeof(p);
    p.type = SC_PACKET_ENTER;
    p.x = g_clients[other_id].GetX();
    p.y = g_clients[other_id].GetY();

    strcpy_s(p.name, g_clients[other_id].GetName());   // data race???
    p.o_type = O_PLAYER;

    Send_Packet(user_id, &p);
}

void CNetMgr::Send_Leave_Packet(const int& user_id, const int& other_id)
{
    sc_packet_leave p;
    p.id = other_id;
    p.size = sizeof(p);
    p.type = SC_PACKET_LEAVE;

    Send_Packet(user_id, &p);
}

void CNetMgr::Send_Move_Packet(const int& user_id, const int& mover_id)
{
    sc_packet_move p;
    p.id = mover_id;
    p.size = sizeof(p);
    p.type = SC_PACKET_MOVE;
    p.x = g_clients[mover_id].GetX();
    p.y = g_clients[mover_id].GetY();

    p.move_time = g_clients[mover_id].GetClientTime();

    Send_Packet(user_id, &p);
}

void CNetMgr::Random_Move_NPC(const int& npc_id)
{
    unordered_set <int> old_viewList;
    for (int i = 0; i < MAX_USER; ++i) {
        if (ENUM_STATUS::ST_ACTIVE != g_clients[i].GetStatus()) continue;
        if (true == is_near(npc_id, i)) old_viewList.insert(i);
    }

    int x = g_npcs[npc_id].GetX();
    int z = g_npcs[npc_id].GetZ();

    switch (rand() % 4)
    {
    case 0: if (x > 0)x--; break;
    case 1: if (x < WORLD_WIDTH - 1)x++; break;
    case 2: if (z > 0)z--; break;
    case 3: if (z < WORLD_HEIGHT - 1)z++; break;
    }

    g_npcs[npc_id].SetX(x);
    g_npcs[npc_id].SetZ(z);

    unordered_set<int> new_viewList;
    for (int i = 0; i < MAX_USER; ++i)
    {
        if (npc_id == i) continue;
        if (g_clients[i].GetStatus() !=ENUM_STATUS::ST_ACTIVE) continue;

        if (true == is_near(i, npc_id))
        {
            new_viewList.insert(i);
            g_clients[i].GetClientLock().lock();
            if (0 != g_clients[i].GetViewList().count(npc_id))
            {
                g_clients[i].GetClientLock().unlock();
                Send_Move_Packet(i, npc_id);
            }
            else {
                g_clients[i].GetClientLock().unlock();
                Send_Enter_Packet(i, npc_id);
            }
        }
        else {
            g_clients[i].GetClientLock().lock();
            if (0 != g_clients[i].GetViewList().count(npc_id))
            {
                g_clients[i].GetClientLock().unlock();
                Send_Leave_Packet(i, npc_id);
            }
            else g_clients[i].GetClientLock().unlock();
        }

    }

}

bool is_near(int i, int ii) {};
