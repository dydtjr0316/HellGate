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
    // ����
    //_tSector old_sector(g_npcs[npc_id].GetX() / SECTOR_ROW_Length, g_npcs[npc_id].GetZ() / SECTOR_COL_Length);
    


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

    //_tSector new_sector(g_npcs[npc_id].GetX() / SECTOR_ROW_Length, g_npcs[npc_id].GetZ() / SECTOR_COL_Length);
}

//void CNetMgr::Disconnect(const OBJID& eID, const int& id)
//{
//    
//    g_clients[user_id].m_status = ST_ALLOC;
//
//    send_leave_packet(user_id, user_id);
//
//    g_clients[user_id].m_cl.lock();
//    for (int i = 0; i < MAX_USER; ++i)
//    {
//
//        if (g_clients[i].m_status == ST_ACTIVE)
//        {
//            if (i != user_id)
//            {
//                // �þ�ó�� �뵵
//                if (0 != g_clients[i].view_list.count(user_id))
//                {
//                    g_clients[i].view_list.erase(user_id);
//                    send_leave_packet(i, user_id);
//                }
//            }
//        }
//
//    }
//
//    g_clients[user_id].m_status = ST_FREE;
//    g_clients[user_id].view_list.clear();
//
//    g_clients[user_id].m_cl.unlock();
//
//    closesocket(g_clients[user_id].m_s);
//    g_clients[user_id].m_s = 0;
//
//    g_DataBase.UpSert_DB(g_clients[user_id]);
//}

//void CNetMgr::Do_Move(const int& user_id, const int& dir)
//{
//    vector<unordered_set<int>> vSectors;
//
//    int x1 = (g_clients[user_id].GetX()- VIEW_LIMIT) / SECTOR_COL_Length;
//    int z1 = (g_clients[user_id].GetZ() - VIEW_LIMIT) / SECTOR_ROW_Length;
//
//    int x2 = (g_clients[user_id].GetX() + VIEW_LIMIT) / SECTOR_COL_Length;
//    int z2 = (g_clients[user_id].GetZ() + VIEW_LIMIT) / SECTOR_ROW_Length;
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
//    g_clients[user_id].Change_Sector();
//
//    Send_Move_Packet(user_id, user_id);
//
//    //�þ� ó�� ������
//    unordered_set<int> new_viewList;
//
//    for (auto& vSec : vSectors)
//    {
//        if (vSec.size() != 0)
//        {
//            for (auto& i : vSec)
//            {
//                if (user_id == i) continue;
//                if (g_clients[i].Is_SameStatus(ENUM_STATUS::ST_SLEEP))
//                {
//                    // �̺κ� �� �س������� ����
//                    wake_up_npc(g_clients[i].m_id);
//                    wake_up_monster(g_clients[i].m_id);
//                }
//                if (g_clients[i].m_status != ST_ACTIVE) continue;
//
//                if (true == is_near(user_id, i))
//                {
//                    new_viewList.insert(i);
//                }
//            }
//        }
//    }
//
//    for (int i = MAX_USER; i < MAX_USER + NUM_NPC; ++i)
//    {
//        if (true == is_near(user_id, i))
//        {
//            new_viewList.insert(i);
//            wake_up_npc(i);
//        }
//    }
//    for (int i = MAX_USER + NUM_NPC; i < MAX_USER + NUM_NPC + DIVIDE_MONNSTER * 4; ++i)
//    {
//        if (true == is_near(user_id, i))
//        {
//            new_viewList.insert(i);
//            wake_up_monster(i);
//        }
//    }
//
//    //�þ߿� ���� ��ü ó��
//    for (int ob : new_viewList)
//    {
//        //�þ߿� ���� ���� ��ü ����
//        if (0 == old_viewList.count(ob))
//        {
//            g_clients[user_id].view_list.insert(ob);
//            send_enter_packet(user_id, ob);
//
//            if (false == is_npc(ob)) // npc��� �˻���ص��Ǵ� �κ�
//            {
//                //���� viewlist�� ���� ������
//                if (0 == g_clients[ob].view_list.count(user_id))
//                {
//                    //���濡 view_list���� ���� �־�� ��.
//                    g_clients[ob].view_list.insert(user_id);
//                    send_enter_packet(ob, user_id);
//                }
//                else
//                {
//                    // �ִٸ� �̵� ��Ǹ� ������
//                    send_move_packet(ob, user_id);
//                }
//            }
//        }
//        //�������� �þ߿� �־��� �̵� �Ŀ��� �þ߿� �ִ� ��ü
//        else
//        {
//            //// ������ ���� �ʿ�� ���� (���� �̵��� ���̱� ������)
//            // ���� �̵��� �� ���̱� ������ �ٸ� ������Դ� �˷��� �Ѵ�.
//            // ��, ��Ƽ������� ������ ���̱� ������ ������ �� ���̿� �þ߿��� ��������� ���� ����.
//
//
//            if (false == is_npc(ob)) // npc��� �˻���ص��Ǵ� �κ�
//            {
//                if (0 != g_clients[ob].view_list.count(user_id))
//                {
//                    send_move_packet(ob, user_id);
//                }
//                else
//                {
//                    g_clients[ob].view_list.insert(user_id);
//                    send_enter_packet(ob, user_id);
//                }
//            }
//        }
//    }
//    //������ �־��µ� ������ ���� ��ü ó��
//    for (int ob : old_viewList)
//    {
//
//        if (0 == new_viewList.count(ob))
//        {
//            g_clients[user_id].view_list.erase(ob);
//            send_leave_packet(user_id, ob);
//
//            if (false == is_npc(ob)) // npc��� �˻���ص��Ǵ� �κ�
//            {
//                if (0 != g_clients[ob].view_list.count(user_id))
//                {
//                    g_clients[ob].view_list.erase(user_id);
//                    send_leave_packet(ob, user_id);
//                }
//            }
//        }
//
//    }
//}

bool CheckNearObject_Default(const CGameObject* client, const int client_id, const CGameObject* other, const int other_id) {};
bool CheckNearObject_NPC(const CGameObject* client, const int client_id, const CNPC* npc, const int npc_id) {};
