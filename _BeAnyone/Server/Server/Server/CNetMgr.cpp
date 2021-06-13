#include "stdafx.h"
#include "CNetMgr.h"
#include "CSectorMgr.h"
int cnt = 0;
int login = 0;
int ghost = 0;
CMonster* CNetMgr::Cast_Monster(CGameObject* obj)
{
    if (obj != nullptr)return dynamic_cast<CMonster*>(obj);
    else return nullptr;
}
void CNetMgr::error_display(const char* msg, int err_no)     // ���� ���
{
    WCHAR* lpMsgBuf;
    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_FROM_SYSTEM,
        NULL, err_no,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR)&lpMsgBuf, 0, NULL);
    std::cout << msg;
    std::wcout << L"���� " << lpMsgBuf << std::endl;
    while (true);
    LocalFree(lpMsgBuf);
}


bool CNetMgr::is_near( const uShort& p1, const uShort& p2)
{
    if(p1 == p2)return false;
    float dist =
        (m_pMediator->Find(p1)->GetLocalPosVector().x - m_pMediator->Find(p2)->GetLocalPosVector().x)
        * (m_pMediator->Find(p1)->GetLocalPosVector().x - m_pMediator->Find(p2)->GetLocalPosVector().x);

    dist += (m_pMediator->Find(p1)->GetLocalPosVector().z - m_pMediator->Find(p2)->GetLocalPosVector().z)
        * (m_pMediator->Find(p1)->GetLocalPosVector().z - m_pMediator->Find(p2)->GetLocalPosVector().z);

  

    return dist <= (float)(VIEW_LIMIT * VIEW_LIMIT);
}


//void CNetMgr::Random_Move_NPC(const int& id)
//{
//    CGameObject* NPCObj = m_pMediator->Find( id);
//    float x = NPCObj->GetX();
//    float y = NPCObj->GetY();
//    
//
//    // ü���� ���� �Լ� ȣ��
//    _tSector oldSector;
//    oldSector = NPCObj->GetSector();
//
//    switch (rand() % 4)
//    {
//    case 0: if (x > 0)x--; break;
//    case 1: if (x < WORLD_WIDTH - 1)x++; break;
//    case 2: if (y > 0)y--; break;
//    case 3: if (y < WORLD_HEIGHT - 1)y++; break;
//    }
//
//    NPCObj->SetX(x);
//    NPCObj->SetY(y);
//    
//    NPCObj->Change_Sector(oldSector);
//
//    for (auto& vSec : NPCObj->Search_Sector()) {
//        for (auto& clientID : vSec)
//        {
//            if (!IsClient(clientID))continue;
//
//            CGameObject* ClientObj = m_pMediator->Find(clientID);
//            if (ClientObj->GetStatus() != OBJSTATUS::ST_ACTIVE)continue;
//
//            if (is_near(clientID, id))
//            {
//                ClientObj->GetLock().lock();
//                if (0 == Cast_Client(ClientObj)->GetViewList().count(id))  // map���� �ٲٱ�
//                {
//                    //cout << "�丮��Ʈ�� ���� ����" << endl;
//                    ClientObj->GetLock().unlock();
//                    Cast_Client(ClientObj)->GetViewList().insert(id);
//                    Send_Enter_Packet(clientID, id);
//                }
//                else
//                {
//                    //cout << "�丮��Ʈ�� npc �־�" << endl;
//                    ClientObj->GetLock().unlock();
//                    Send_Move_Packet(clientID, id);
//                }
//            }
//            else
//            {
//                ClientObj->GetLock().lock();
//                if (0 == Cast_Client(ClientObj)->GetViewList().count(id))
//                {
//                    ClientObj->GetLock().unlock();
//                }
//                else
//                {
//                    ClientObj->GetLock().unlock();
//                    Cast_Client(ClientObj)->GetViewList().erase(id);
//                    Send_Leave_Packet(clientID, id);
//                }
//
//            }
//        }
//    }
//}

//void CNetMgr::Random_Move_Monster(const int& id)
//{
//    CGameObject* MonsterObj = m_pMediator->Find( id);
//    Vector3 monsterPos = MonsterObj->GetLocalPosVector();
//
//    _tSector oldSector = MonsterObj->GetSector();
//
//    switch (rand() % 4)
//    {
//    case 0: if (monsterPos.x > 0)monsterPos.x--; break;
//    case 1: if (monsterPos.x < WORLD_WIDTH - 1)monsterPos.x++; break;
//    case 2: if (monsterPos.z > 0)monsterPos.z--; break;
//    case 3: if (monsterPos.z < WORLD_HEIGHT - 1)monsterPos.z++; break;
//    }
//
//    MonsterObj->SetPosX(monsterPos.x);
//    MonsterObj->SetPosZ(monsterPos.z);
//
//    MonsterObj->Change_Sector(oldSector);
//
//    for (auto& vSec : MonsterObj->Search_Sector()) {
//        for (auto& sec : vSec)
//        {
//            if (!IsClient(sec))continue;
//
//            CClient* pClient = Cast_Client(m_pMediator->Find(sec));
//            if (pClient->GetStatus() != OBJSTATUS::ST_ACTIVE)continue;
//
//            if (is_near(sec, id))    // is near�� �ʿ��Ѱ�?
//            {
//                pClient->GetLock().lock();
//                if (0 == pClient->GetViewList().count(id))  // �丮��Ʈ�� ���Ͱ� ���ٸ� ���� ��Ŷ
//                {
//                    pClient->GetLock().unlock();
//                    pClient->GetViewList().insert(id);
//                    Send_Enter_Packet(sec, id);
//                }
//                else                                        // �丮��Ʈ�� ���Ͱ� ������ ���� ��Ŷ
//                {
//                    pClient->GetLock().unlock();
//                    Send_Move_Packet(sec, id);
//                }
//            }
//            else
//            {
//                pClient->GetLock().lock();
//                if (0 == pClient->GetViewList().count(id))
//                {
//                    pClient->GetLock().unlock();
//                    
//                }
//                else
//                {
//                    pClient->GetLock().unlock();
//                    pClient->GetViewList().erase(id);
//                    Send_Leave_Packet(sec, id);
//                }
//            }
//        }
//    }
//}

void CNetMgr::Do_Attack(const uShort& attacker, const uShort& victim)
{
    CMonster* monster = dynamic_cast<CMonster*>(m_pMediator->Find(victim));
    unordered_set<uShort> new_viewList;
    vector<unordered_set<uShort>> vSectors = CSectorMgr::GetInst()->Search_Sector(m_pMediator->Find(victim));
    for (auto& vSec : vSectors)
    {
        if (vSec.size() != 0)
        {
            for (auto& user : vSec)
            {
                if (IsClient(user) && is_near(victim, user))
                {
                    new_viewList.insert(user);
                }
            }
        }
    }

    if (monster->GetHP() - 12 >= 0)
    {
        monster->SetHP(monster->GetHP() - 12);
        for (auto& clientID : new_viewList)
        {
                m_pSendMgr->Send_Attacked_Packet_Monster(clientID, victim);
        }
    }
    else
    {
            monster->SetHP(0.f);
        for (auto& clientID : new_viewList)
        {
            m_pSendMgr->Send_Attacked_Packet_Monster(clientID, victim);
            m_pSendMgr->Send_Leave_Packet(clientID, victim, true);
        }   
    }
}
void CNetMgr::Kill_Monster(const uShort& monster_id)
{
    CMonster* monster = dynamic_cast<CMonster*>(m_pMediator->Find(monster_id));
    unordered_set<uShort> new_viewList;
    vector<unordered_set<uShort>> vSectors = CSectorMgr::GetInst()->Search_Sector(m_pMediator->Find(monster_id));

    //for (auto& vSec : vSectors)
    //{
    //    if (vSec.size() != 0)
    //    {
    //        for (auto& user : vSec)
    //        {
    //            if (IsClient(user) && is_near(monster_id, user))
    //            {
    //                new_viewList.insert(user);
    //            }
    //        }
    //    }
    //}
    g_Sector[m_pMediator->Find(monster_id)->GetSector().x][m_pMediator->Find(monster_id)->GetSector().z].erase(monster_id);
    m_pMediator->Delete_Obj(monster_id);

}
void CNetMgr::Do_Move(const uShort& user_id, const char& dir, Vector3& localVec, const float& rotateY)
{
    CClient* pClient = Cast_Client(m_pMediator->Find(user_id));

    unordered_set<int> old_viewList = pClient->GetViewList();

    _tSector oldSector = pClient->GetSector();

    /*switch (dir)
    {
    case MV_UP:
        if (localVec.y > 0) localVec += dirVec;
        break;
    case MV_DOWN:
        if (localVec.y < (WORLD_HEIGHT - 1)) localVec += dirVec;
        break;
    case MV_LEFT:
        if (localVec.x > 0) localVec += dirVec;
        break;
    case MV_RIGHT:
        if (localVec.x < (WORLD_WIDTH - 1)) localVec += dirVec;
        break;
    case MV_FRONT:
        if (localVec.z < (WORLD_WIDTH - 1)) localVec += dirVec;
        break;
    case MV_BACK:
        if (localVec.z > 0) localVec += dirVec;
        break;

    default:
        cout << "Unknown Direction from Client move packet!\n";
        DebugBreak();
        exit(-1);
    }*/

    pClient->SetPosV(localVec);
    pClient->SetRotateY(rotateY);
    
    pClient->Change_Sector(oldSector);
    unordered_set<uShort> new_viewList;

    //m_pSendMgr->Send_Move_Packet(user_id, user_id, dir);
    vector<unordered_set<uShort>> vSectors = CSectorMgr::GetInst()->Search_Sector(m_pMediator->Find(user_id));

    for (auto& vSec : vSectors)
    {
        if (vSec.size() != 0)
        {
            for (auto& user : vSec)
            {
                if (is_near(user_id, user))
                {
                    if (!IsClient(user))
                    {
                        if (m_pMediator->Find(user)->GetStatus() == OBJSTATUS::ST_SLEEP)
                        {
                            if (IsMonster(user))
                                WakeUp_Monster(user);
                            else
                            {
                                WakeUp_NPC(user);
                            }
                        }
                    }
                    else
                    {

                    }
                    new_viewList.insert(user);
                }
            }
        }
    }

    for (auto& ob : new_viewList)
    {
        //�þ߿� ���� ���� ��ü ����

        if (0 == old_viewList.count(ob)) // ���� ���� ���̵�
        {
            pClient->GetViewList().insert(ob);
            m_pSendMgr->Send_Enter_Packet(user_id, ob);
            if (IsClient(ob) && ob != user_id)
            {
                if (Cast_Client(m_pMediator->Find(ob))->GetViewList().count(user_id) == 0)
                {
                    Cast_Client(m_pMediator->Find(ob))->GetViewList().insert(user_id);
                    m_pSendMgr->Send_Enter_Packet(ob, user_id);
                }
                else
                {
                    m_pSendMgr->Send_Move_Packet(ob, user_id, dir);  // ���⼭ �� ����
                }
            }
        }
        else // �������� �ִ� ���̵� 
        {
            if (IsClient(ob) && ob != user_id)
            {
                if (Cast_Client(m_pMediator->Find(ob))->GetViewList().count(user_id) == 0)
                {
                    Cast_Client(m_pMediator->Find(ob))->GetViewList().insert(user_id);
                    m_pSendMgr->Send_Enter_Packet(ob, user_id);
                }
                else
                {
                    m_pSendMgr->Send_Move_Packet(ob, user_id, dir);
                }
            }
        }
    }
    for (auto& ob : old_viewList)
    {
        if (new_viewList.count(ob) == 0)
        {
            pClient->GetViewList().erase(ob);
            m_pSendMgr->Send_Leave_Packet(user_id, ob);

            if (IsClient(ob))
            {
                if (Cast_Client(m_pMediator->Find(ob))->GetViewList().count(user_id) != 0)
                {
                    Cast_Client(m_pMediator->Find(ob))->GetViewList().erase(user_id);
                    m_pSendMgr->Send_Leave_Packet(ob, user_id);
                }
            }
        }
    }
}

void CNetMgr::Do_Stop(const uShort& user_id, const bool& isMoving)
{
    CClient* pClient = Cast_Client(m_pMediator->Find(user_id));

    unordered_set<int> old_viewList = pClient->GetViewList();

    _tSector oldSector = pClient->GetSector();
    pClient->SetIsMoving(isMoving);
    pClient->Change_Sector(oldSector);
    unordered_set<int> new_viewList;

    vector<unordered_set<uShort>> vSectors = CSectorMgr::GetInst()->Search_Sector(m_pMediator->Find(user_id));


    for (auto& vSec : vSectors)
    {
        if (vSec.size() != 0)
        {
            for (auto& user : vSec)
            {
                if (is_near(user_id, user))
                {
                    if (!IsClient(user))
                    {
                        if (m_pMediator->Find(user)->GetStatus() == OBJSTATUS::ST_SLEEP)
                        {
                            if (IsMonster(user))
                                WakeUp_Monster(user);
                            else
                            {
                                WakeUp_NPC(user);
                            }
                        }
                    }
                    else
                    {

                    }
                    new_viewList.insert(user);
                }
            }
        }
    }

    for (auto& ob : new_viewList)
    {
        //�þ߿� ���� ���� ��ü ����

        if (0 == old_viewList.count(ob)) // ���� ���� ���̵�
        {
            pClient->GetViewList().insert(ob);
            m_pSendMgr->Send_Enter_Packet(user_id, ob);
            if (IsClient(ob) && ob != user_id)
            {
                if (Cast_Client(m_pMediator->Find(ob))->GetViewList().count(user_id) == 0)
                {
                    Cast_Client(m_pMediator->Find(ob))->GetViewList().insert(user_id);
                    m_pSendMgr->Send_Enter_Packet(ob, user_id);
                }
                else
                {
                    m_pSendMgr->Send_Stop_Packet(ob, user_id, isMoving);  // ���⼭ �� ����
                }
            }
        }
        else // �������� �ִ� ���̵� 
        {
            if (IsClient(ob) && ob != user_id)
            {
                if (Cast_Client(m_pMediator->Find(ob))->GetViewList().count(user_id) == 0)
                {
                    Cast_Client(m_pMediator->Find(ob))->GetViewList().insert(user_id);
                    m_pSendMgr->Send_Enter_Packet(ob, user_id);
                }
                else
                {
                    m_pSendMgr->Send_Stop_Packet(ob, user_id,  isMoving);  // ���⼭ �� ����
                }
            }
        }
    }
    for (auto& ob : old_viewList)
    {
        if (new_viewList.count(ob) == 0)
        {
            pClient->GetViewList().erase(ob);
            m_pSendMgr->Send_Leave_Packet(user_id, ob);

            if (IsClient(ob))
            {
                if (Cast_Client(m_pMediator->Find(ob))->GetViewList().count(user_id) != 0)
                {
                    Cast_Client(m_pMediator->Find(ob))->GetViewList().erase(user_id);
                    m_pSendMgr->Send_Leave_Packet(ob, user_id);
                }
            }
        }
    }
}




void CNetMgr::Disconnect(const uShort& user_id)
{
    CGameObject* pUser = m_pMediator->Find( user_id);
    pUser->SetStatus(OBJSTATUS::ST_ALLOC);

    m_pMediator->Delete_Reckoner(user_id);

    m_pSendMgr->Send_Leave_Packet( user_id, user_id);

    pUser->GetLock().lock();
    for (int i = 0; i < MAX_USER; ++i)
    {
        if (m_pMediator->Find( i)->GetStatus() == OBJSTATUS::ST_ACTIVE)
        {
            if (IsClient(i)) {
                if (i != user_id)
                {
                    if (Cast_Client(m_pMediator->Find(i))->GetViewList().count(user_id))
                    {
                        Cast_Client(m_pMediator->Find(i))->GetViewList().erase(user_id);
                        m_pSendMgr->Send_Leave_Packet(i, user_id);
                    }
                }
            }
        }
    }

    pUser->SetStatus(OBJSTATUS::ST_FREE);
    Cast_Client(pUser)->GetViewList().clear();
    pUser->GetLock().unlock();

    closesocket(pUser->GetSocket());
    pUser->SetSocket_Zero();
    // database�ڸ� 
}

void CNetMgr::Enter_Game(const uShort& user_id, char name[])
{
    CClient* pUser = Cast_Client(m_pMediator->Find(user_id));
    pUser->GetLock().lock();
    pUser->SetName(name);
    pUser->GetLock().unlock();
    pUser->GetName()[MAX_ID_LEN] = NULL;
    m_pSendMgr->Send_LoginOK_Packet(user_id);
    pUser->Insert_Sector();

    unordered_set<int> new_viewList;

    vector<unordered_set<uShort>> vSectors = CSectorMgr::GetInst()->Search_Sector(m_pMediator->Find(user_id));


    for (auto& vSec : vSectors)
    {
        for (auto& id : vSec) {
            if (m_pMediator->Find(id)->GetID() == m_pMediator->Find(user_id)->GetID())continue;
            if (m_pMediator->Find(id)->GetStatus() != OBJSTATUS::ST_ACTIVE)continue;
            if (is_near(user_id, id)&&user_id!=id)
            {
               /* if (m_pMediator->Find(id)->GetStatus() == OBJSTATUS::ST_SLEEP)
                {
                    if(IsMonster(id))
                        WakeUp_Monster(id);
                    else if (IsNpc(id))
                        WakeUp_NPC(id);
                }*/
                if (IsClient(id))
                {
                    if (Cast_Client(m_pMediator->Find(id))->GetViewList().count(user_id) == 0)
                    {
                        Cast_Client(m_pMediator->Find(id))->GetViewList().insert(user_id);
                        m_pSendMgr->Send_Enter_Packet(id, user_id);
                    }
                }
                if (pUser->GetViewList().count(id) == 0)
                {
                    pUser->GetViewList().insert(id);
                    m_pSendMgr->Send_Enter_Packet(user_id, id);
                }
            }
        }
    }

    pUser->SetStatus(OBJSTATUS::ST_ACTIVE);
}


void CNetMgr::Process_Packet(const uShort& user_id, char* buf)
{
    switch (buf[1]) {
    case CS_MOVE: {
        cs_packet_move* packet = reinterpret_cast<cs_packet_move*>(buf);


        m_pMediator->Find(user_id)->SetIsMoving(packet->isMoving);
        m_pMediator->Find(user_id)->SetClientTime(packet->move_time);
        m_pMediator->Find(user_id)->SetSpeed(packet->speed);
        m_pMediator->Find(user_id)->SetHalfRTT(packet->Start);
        m_pMediator->Find(user_id)->SetDirV(packet->DirVec);
        m_pMediator->Find(user_id)->SetDeadReckoningPacket(packet);

        m_pMediator->ReckonerAdd(user_id);

        Do_Move(user_id, packet->dir, packet->localVec, packet->rotateY);

    }
                break;
    case CS_STOP:
    {
        cs_packet_stop* packet = reinterpret_cast<cs_packet_stop*>(buf);
        m_pMediator->Find(user_id)->SetIsMoving(packet->isMoving);
        Do_Stop(user_id, packet->isMoving);
    }
    break;
    case CS_LOGIN: {
        cs_packet_login* packet = reinterpret_cast<cs_packet_login*>(buf);
        cout << packet->name << endl;
        Enter_Game(user_id, packet->name);
    }
                 break;
   

    case CS_ATTACK:
    {
        cs_packet_attack* packet = reinterpret_cast<cs_packet_attack*>(buf);
        Do_Attack(user_id, packet->id);
    }
    break;
    case CS_MONSTER_DEAD:
    {
        cs_packet_MonsterDead* packet = reinterpret_cast<cs_packet_MonsterDead*>(buf);
        if(g_Object.count(packet->id)!=0)
            Kill_Monster(packet->id);
    }
    break;
    case CS_ATTACK_ANIMATION:
    {
        cs_packet_AttackAni* packet = reinterpret_cast<cs_packet_AttackAni*>(buf);
        if (g_Object.count(packet->id) == 0)break;
        CClient* monster = Cast_Client(m_pMediator->Find(packet->id));
        unordered_set<uShort> new_viewList;
        vector<unordered_set<uShort>> vSectors = CSectorMgr::GetInst()->Search_Sector(m_pMediator->Find(packet->id));

        for (auto& vSec : vSectors)
        {
            if (vSec.size() != 0)
            {
                for (auto& user : vSec)
                {
                    if (IsClient(user) && is_near(packet->id, user))
                    {
                        new_viewList.insert(user);
                    }
                }
            }
        }
        for (auto& obj : new_viewList)
        {
            m_pSendMgr->Send_Attack_Animation_Packet(obj, packet->id, packet->isAttack);

        }
    }
    break;
    default:
        cout << "Unknown Packet Type Error!\n";
        DebugBreak();
        exit(-1);
    }                       
}

void CNetMgr::Recv_Packet_Construct(const uShort& user_id, const int& io_byte)
{
    CGameObject* pObj = dynamic_cast<CGameObject*>(m_pMediator->Find( user_id));
    //
    EXOVER r_o = (pObj->GetExover());

    int rest_byte = io_byte;
    char* p = r_o.io_buf;
    int packet_size = 0;
    if (0 != pObj->Get_Prev_Size()) packet_size = pObj->Get_Packet_buf()[0];
    while (rest_byte > 0) {
        if (0 == packet_size) packet_size = *p;
        if (packet_size <= rest_byte + pObj->Get_Prev_Size()) {
            memcpy(pObj->Get_Packet_buf() + pObj->Get_Prev_Size(), p, packet_size - pObj->Get_Prev_Size());
            p += packet_size - pObj->Get_Prev_Size();
            rest_byte -= packet_size - pObj->Get_Prev_Size();
            packet_size = 0;
            Process_Packet(user_id, pObj->Get_Packet_buf());
            pObj->SetPrev_Size(0);
        }
        else {
            memcpy(pObj->Get_Packet_buf() + pObj->Get_Prev_Size(), p, rest_byte);
            pObj->SetPrev_Size(pObj->Get_Prev_Size() + rest_byte);
            rest_byte = 0;
            p += rest_byte;
        }
    }
}

void CNetMgr::Init_Client()
{
    CGameObject* pObj = nullptr;
    for (int i = 0; i < MAX_USER; ++i) {
        pObj = new CClient;
        pObj->SetID(i);
        pObj->SetStatus(OBJSTATUS::ST_FREE);
        m_pMediator->Add(pObj,  i);
    }
}
void CNetMgr::Init_Monster()
{
    CGameObject* pObj = nullptr;
    srand((unsigned int)time(NULL));
    for (int i = START_MONSTER; i < END_MONSTER; ++i) {
        pObj = new CMonster;
        pObj->SetPosV((float)(1500), 300.f, (float)(5000 +  (i - 1000) * 400));
        pObj->SetID(i);
        dynamic_cast<CMonster*>(pObj)->SetHP(100);
        pObj->SetStatus(OBJSTATUS::ST_SLEEP);
        pObj->Insert_Sector();
        m_pMediator->Add(pObj, i);
        cout << "ID : " << i << "  HP : " << dynamic_cast<CMonster*>(pObj)->GetHP() << endl;
  
    }
}
void CNetMgr::Init_NPC()
{
    CGameObject* pObj = nullptr;

    for (int i = START_NPC; i < END_NPC; ++i)
    {
        pObj = new CNPC;
        pObj->SetPosV((float)(rand() % WORLD_WIDTH), (float)(rand() % WORLD_WIDTH), (float)(rand() % WORLD_WIDTH));
        pObj->SetID(i);
        char npc_name[50];
        sprintf_s(npc_name, "N%d", i);
        pObj->SetName(npc_name);
        pObj->SetStatus(OBJSTATUS::ST_SLEEP);
        pObj->Insert_Sector();
        m_pMediator->Add(pObj, i);

    }
}

bool CNetMgr::IsClient(const uShort& id)
{
    if (id >= 0 && id < MAX_USER)
        return true;
    else
        return false;
}

bool CNetMgr::IsMonster(const uShort& id)
{
    if (id >= START_MONSTER && id < END_MONSTER)
        return true;
    else
        return false;
}

bool CNetMgr::IsNpc(const uShort& id)
{
    if (id >= START_NPC && id < END_NPC)
        return true;
    else
        return false;
}

CClient* CNetMgr::Cast_Client(CGameObject* obj)
{
    if (obj != nullptr)return dynamic_cast<CClient*>(obj);
    else return nullptr;
}

void CNetMgr::Add_Timer(const uShort& obj_id, const int& status, system_clock::time_point t)
{
    event_type ev{ obj_id, t, status };

    timer_lock.lock();
    timer_queue.push(ev);
    timer_lock.unlock();
}

void CNetMgr::Worker_Thread()
{
    while (true) {
        DWORD io_byte;
        ULONG_PTR key;
        WSAOVERLAPPED* over;
        GetQueuedCompletionStatus(g_iocp, &io_byte, &key, &over, INFINITE);

        EXOVER* exover = reinterpret_cast<EXOVER*>(over);
        uShort user_id = static_cast<uShort>(key);
        
      
        CClient* pUser = Cast_Client(m_pMediator->Find( user_id));
        switch (exover->op) {
        case ENUMOP::OP_RECV:
            if (0 == io_byte) {
                Disconnect(user_id);
                
            }
            else {
                CClient* pClient = Cast_Client(m_pMediator->Find( user_id));
                Recv_Packet_Construct(user_id, io_byte);
                pClient->ZeroMemory_recv_over();
                DWORD flags = 0;
                WSARecv(pClient->GetSocket(), &(pClient->GetExover().wsabuf), 1, NULL, &flags, &(pClient->GetExover().over), NULL);
            }
            break;
        case ENUMOP::OP_SEND:
            if (0 == io_byte) Disconnect(user_id);
            delete exover;
            break;
        case ENUMOP::OP_ACCEPT:
        {
           // ghost++;
            //cout << "Ghost Cnt -> " << ghost << endl;
            uShort user_id = -1;
            uShort i;
            for (i = 0; i < MAX_USER; ++i) {
                m_pMediator->Find( i)->GetLock().lock();
                if (m_pMediator->Find( i)->GetStatus() == OBJSTATUS::ST_FREE) {
                    m_pMediator->Find( i)->SetStatus(OBJSTATUS::ST_ALLOC);
                    user_id = i;
                    //
                    m_pMediator->Find( i)->GetLock().unlock();
                    // ����

                    break;
                }
                m_pMediator->Find( i)->GetLock().unlock();
            }

            SOCKET c_socket = exover->c_socket;

            if (MAX_USER == i) {
                cout << "Max user limit exceeded.\n";
                closesocket(c_socket);
            }

            if (-1 == user_id)
                closesocket(c_socket);
            else {
                CClient* pClient = Cast_Client(m_pMediator->Find( user_id));

                pClient->SetPrev_Size(0);
                pClient->GetExover().op = ENUMOP::OP_RECV;
                pClient->ZeroMemory_recv_over();
                pClient->GetExover().wsabuf.buf = pClient->GetExover().io_buf;
                pClient->GetExover().wsabuf.len = MAX_BUF_SIZE;
                pClient->SetSocket(c_socket);

                ////////////////////////////////////////////////////////
                pClient->SetPosV(
                    (float)(rand() % 1000), // ���� real float
                    (float)(290.f),
                    (float)(rand() % 1000));

                cout << pClient->GetLocalPosVector().x << ", " << pClient->GetLocalPosVector().z << endl;
               // if (pClient->GetLocalPosVector().x == 724.f)
                    //cout << "�ͽ��̴� !!!!" << endl;
                ////////////////////////////////////////////////////////
                
                pClient->SetFirstPos(pClient->GetLocalPosVector());

                CreateIoCompletionPort(reinterpret_cast<HANDLE>(c_socket), g_iocp, user_id, 0);
                DWORD flags = 0;
                int ret;
                pClient->GetLock().lock();
                if (pClient->GetStatus() == OBJSTATUS::ST_ALLOC)
                {
                    ret = WSARecv(pClient->GetSocket(), &pClient->GetExover().wsabuf, 1, NULL,
                        &flags, &pClient->GetExover().over, NULL);
                }
                pClient->GetLock().unlock();

                if (SOCKET_ERROR == ret) {
                    int err_no = WSAGetLastError();
                    if (ERROR_IO_PENDING != err_no)
                        error_display("recv", err_no);
                }
            }
            c_socket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
            exover->c_socket = c_socket;
            ZeroMemory(&exover->over, sizeof(exover->over));
            AcceptEx(l_socket, c_socket, exover->io_buf, NULL,
                sizeof(sockaddr_in) + 16, sizeof(sockaddr_in) + 16, NULL, &exover->over);
        
            m_pSendMgr->Send_ID_Packet(user_id);
        }
        break;
        case ENUMOP::OP_RAMDON_MOVE_NPC:
        {
           // Random_Move_NPC(user_id); 
            bool keep_alive = false;
            //active�� �÷��̾ �ֺ��� ������ ��� �����α�
            for (auto& vSec : CSectorMgr::GetInst()->Search_Sector(m_pMediator->Find(user_id)))
            {
                for (auto& id : vSec)
                {
                    if (IsClient(id))
                    {
                        if (is_near(user_id, id))
                        {
                            if (m_pMediator->Find(id)->GetStatus() == OBJSTATUS::ST_ACTIVE)
                            {
                                keep_alive = true; 
                                break;
                            }
                        }
                    }
                }
           }
            if (true == keep_alive) Add_Timer(user_id, ENUMOP::OP_RAMDON_MOVE_NPC, system_clock::now() + 1s);
            else m_pMediator->Find( user_id)->SetStatus(OBJSTATUS::ST_SLEEP);
            //������ ���� �ƹ��� ������ SLEEP���� ����α� 
            delete exover;

        }
        break;
        case ENUMOP::OP_RAMDON_MOVE_MONSTER:
        {
            if (g_Object.count(user_id) == 0)break;
            //Random_Move_Monster(user_id);
            bool keep_alive = false;
            //active�� �÷��̾ �ֺ��� ������ ��� �����α�
            for (auto& vSec : CSectorMgr::GetInst()->Search_Sector(m_pMediator->Find(user_id)))
            {
                for (auto& id : vSec)
                {
                    if (IsClient(id))
                        if (is_near(user_id, id))
                            if (m_pMediator->Find(id)->GetStatus() == OBJSTATUS::ST_ACTIVE)
                            {
                                keep_alive = true;
                                break;
                            }
                }
            }
            if (true == keep_alive) Add_Timer(user_id, ENUMOP::OP_RAMDON_MOVE_MONSTER, system_clock::now() + 1s);
            else m_pMediator->Find( user_id)->SetStatus(OBJSTATUS::ST_SLEEP);
            //������ ���� �ƹ��� ������ SLEEP���� ����α� 
            delete exover;
        }
        break;
        }
   
    }

}

void CNetMgr::DeadReckoning_Thread()
{
    if (g_Reckoner.size() == 0)return;
    CGameObject* obj = nullptr;
    cs_packet_move* drmPacket = nullptr;
    while (true)
    {
        for (auto& reckoner : g_Reckoner)
        {
            obj = m_pMediator->Find(reckoner);
            drmPacket = obj->GetDeadReckoningPacket();
            if (drmPacket->isMoving)
            {
                obj->SetRotateY(drmPacket->rotateY);
                obj->SetPosV(obj->GetLocalPosVector() + drmPacket->DirVec * obj->GetSpeed() * DeltaTime);
                //Do_Move(reckoner, drmPacket->dir, obj->GetLocalPosVector(), obj->GetRotateY());
            }

        }
    }
    cout << "deadReckoning_ Thread Working" << endl;
}

void CNetMgr::Timer_Worker()
{
    while (true) {
        while (true) {
            if (!timer_queue.empty())
            {
                event_type ev = timer_queue.top();
                if (ev.wakeup_time > system_clock::now())break;
                timer_lock.lock();
                timer_queue.pop();
                timer_lock.unlock();
                if (ev.event_id == ENUMOP::OP_RAMDON_MOVE_NPC)
                {
                    EXOVER* over = new EXOVER();
                    over->op = ENUMOP::OP_RAMDON_MOVE_NPC;
                    PostQueuedCompletionStatus(g_iocp, 1, ev.obj_id, &over->over);
                }
                if (ev.event_id == ENUMOP::OP_RAMDON_MOVE_MONSTER)
                {
                    EXOVER* over = new EXOVER();
                    over->op = ENUMOP::OP_RAMDON_MOVE_MONSTER;
                    PostQueuedCompletionStatus(g_iocp, 1, ev.obj_id, &over->over);
                }
            }
            else break;
        }
        this_thread::sleep_for(1ms);
    }
}
bool CAS(int* addr, int exp, int update)        // cas 
{
    return atomic_compare_exchange_strong(reinterpret_cast<atomic_int*>(addr), &exp, update);
}
void CNetMgr::WakeUp_NPC(const uShort& id)
{
    int status = OBJSTATUS::ST_SLEEP;
    if (CAS((int*)(&(m_pMediator->Find(id)->GetStatus())), status, (int)ST_ACTIVE))
    {
        Add_Timer(id, OP_RAMDON_MOVE_NPC, system_clock::now() + 1s);
    }
}

void CNetMgr::WakeUp_Monster(const uShort& id)
{
    int status = OBJSTATUS::ST_SLEEP;
    if (CAS((int*)(&(m_pMediator->Find(id)->GetStatus())), status, (int)ST_ACTIVE))
    {
        Add_Timer(id, OP_RAMDON_MOVE_MONSTER, system_clock::now() + 1s);
    }
}