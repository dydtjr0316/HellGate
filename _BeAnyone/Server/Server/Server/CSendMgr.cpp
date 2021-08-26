#include "stdafx.h"
#include "CSendMgr.h"
int sendentercnt = 1;

mutex sendLock;
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

    sendLock.lock();
    WSASend(Netmgr.GetMediatorMgr()->Find(id)->GetSocket(), &exover->wsabuf, 1, NULL, 0,
        &exover->over, NULL);
    sendLock.unlock();

}

void CSendMgr::Send_LevelUP_Packet(const uShort& id)
{
    sc_packet_level_up p;
    p.id = id;
    p.size = sizeof(sc_packet_level_up);
    p.type = SC_PACKET_LEVEL_UP;
    p.max_hp = CAST_CLIENT(Netmgr.GetMediatorMgr()->Find(id))->GetMaxHP();
    p.level = CAST_CLIENT(Netmgr.GetMediatorMgr()->Find(id))->GetLevel();
    p.attack_damage = CAST_CLIENT(Netmgr.GetMediatorMgr()->Find(id))->GetAttackDamage();
    p.hp = CAST_CLIENT(Netmgr.GetMediatorMgr()->Find(id))->GetHP();
    p.exp = CAST_CLIENT(Netmgr.GetMediatorMgr()->Find(id))->GetEXP();
    p.max_exp = CAST_CLIENT(Netmgr.GetMediatorMgr()->Find(id))->GetMaxEXP();;
    Send_Packet(id, &p);
}

void CSendMgr::Send_ItemCreate_Packet(const uShort& user_id, const Vector3& pos)
{
    sc_packet_ItemCreate_Packet p;
    p.size = sizeof(sc_packet_ItemCreate_Packet);
    p.type = SC_ITEMCREATE;
    p.vPos = pos;
    Send_Packet(user_id, &p);
}

void CSendMgr::Send_ItemDelete_Packet(const uShort& user_id, const Vector3& pos)
{
    sc_packet_ItemDelete_Packet p;
    p.size = sizeof(sc_packet_ItemDelete_Packet);
    p.type = SC_ITEMDELETE;
    p.vPos = pos;
    Send_Packet(user_id, &p);
}

void CSendMgr::Send_Attack_Effect(const uShort& user_id, const Vector3& effectPos)
{
    sc_packet_AttackEfefct_Packet p;
    p.size = sizeof(sc_packet_AttackEfefct_Packet);
    p.type = SC_ATTACKEFFECT;
    p.vPos = effectPos;
    Send_Packet(user_id, &p);
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
    p.hp = CAST_CLIENT(Netmgr.GetMediatorMgr()->Find(client_id))->GetHP();
    Send_Packet(client_id, &p);
}

void CSendMgr::Send_ID_Packet(const uShort& user_id)
{
    sc_packet_id p;
    CClient* pClient = CAST_CLIENT(Netmgr.GetMediatorMgr()->Find(user_id));
    p.id = user_id;
    p.size = sizeof(sc_packet_id);
    p.type = SC_PACKET_ID;

    Send_Packet(user_id, &p);
}

void CSendMgr::Send_LoginOK_Packet(const uShort& user_id)
{
    sc_packet_login_ok p;
    CClient* pClient = CAST_CLIENT(Netmgr.GetMediatorMgr()->Find(user_id));
    p.id = user_id;
    p.size = sizeof(sc_packet_login_ok);
    p.type = SC_PACKET_LOGIN_OK;
    p.exp = pClient->GetEXP();
    p.hp = pClient->GetHP();
    p.level = pClient->GetLevel();
    p.localVec = pClient->GetLocalPosVector();
    p.iMax_exp = pClient->GetMaxEXP();
    p.Attack_Damage = pClient->GetAttackDamage();


    // //cout << "Login 갯수 -> " << login <<endl;

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
    {
        p.hp = dynamic_cast<CMonster*>(Netmgr.GetMediatorMgr()->Find(other_id))->GetHP();

     /*   //cout << "********************" << endl;
        //cout << other_id << "가 " << user_id << "에게  Enter Packet 전송" << endl;
        //cout << "Monster POS ->   <" << Netmgr.GetMediatorMgr()->Find(other_id)->GetLocalPosVector().x << ", "
            << Netmgr.GetMediatorMgr()->Find(other_id)->GetLocalPosVector().z << ">" << endl;
        //cout << "********************" << endl << endl;*/
    }
    {
        cout << "********************" << endl;
        cout << "********************" << endl;
        cout << other_id << "가 " << user_id << "에게 Enter Packet 전송" << endl;
        cout << "********************" << endl;
        cout << "********************" << endl;
    }
    Send_Packet(user_id, &p);
}

void CSendMgr::Send_Leave_Packet(const uShort& user_id, const uShort& other_id, const bool& isAttack)
{
    if (Netmgr.GetMediatorMgr()->Count(other_id) == 0)return;
    //g_QuadTree.Delete(Netmgr.GetMediatorMgr()->Find(other_id));

    sc_packet_leave p;
    p.id = other_id;
    p.size = sizeof(p);
    p.type = SC_PACKET_LEAVE;
    p.isAttack = isAttack;
    if (p.id >= START_MONSTER && p.id < END_MONSTER)
    {
        
        cout << other_id << "가 " << user_id << "에게 Leave Packet 전송" << endl;
        cout << "********************" << endl;
        cout << "********************" << endl;
        Netmgr.GetMediatorMgr()->Find(other_id)->SetIsMoving(false);

    }
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
   // p.move_time = Netmgr.GetMediatorMgr()->Find(mover_id)->GetClientTime();
    p.rotateY = Netmgr.GetMediatorMgr()->Find(mover_id)->GetRotateY();
    p.speed = Netmgr.GetMediatorMgr()->Find(mover_id)->GetSpeed();
    p.Start = system_clock::now();
    p.isMoving = Netmgr.GetMediatorMgr()->Find(mover_id)->GetIsMoving();
   // cout << "서버에서 보낼타이밍 \t" << p.localVec.x << " , " << p.localVec.z << endl;

    Send_Packet(user_id, &p);
}

void CSendMgr::Send_Attack_Animation_Packet(const uShort& user_id, const uShort& attckerid, const bool& isAttack, const Ani_TYPE& type)
{
    sc_packet_AttackAni p;
    p.id = attckerid;
    p.size = sizeof(p);
    p.isAttack = isAttack;
    p.anitype = (char)type;
    p.type = SC_PACKET_ATTACKANI;
    Send_Packet(user_id, &p);
}

void CSendMgr::Send_Monster_Animation_Packet(const uShort& monster_id, const uShort& user_id, const MONSTER_ANI_TYPE& aniType, const uShort& id)
{
    sc_packet_Monster_Animation p;
    p.id = monster_id;
    p.size = sizeof(p);
    p.type = SC_PACKET_MONSTER_ANIMATION;
    p.aniType = aniType;
    p.otherid = user_id;
    Send_Packet(user_id, &p);

}

void CSendMgr::Send_Monster_Move_Packet(const uShort& user_id, const uShort& mover_id, const char& dir)
{
    sc_packet_monster_automove p;
    p.size = sizeof(p);
    p.type = SC_PACKET_MONSTER_MOVE;
    p.id = mover_id;
    p.eDir = dir;
    p.pos = Netmgr.GetMediatorMgr()->Find(p.id)->GetLocalPosVector();

    if (p.id == 1000)
    {
        //cout << "-----------------------------------------------------------" << endl;
        //cout << "-----------------------------------------------------------" << endl;
         //cout << "id : " << mover_id << "  Dir :  " << (int)p.eDir << endl;
        //cout << p.pos.x << ", " << p.pos.z << endl;
        //cout << "-----------------------------------------------------------" << endl;
        //cout << "-----------------------------------------------------------" << endl;
    }
    Send_Packet(user_id, &p);
}


void CSendMgr::Send_Stop_Packet(const uShort& user_id, const uShort& mover_id, const bool& isMoving)
{
    sc_packet_stop p;
    p.size = sizeof(p);
    p.type = SC_PACKET_STOP;
    p.id = mover_id;
    cout << p.id << "의 스톱패킷을 "<<user_id<<"로 보냄" << endl;
    cout << "*-*-*-*-*-*-*-*-*-*-*-*-*-*" << endl;
    p.isMoving = isMoving;
    Netmgr.GetMediatorMgr()->Find(mover_id)->SetIsMoving(false);

    Send_Packet(user_id, &p);
}

void CSendMgr::Send_Rotate_Packet(const uShort& moverid, const uShort& userid, const Vector3& rotate)
{
    sc_packet_rotate p;
    p.size = sizeof(p);
    p.type = SC_ROTATE;
    p.id = moverid;
    p.rotate = rotate;
    Send_Packet(userid, &p);
}
