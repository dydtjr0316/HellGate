#include "stdafx.h"
#include "CSendMgr.h"

void CSendMgr::Send_Packet(const uShort& id, void* packet)
{
    unsigned char* buf = reinterpret_cast<unsigned char*>(packet);

    EXOVER* exover = new EXOVER;
    ZeroMemory(&exover->io_buf, sizeof(exover->io_buf));
    memcpy(exover->io_buf, buf, buf[0]);
    exover->op = ENUMOP::OP_SEND;
    exover->wsabuf.buf = reinterpret_cast<char*>(exover->io_buf);
    exover->wsabuf.len = buf[0];
    ZeroMemory(&exover->over, sizeof(exover->over));

    Netmgr.GetMediatorMgr()->Find(id)->GetLock().lock();
    WSASend(Netmgr.GetMediatorMgr()->Find(id)->GetSocket(), &exover->wsabuf, 1, NULL, 0,
        &exover->over, NULL);
    Netmgr.GetMediatorMgr()->Find(id)->GetLock().unlock();

}

void CSendMgr::Send_LevelUP_Packet(const uShort& id)
{
    sc_packet_level_up p;
    p.id = id;
    p.size = sizeof(sc_packet_level_up);
    p.type = SC_PACKET_LEVEL_UP;
    p.max_hp = Netmgr.Cast_Client(Netmgr.GetMediatorMgr()->Find(id))->GetMaxHP();
    p.level = Netmgr.Cast_Client(Netmgr.GetMediatorMgr()->Find(id))->GetLevel();
    p.attack_damage = Netmgr.Cast_Client(Netmgr.GetMediatorMgr()->Find(id))->GetAttackDamage();
    p.hp = Netmgr.Cast_Client(Netmgr.GetMediatorMgr()->Find(id))->GetHP();
    p.exp = Netmgr.Cast_Client(Netmgr.GetMediatorMgr()->Find(id))->GetEXP();
    p.max_exp = Netmgr.Cast_Client(Netmgr.GetMediatorMgr()->Find(id))->GetMaxEXP();;
    Send_Packet(id, &p);
}

void CSendMgr::Send_Attacked_Packet_Monster(const uShort& attacker, const uShort& monster_id)
{
    sc_packet_attack p;
    p.id = monster_id;
    p.size = sizeof(sc_packet_attack);
    p.type = SC_PACKET_ATTACK;
    p.hp = dynamic_cast<CMonster*>(Netmgr.GetMediatorMgr()->Find(monster_id))->GetHP();
    Send_Packet(attacker, &p);
}

void CSendMgr::Send_Attacked_Packet_Client(const uShort& client_id)
{
    sc_packet_attack p;
    p.id = client_id;
    p.size = sizeof(sc_packet_attack);
    p.type = SC_PACKET_ATTACK;
    p.hp = Netmgr.Cast_Client(Netmgr.GetMediatorMgr()->Find(client_id))->GetHP();
    Send_Packet(client_id, &p);
}

void CSendMgr::Send_ID_Packet(const uShort& user_id)
{
    sc_packet_id p;
    CClient* pClient = Netmgr.Cast_Client(Netmgr.GetMediatorMgr()->Find(user_id));
    p.id = user_id;
    p.size = sizeof(sc_packet_id);
    p.type = SC_PACKET_ID;

    Send_Packet(user_id, &p);
}

void CSendMgr::Send_LoginOK_Packet(const uShort& user_id)
{
    sc_packet_login_ok p;
    CClient* pClient = Netmgr.Cast_Client(Netmgr.GetMediatorMgr()->Find(user_id));
    p.id = user_id;
    p.size = sizeof(sc_packet_login_ok);
    p.type = SC_PACKET_LOGIN_OK;
    p.exp = pClient->GetEXP();
    p.hp = pClient->GetHP();
    p.level = pClient->GetLevel();
    p.localVec = pClient->GetLocalPosVector();
    p.iMax_exp = pClient->GetMaxEXP();
    p.Attack_Damage = pClient->GetAttackDamage();


    // cout << "Login °¹¼ö -> " << login <<endl;

    Send_Packet(user_id, &p);
}

void CSendMgr::Send_Enter_Packet(const uShort& user_id, const uShort& other_id)
{
    sc_packet_enter p;
    p.id = other_id;
    p.size = sizeof(p);
    p.type = SC_PACKET_ENTER;
    p.localVec = Netmgr.GetMediatorMgr()->Find(other_id)->GetLocalPosVector();
    p.RotateY = Netmgr.GetMediatorMgr()->Find(other_id)->GetRoatateVector().y;
    strcpy_s(p.name, Netmgr.GetMediatorMgr()->Find(other_id)->GetName());    // data race???
    p.o_type = O_PLAYER;
    if (p.id >= START_MONSTER && p.id < END_MONSTER)
        p.hp = dynamic_cast<CMonster*>(Netmgr.GetMediatorMgr()->Find(other_id))->GetHP();

    Send_Packet(user_id, &p);
}

void CSendMgr::Send_Leave_Packet(const uShort& user_id, const uShort& other_id, const bool& isAttack)
{
    if (Netmgr.GetMediatorMgr()->Count(other_id) == 0)return;
    sc_packet_leave p;
    p.id = other_id;
    p.size = sizeof(p);
    p.type = SC_PACKET_LEAVE;
    p.isAttack = isAttack;


    Send_Packet(user_id, &p);
}

void CSendMgr::Send_Move_Packet(const uShort& user_id, const uShort& mover_id, const char& dir)
{
    sc_packet_move p;
    p.id = mover_id;
    p.size = sizeof(p);
    p.type = SC_PACKET_MOVE;
    p.localVec = Netmgr.GetMediatorMgr()->Find(mover_id)->GetLocalPosVector();
    p.dirVec = Netmgr.GetMediatorMgr()->Find(mover_id)->GetDirVector();
    p.dir = dir;
    p.move_time = Netmgr.GetMediatorMgr()->Find(mover_id)->GetClientTime();
    p.rotateY = Netmgr.GetMediatorMgr()->Find(mover_id)->GetRotateY();
    p.speed = Netmgr.GetMediatorMgr()->Find(mover_id)->GetSpeed();
    p.Start = Netmgr.GetMediatorMgr()->Find(mover_id)->GetHalfRTT();
    p.isMoving = Netmgr.GetMediatorMgr()->Find(mover_id)->GetIsMoving();

    Send_Packet(user_id, &p);
}

void CSendMgr::Send_Attack_Animation_Packet(const uShort& user_id, const uShort& attckerid, const bool& isAttack)
{
    sc_packet_AttackAni p;
    p.id = attckerid;
    p.size = sizeof(p);
    p.isAttack = isAttack;
    p.type = SC_PACKET_ATTACKANI;
    Send_Packet(user_id, &p);
}


void CSendMgr::Send_Stop_Packet(const uShort& user_id, const uShort& mover_id, const bool& isMoving)
{
    sc_packet_stop p;
    p.size = sizeof(p);
    p.type = SC_PACKET_STOP;
    p.id = mover_id;
    p.isMoving = isMoving;
    Send_Packet(user_id, &p);
}