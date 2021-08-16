#include "stdafx.h"
#include "CNetMgr.h"
float packetTimeCnt = 0.f;
mutex tempLock;
int cnt = 0;
void CNetMgr::error_display(const char* msg, int err_no)     // 에러 출력
{
    WCHAR* lpMsgBuf;
    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_FROM_SYSTEM,
        NULL, err_no,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR)&lpMsgBuf, 0, NULL);
    std::cout << msg;
    std::wcout << L"에러 " << lpMsgBuf << std::endl;
    while (true);
    LocalFree(lpMsgBuf);
}

void CNetMgr::Random_Move_Monster(const uShort& Monster_id)
{
    CGameObject* MonsterObj = m_pMediator->Find(Monster_id);
    if (MonsterObj == nullptr)return;
    Vector3 MonsterPos = MonsterObj->GetLocalPosVector();
    unordered_set<uShort> new_viewList;


    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(0, 3);
    MONSTER_AUTOMOVE_DIR dir = (MONSTER_AUTOMOVE_DIR)(dis(gen));


    for (auto& new_ids : g_QuadTree.search(CBoundary(m_pMediator->Find(Monster_id))))
    {
        new_viewList.insert(new_ids);
    }
    //tempLock.lock();

    CAST_MONSTER(MonsterObj)->SetIsMoving(true);
    // new viewlist를 알아야함 ..
    //CAST_MONSTER(MonsterObj)->SetDir(dir);
    //tempLock.unlock();
    //for (auto& id : new_viewList)
    //{

    //    // 여기 고쳐야 함
    //    if (m_pMediator->IsType(id, OBJECT_TYPE::CLIENT))
    //    {

    //        tempLock.lock();
    //        //cout << m_pMediator->Find(Monster_id)->GetLocalPosVector().x << ",  " << m_pMediator->Find(Monster_id)->GetLocalPosVector().z << endl;
    //        m_pSendMgr->Send_Monster_Move_Packet(id, Monster_id, (char)dir);
    //        tempLock.unlock();
    //    }
    //}
}

//void CNetMgr::Random_Move_NPC(const int& id)
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
//            if (!m_pMediator->IsType(sec))continue;
//
//            CClient* pClient = CAST_CLIENT(m_pMediator->Find(sec));
//            if (pClient->GetStatus() != OBJSTATUS::ST_ACTIVE)continue;
//
//            if (is_near(sec, id))    // is near이 필요한가?
//            {
//                pClient->GetLock().lock();
//                if (0 == pClient->GetViewList().count(id))  // 뷰리스트에 몬스터가 없다면 엔터 패킷
//                {
//                    pClient->GetLock().unlock();
//                    pClient->GetViewList().insert(id);
//                    Send_Enter_Packet(sec, id);
//                }
//                else                                        // 뷰리스트에 몬스터가 있으면 무브 패킷
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
    if (monster == nullptr)return;

    //vector<unordered_set<uShort>> vSectors = CSectorMgr::GetInst()->Search_Sector(m_pMediator->Find(attacker));// search sector 인자 확인

    unordered_set<uShort> vSectors = g_QuadTree.search(CBoundary(m_pMediator->Find(attacker)));
    //client 인가 monster인가 

    if (monster->GetHP() - 12 >= 0)
    {
        monster->SetHP(monster->GetHP() - 12);
        for (auto& clientID : vSectors)
        {
            if (m_pMediator->IsType(clientID, OBJECT_TYPE::CLIENT))
                m_pSendMgr->Send_Attacked_Packet_Monster(clientID, victim);
        }
    }
    else
    {
            monster->SetHP(0);
            for (auto& clientID : vSectors)
            {

                if (m_pMediator->IsType(clientID, OBJECT_TYPE::CLIENT))
                {
                    m_pSendMgr->Send_Attacked_Packet_Monster(clientID, victim);
                    m_pSendMgr->Send_Monster_Animation_Packet(victim, clientID, MONSTER_ANI_TYPE::DEAD);
                    /*cout << "Do_Attack : "<<clientID << endl;
                    m_pSendMgr->Send_Leave_Packet(clientID, victim, false);*/
                }
            }
    }
}
void CNetMgr::Kill_Monster(const uShort& monster_id)
{
    if (m_pMediator->Find(monster_id) == nullptr)return;

    //vector<unordered_set<uShort>> vSectors = CSectorMgr::GetInst()->Search_Sector(m_pMediator->Find(monster_id));
    if (m_pMediator->Count(monster_id) != 0)
    {
        unordered_set<uShort> vSectors = g_QuadTree.search(CBoundary(m_pMediator->Find(monster_id)));

        // g_QuadTree.Delete(m_pMediator->Find(monster_id));
        tempLock.lock();
        cout << "Do_Attack : " << monster_id << endl;
        for(auto& clientID: m_pMediator->GetReckonerList())
            m_pSendMgr->Send_Leave_Packet(clientID, monster_id, false);
        m_pMediator->Delete_Obj(monster_id);
        m_pMediator->Delete_MonsterReckoner(monster_id);
        tempLock.unlock();
    }
}
void CNetMgr::Do_Move(const uShort& user_id, const char& dir, Vector3& localVec, const float& rotateY)
{
    CClient* pClient = CAST_CLIENT(m_pMediator->Find(user_id));
    if (pClient == nullptr)return;

    unordered_set<uShort> old_viewList = pClient->GetViewList();
    pClient->SetPosV(localVec);
    pClient->SetRotateY(rotateY);

    unordered_set<uShort> new_viewList;

    unordered_set<uShort>vSectors = g_QuadTree.search(CBoundary(m_pMediator->Find(user_id)));
    if (vSectors.size() != 0)
    {
        for (auto& user : vSectors)
        {
            if (!m_pMediator->IsType(user, OBJECT_TYPE::CLIENT))
            {
                if (m_pMediator->Find(user)->GetStatus() == OBJSTATUS::ST_SLEEP)
                {
                    if (m_pMediator->IsType(user, OBJECT_TYPE::MONSTER))
                        WakeUp_Monster(user);
                    else
                    {
                        // WakeUp_NPC(user);
                    }
                }
            }
            new_viewList.insert(user);
        }
    }


    for (auto& ob : new_viewList) //시야에 새로 들어온 객체 구분
    {
        if (0 == old_viewList.count(ob)) // 새로 들어온 아이디
        {
            pClient->GetViewList().insert(ob);
            m_pSendMgr->Send_Enter_Packet(user_id, ob);

            if (CAST_CLIENT(m_pMediator->Find(ob))->GetViewList().count(user_id))
                m_pSendMgr->Send_Enter_Packet(ob, user_id);
            else
                m_pSendMgr->Send_Move_Packet(ob, user_id, dir);  // 여기서 또 들어옴

                //if (m_pMediator->IsType(ob, OBJECT_TYPE::CLIENT) && ob != user_id)
                //{
                //    
                //    //if (CAST_CLIENT(m_pMediator->Find(ob))->GetViewList().count(user_id) == 0)
                //    //{
                //    //    CAST_CLIENT(m_pMediator->Find(ob))->GetViewList().insert(user_id);
                //    //    m_pSendMgr->Send_Enter_Packet(ob, user_id);
                //    //}
                //    //else
                //    //{
                //    //    m_pSendMgr->Send_Move_Packet(ob, user_id, dir);  // 여기서 또 들어옴
                //    //}
                //}
        }
        else // 이전에도 있던 아이디 
        {
            if (m_pMediator->IsType(ob, OBJECT_TYPE::CLIENT) && ob != user_id)
            {
                if (CAST_CLIENT(m_pMediator->Find(ob))->GetViewList().count(user_id) == 0)
                {
                    m_pSendMgr->Send_Enter_Packet(ob, user_id);
                }
                else
                {
                    CAST_CLIENT(m_pMediator->Find(ob))->GetViewList().insert(user_id);
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
            tempLock.lock();
            cout << "\t\t\tDo_Move---------------------" << endl;
            m_pSendMgr->Send_Leave_Packet(user_id, ob);
            tempLock.unlock();
            if (CAST_CLIENT(m_pMediator->Find(ob))->GetViewList().count(user_id) != 0)
            {
                CAST_CLIENT(m_pMediator->Find(ob))->GetViewList().erase(user_id);
                cout << "\t\t\tDo_Move---------------------안쪽" << endl;
                m_pSendMgr->Send_Leave_Packet(ob, user_id);
            }
        }

    }
}

void CNetMgr::Do_Stop(const uShort& user_id, const bool& isMoving)
{
    CClient* pClient = CAST_CLIENT(m_pMediator->Find(user_id));
    if (pClient == nullptr)return;

    unordered_set<uShort> old_viewList = pClient->GetViewList();
    tempLock.lock();
    pClient->SetIsMoving(false);
    tempLock.unlock();
    unordered_set<uShort>vSectors = g_QuadTree.search(CBoundary(m_pMediator->Find(user_id)));

    for (auto& ob : vSectors)
    {
        m_pSendMgr->Send_Stop_Packet(ob, user_id, isMoving);
    }

    //for (auto& ob : vSectors)
    //{
    //    //시야에 새로 들어온 객체 구분

    //    if (0 == old_viewList.count(ob)) // 새로 들어온 아이디
    //    {
    //        pClient->GetViewList().insert(ob);
    //        m_pSendMgr->Send_Enter_Packet(user_id, ob);
    //        if (m_pMediator->IsType(ob, OBJECT_TYPE::CLIENT) && ob != user_id)
    //        {
    //            if (CAST_CLIENT(m_pMediator->Find(ob))->GetViewList().count(user_id) == 0)
    //            {
    //                CAST_CLIENT(m_pMediator->Find(ob))->GetViewList().insert(user_id);
    //                m_pSendMgr->Send_Enter_Packet(ob, user_id);
    //            }
    //            else
    //            {
    //                m_pSendMgr->Send_Stop_Packet(ob, user_id, isMoving);  // 여기서 또 들어옴
    //            }
    //        }
    //    }
    //    else // 이전에도 있던 아이디 
    //    {
    //        if (m_pMediator->IsType(ob, OBJECT_TYPE::CLIENT) && ob != user_id)
    //        {
    //            if (CAST_CLIENT(m_pMediator->Find(ob))->GetViewList().count(user_id) == 0)
    //            {
    //                CAST_CLIENT(m_pMediator->Find(ob))->GetViewList().insert(user_id);
    //                m_pSendMgr->Send_Enter_Packet(ob, user_id);
    //            }
    //            else
    //            {
    //                m_pSendMgr->Send_Stop_Packet(ob, user_id, isMoving);  // 여기서 또 들어옴
    //            }
    //        }
    //    }
    //}
    //for (auto& ob : old_viewList)
    //{
    //    if (vSectors.count(ob) == 0)
    //    {
    //        pClient->GetViewList().erase(ob);
    //        cout << "Do_Stop---------------------" << endl;
    //        m_pSendMgr->Send_Leave_Packet(user_id, ob);

    //        if (m_pMediator->IsType(ob, OBJECT_TYPE::CLIENT))
    //        {
    //            if (CAST_CLIENT(m_pMediator->Find(ob))->GetViewList().count(user_id) != 0)
    //            {
    //                CAST_CLIENT(m_pMediator->Find(ob))->GetViewList().erase(user_id);
    //                cout << "Do_Stop---------------------안쪽" << endl;
    //                m_pSendMgr->Send_Leave_Packet(ob, user_id);
    //            }
    //        }
    //    }
    //}
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
            if (m_pMediator->IsType(i, OBJECT_TYPE::CLIENT)) {
                if (i != user_id)
                {
                    if (CAST_CLIENT(m_pMediator->Find(i))->GetViewList().count(user_id))
                    {
                        CAST_CLIENT(m_pMediator->Find(i))->GetViewList().erase(user_id);
                        m_pSendMgr->Send_Leave_Packet(i, user_id);
                    }
                }
            }
        }
    }
    
    pUser->SetStatus(OBJSTATUS::ST_FREE);
    CAST_CLIENT(pUser)->GetViewList().clear();
    pUser->GetLock().unlock();

    closesocket(pUser->GetSocket());
    pUser->SetSocket_Zero();
    // database자리 
}

void CNetMgr::Enter_Game(const uShort& user_id, char name[])
{
    CClient* pUser = CAST_CLIENT(m_pMediator->Find(user_id));
    pUser->GetLock().lock();
    pUser->SetName(name);
    pUser->GetLock().unlock();
    pUser->GetName()[MAX_ID_LEN] = NULL;
    // 0624
    pUser->Insert_Sector();

    unordered_set<int> new_viewList;

    unordered_set<uShort> vSectors = g_QuadTree.search(CBoundary(m_pMediator->Find(user_id)));

    for (auto& id : vSectors)
    {
        if (id == user_id)continue;
        if (m_pMediator->Find(id)->GetStatus() != OBJSTATUS::ST_ACTIVE)continue;
         if (m_pMediator->Find(id)->GetStatus() == OBJSTATUS::ST_SLEEP)
         {
             if(m_pMediator->IsType(id, OBJECT_TYPE::MONSTER))
                 WakeUp_Monster(id);
             /*else if (IsNpc(id))
                 WakeUp_NPC(id);*/
         }
         tempLock.lock();
        if (m_pMediator->IsType(id, OBJECT_TYPE::CLIENT))
        {
            if (CAST_CLIENT(m_pMediator->Find(id))->GetViewList().count(user_id) == 0)
            {
                CAST_CLIENT(m_pMediator->Find(id))->GetViewList().insert(user_id);
                m_pSendMgr->Send_Enter_Packet(id, user_id);
            }
        }
        if (pUser->GetViewList().count(id) == 0)
        {
            pUser->GetViewList().insert(id);
            m_pSendMgr->Send_Enter_Packet(user_id, id);
        }
        tempLock.unlock();
    }

    if (m_pMediator->IsType(user_id, OBJECT_TYPE::CLIENT)) pUser->SetStatus(OBJSTATUS::ST_ACTIVE);
}

void CNetMgr::Process_Packet(const uShort& user_id, char* buf)
{
    switch (buf[1]) {
    case CS_MOVE: {
        cs_packet_move* packet = reinterpret_cast<cs_packet_move*>(buf);

        if (user_id == 0)
        {
            cout << "Packet 받았을때 현재 서버 상태" << endl;
            cout << "-------------------------------" << endl;
            cout << "-------------------------------" << endl;
            cout << m_pMediator->Find(user_id)->GetLocalPosVector().x << " , " << m_pMediator->Find(user_id)->GetLocalPosVector().z << endl;
            cout << "-------------------------------" << endl;
            cout << "-------------------------------" << endl;
            cout << "************************************************" << endl;
            cout << "Packet 받았을때 Packet 상태" << endl;
            cout << "-------------------------------" << endl;
            cout << "-------------------------------" << endl;
            cout << packet->localVec.x << " , " << packet->localVec.z << endl;
            cout << "-------------------------------" << endl;
            cout << "-------------------------------" << endl;
        }
        tempLock.lock();
        m_pMediator->Find(user_id)->SetIsMoving(true);
        tempLock.unlock();
        m_pMediator->Find(user_id)->SetClientTime(packet->move_time);
        m_pMediator->Find(user_id)->SetSpeed(packet->speed);
        m_pMediator->Find(user_id)->SetHalfRTT(packet->Start);
        m_pMediator->Find(user_id)->SetDirV(packet->DirVec);
        m_pMediator->Find(user_id)->SetDeadReckoningPacket(packet);
   
        //
        m_pMediator->ReckonerAdd(user_id);

        Do_Move(user_id, packet->dir, packet->localVec, packet->rotateY);

    }
                break;
    case CS_STOP:
    {
        cs_packet_stop* packet = reinterpret_cast<cs_packet_stop*>(buf);
        tempLock.lock();
        m_pMediator->Find(user_id)->SetIsMoving(false);
        tempLock.unlock();

        Do_Stop(user_id, packet->isMoving);
    }
    break;
    case CS_LOGIN: {
        cs_packet_login* packet = reinterpret_cast<cs_packet_login*>(buf) ;
        //cout << "send loginok packet" << endl;
        m_pSendMgr->Send_LoginOK_Packet(user_id);

        Enter_Game(user_id, packet->name);
    }
                 break;
   

    case CS_ATTACK:
    {
        cs_packet_attack* packet = reinterpret_cast<cs_packet_attack*>(buf);
        Do_Attack(user_id, packet->id);
    }
    break;
    case CS_ROTATE:
    {
        cs_packet_rotate* packet = reinterpret_cast<cs_packet_rotate*>(buf);
        unordered_set<uShort>vSectors = g_QuadTree.search(CBoundary(m_pMediator->Find(user_id)));
        for (auto& obj : vSectors)
        {
            if (m_pMediator->IsType(obj, OBJECT_TYPE::CLIENT))
            {
                m_pSendMgr->Send_Rotate_Packet(user_id, obj, packet->rotate);
            }
        }

    }
        break;
    case CS_MONSTER_DEAD:
    {
        cs_packet_MonsterDead* packet = reinterpret_cast<cs_packet_MonsterDead*>(buf);
        //m_pMediator->Find(packet->id)->SetIsMoving(false);

        if(m_pMediator->Count(packet->id)!=0)
            Kill_Monster(packet->id);
    }
    break;
    case CS_ATTACK_ANIMATION:
    {
        cs_packet_Animation* packet = reinterpret_cast<cs_packet_Animation*>(buf);
        if (m_pMediator->Count(packet->id) == 0)break;
        CClient* monster = CAST_CLIENT(m_pMediator->Find(packet->id));
        unordered_set<uShort> new_viewList = g_QuadTree.search(m_pMediator->Find(user_id));
        for (auto& user : new_viewList)
        {
            if (m_pMediator->IsType(user, OBJECT_TYPE::CLIENT))
                m_pSendMgr->Send_Attack_Animation_Packet(user, packet->id, packet->isact, (Ani_TYPE)packet->anitype);
        }
    }
    break;
    case CS_MONSTER_ANIMATION:
    {
        cs_pcaket_Monster_Animation* packet = reinterpret_cast<cs_pcaket_Monster_Animation*>(buf);

        //if (m_pMediator->Count(packet->id) == 0)break;
        CMonster* monster = CAST_MONSTER(m_pMediator->Find(packet->id));
        unordered_set<uShort> new_viewList = g_QuadTree.search(m_pMediator->Find(packet->id));
        for (auto& user : new_viewList)
        {
            if (m_pMediator->IsType(user, OBJECT_TYPE::CLIENT))
            {/*
                cout << "monster animation user id -> " << user << endl;
                cout << "monster animation monster id -> " << packet->id << endl;
                cout << "monster animation ani type -> " << (int)packet->aniType << endl;*/
                m_pMediator->Find(packet->id)->SetIsMoving(packet->isMoving);
                
                if (MONSTER_ANI_TYPE::ATTACK == packet->aniType)
                {
                    cout << "\t\t\t\t " << user_id << endl;
                    m_pSendMgr->Send_Monster_Animation_Packet(packet->id, user, packet->aniType, user_id);
                }
                else
                    m_pSendMgr->Send_Monster_Animation_Packet(packet->id, user, packet->aniType);
            }
        }
    }
    break;
    case CS_ITEMCREATE:
    {
        cs_packet_ItemCreate_Packet* packet = reinterpret_cast<cs_packet_ItemCreate_Packet*>(buf);
        for (auto& obj : m_pMediator->GetReckonerList())
        {
            m_pSendMgr->Send_ItemCreate_Packet(obj, packet->vPos);
        }
    }
        break;
    case CS_ITEMDELETE:
    {
        cs_packet_ItemDelete_Packet* packet = reinterpret_cast<cs_packet_ItemDelete_Packet*>(buf);
        
        for (auto& obj : m_pMediator->GetReckonerList())
        {
            cout << packet->vPos.x << " - " << packet->vPos.y << " - " << packet->vPos.z << " - " << endl;
            m_pSendMgr->Send_ItemDelete_Packet(obj, packet->vPos);

        }
    }
    break;

    case CS_MONSTERDIR:
    {
        cs_pcaket_MonsterDir* packet = reinterpret_cast<cs_pcaket_MonsterDir*>(buf);
        Vector3 temp = packet->dir;
        m_pMediator->Find(packet->id)->SetDirV(temp);
        cout << "///////////////////" << endl;

        cout << packet->dir.z << endl;
        cout << "///////////////////" << endl;
        if (!CAST_MONSTER(m_pMediator->Find(packet->id))->GetIsDir())
        {
            CAST_MONSTER(m_pMediator->Find(packet->id))->SetIsDir(true);
        }
    }
    break;

    case CS_ATTACKEFFECT:
    {
        cs_packet_Attack_Effect* packet = reinterpret_cast<cs_packet_Attack_Effect*>(buf);
        unordered_set<uShort> new_viewList = g_QuadTree.search(m_pMediator->Find(packet->id));

        for (auto& obj : new_viewList)
        {
            if (m_pMediator->IsType(obj, OBJECT_TYPE::CLIENT))
            {
                m_pSendMgr->Send_Attack_Effect(obj, packet->pos);
            }
        }


    }
    break;
    default:
        cout << "Unknown Packet Type Error!\n";
        cout << (int)buf[1] << endl;
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
        GetQueuedCompletionStatus(g_IocpHandle, &io_byte, &key, &over, INFINITE);

        EXOVER* exover = reinterpret_cast<EXOVER*>(over);
        uShort user_id = static_cast<uShort>(key);
        
        //cout << "ID: " << user_id << endl;
        //cout << "iobyte: " << io_byte << endl;
        //cout << "-----------------------" << endl;
        if (user_id != 10000)
        {

        }
        CClient* pUser = CAST_CLIENT(m_pMediator->Find( user_id));
        switch (exover->op) {
        case ENUMOP::OP_RECV:
            if (0 == io_byte) {
                Disconnect(user_id);
                
            }
            else {
                CClient* pClient = CAST_CLIENT(m_pMediator->Find( user_id));
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
                    // 여기

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
                CClient* pClient = CAST_CLIENT(m_pMediator->Find( user_id));

                pClient->SetPrev_Size(0);
                pClient->GetExover().op = ENUMOP::OP_RECV;
                pClient->ZeroMemory_recv_over();
                pClient->GetExover().wsabuf.buf = pClient->GetExover().io_buf;
                pClient->GetExover().wsabuf.len = MAX_BUF_SIZE;
                pClient->SetSocket(c_socket);
                // 0623용석
                g_QuadTree.Insert(pClient);

                ////////////////////////////////////////////////////////
                pClient->SetPosV(
                    (float)(rand() % 1000)+500.f, // 수정 real float
                    (float)(240.f),
                    (float)(rand() % 1000) + 500.f);

                cout << pClient->GetLocalPosVector().x << ", " << pClient->GetLocalPosVector().z << endl;
                
                pClient->SetFirstPos(pClient->GetLocalPosVector());

                CreateIoCompletionPort(reinterpret_cast<HANDLE>(c_socket), g_IocpHandle, user_id, 0);
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
            AcceptEx(g_listenSocket, c_socket, exover->io_buf, NULL,
                sizeof(sockaddr_in) + 16, sizeof(sockaddr_in) + 16, NULL, &exover->over);
            m_pSendMgr->Send_ID_Packet(user_id);

        }
        break;
        case ENUMOP::OP_RAMDON_MOVE_NPC:
        {
           //// Random_Move_NPC(user_id); 
           // bool keep_alive = false;
           // //active인 플레이어가 주변에 있으면 계속 깨워두기
           // for (auto& vSec : CSectorMgr::GetInst()->Search_Sector(m_pMediator->Find(user_id)))
           // {
           //     for (auto& id : vSec)
           //     {
           //         if (m_pMediator->IsType(id, OBJECT_TYPE::CLIENT))
           //         {
           //             if (CSectorMgr::GetInst()->Is_Near(user_id, id))
           //             {
           //                 if (m_pMediator->Find(id)->GetStatus() == OBJSTATUS::ST_ACTIVE)
           //                 {
           //                     keep_alive = true; 
           //                     break;
           //                 }
           //             }
           //         }
           //     }
           //}
           // if (true == keep_alive) Add_Timer(user_id, ENUMOP::OP_RAMDON_MOVE_NPC, system_clock::now() + monsterAutoMoveTimer);
           // else m_pMediator->Find( user_id)->SetStatus(OBJSTATUS::ST_SLEEP);
           // //주위에 이제 아무도 없으면 SLEEP으로 멈춰두기 
           // delete exover;

        }
        break;
        case ENUMOP::OP_RAMDON_MOVE_MONSTER:
        {
            if (m_pMediator->Find(user_id) == nullptr)break;
            if (!m_pMediator->IsType(user_id, OBJECT_TYPE::MONSTER))break;
            if (m_pMediator->Count(user_id) == 0)break;
            m_pMediator->MonsterReckonerAdd(user_id);
            Random_Move_Monster(user_id);
            bool keep_alive = false;
            //active인 플레이어가 주변에 있으면 계속 깨워두기
            unordered_set<uShort> view = g_QuadTree.search(CBoundary(Netmgr.GetMediatorMgr()->Find(user_id)));
            for (auto& id : view)
            {
                if (m_pMediator->IsType(id, OBJECT_TYPE::CLIENT))
                    if (m_pMediator->Find(id)->GetStatus() == OBJSTATUS::ST_ACTIVE)
                    {
                        keep_alive = true;
                        char temp = (char)(rand() % 4);
                        CAST_MONSTER(m_pMediator->Find(user_id))->SetIsMoving(true);
                        CAST_MONSTER(m_pMediator->Find(user_id))->SetDir((MONSTER_AUTOMOVE_DIR)temp);
                        break;
                    }
            }
            
            if (true == keep_alive) {
                if (user_id < 1001)
                    Add_Timer(user_id, ENUMOP::OP_RAMDON_MOVE_MONSTER, system_clock::now() + (seconds)(rand() % 5 + 5));
                else
                    Add_Timer(user_id, ENUMOP::OP_RAMDON_MOVE_MONSTER, system_clock::now() + (seconds)(rand() % 7 + 3));

            }
            else {
                m_pMediator->Find(user_id)->SetStatus(OBJSTATUS::ST_SLEEP);
                CAST_MONSTER(m_pMediator->Find(user_id))->SetDir(MONSTER_AUTOMOVE_DIR::AUTO);
                m_pMediator->Delete_MonsterReckoner(user_id);
                CAST_MONSTER(m_pMediator->Find(user_id))->SetIsMoving(false);
                //for auto viewand send packet;
            }
            //주위에 이제 아무도 없으면 SLEEP으로 멈춰두기 
            delete exover;
        }
        break;
        }
   
    }

}

void CNetMgr::Processing_Thead()
{

    while (true)
    {
        CTimeMgr::GetInst()->update();

        if (m_pMediator->ReckonerSize() != 0)
        {
            CGameObject* obj = nullptr;
            cs_packet_move* drmPacket = nullptr;
            Vector3 objPos;
            for (auto& reckoner : m_pMediator->GetReckonerList())
            {
                if (m_pMediator->Find(reckoner)->GetDeadReckoningPacket() == nullptr)continue;
                obj = m_pMediator->Find(reckoner);
                objPos = obj->GetLocalPosVector();
                drmPacket = obj->GetDeadReckoningPacket();
                if (obj->GetIsMoving())
                {

                    obj->SetRotateY(drmPacket->rotateY);

                    //여기

                    obj->GetLock().lock();
                    g_QuadTree.Delete(obj);
                    obj->SetPosV(obj->GetLocalPosVector() + drmPacket->DirVec * obj->GetSpeed() * (DeltaTime));
                    if(reckoner == 0)
                        cout << "\t\t" << obj->GetLocalPosVector().x << " , " << obj->GetLocalPosVector().z << endl;

                    g_QuadTree.Insert(obj);
                    obj->GetLock().unlock();

                    // do move 내용 옮기기


                    CAST_CLIENT(obj)->CountRefreshPacketCnt(DeltaTime);
                    if (CAST_CLIENT(obj)->GetRefreshPacketCnt() > 0.5f)
                    {
                        //cout << reckoner << "번 플레이어의 데드레커닝 동기화 패킷 전송" << endl;
                         m_pSendMgr->Send_Move_Packet(reckoner, reckoner, drmPacket->dir);
                        CAST_CLIENT(obj)->SetRefreshPacketCnt_Zero();
                    }
                }
            }
        }

        if (m_pMediator->MonsterReckonerSize() != 0)
        {
            unordered_set<uShort> old_viewList;
            unordered_set<uShort> new_viewList;
            CGameObject* MonsterObj = nullptr;
            Vector3 monsterPos;
            MONSTER_AUTOMOVE_DIR monsterDir;
            bool ismoving = false;
            for (auto& monster : m_pMediator->GetMonsterReckonerList())
            {
                if (m_pMediator->Find(monster) == nullptr)continue;
                if (m_pMediator->MonsterReckonerCount(monster) == 0)continue;
                if (!m_pMediator->Find(monster)->GetIsMoving())continue;

            }
            for (auto& monster : m_pMediator->GetMonsterReckonerList())
            {
                if (m_pMediator->Find(monster) == nullptr)continue;
                if (m_pMediator->MonsterReckonerCount(monster) == 0)continue;
                if (!m_pMediator->Find(monster)->GetIsMoving())continue;
               // if (monster != 1000)continue;
                MonsterObj = m_pMediator->Find(monster);
                monsterPos = m_pMediator->Find(monster)->GetLocalPosVector();
                monsterDir = CAST_MONSTER(m_pMediator->Find(monster))->GetDir();
                ismoving = m_pMediator->Find(monster)->GetIsMoving();

                //CAST_MONSTER(MonsterObj)->CountRefreshPacketCnt(DeltaTime);
                for (auto& old_ids : g_QuadTree.search(CBoundary(m_pMediator->Find(monster))))
                {
                    old_viewList.insert(old_ids);
                }

                float speed = 100.f;
                if (ismoving&& CAST_MONSTER(m_pMediator->Find(monster))->GetIsDir())
                {
                   // tempLock.lock();
                    switch (monsterDir)
                    {
                    case MONSTER_AUTOMOVE_DIR::FRONT:
                        monsterPos += speed * DT * m_pMediator->Find(monster)->GetDirVector();
                        cout << m_pMediator->Find(monster)->GetDirVector().z << endl;
                        break;
                    case MONSTER_AUTOMOVE_DIR::BACK:
                        monsterPos += speed * DT * m_pMediator->Find(monster)->GetDirVector();
                        break;
                    case MONSTER_AUTOMOVE_DIR::LEFT:
                        monsterPos += speed * DT * m_pMediator->Find(monster)->GetDirVector();
                        break;
                    case MONSTER_AUTOMOVE_DIR::RIGHT:
                        monsterPos += speed * DT * m_pMediator->Find(monster)->GetDirVector();
                        break;
                    case MONSTER_AUTOMOVE_DIR::AUTO:
                        break;
                    case MONSTER_AUTOMOVE_DIR::IDLE:
                        break;
                    default:
                        break;
                    }
                   // tempLock.unlock();
                    if(monster==1000)
                        cout << m_pMediator->Find(monster)->GetLocalPosVector().x << ",  " << m_pMediator->Find(monster)->GetLocalPosVector().z << endl;

                    //if (m_pMediator->Find(monster) != nullptr)
                    {
                       // tempLock.lock();
                        if (m_pMediator->Count(monster) == 0)continue;
                        g_QuadTree.Delete(m_pMediator->Find(monster));
                        m_pMediator->Find(monster)->SetPosV(monsterPos);
                        if(monster == 1000)
                        //cout << m_pMediator->Find(monster)->GetLocalPosVector().x << " , " << m_pMediator->Find(monster)->GetLocalPosVector().z << endl;
                        //cout << m_pMediator->Find(monster)->GetLocalPosVector().x << ", " << m_pMediator->Find(monster)->GetLocalPosVector().z << endl;
                        CAST_MONSTER(m_pMediator->Find(monster))->CountRefreshPacketCnt(DeltaTime);
                        if (CAST_MONSTER(m_pMediator->Find(monster))->GetRefreshPacketCnt() >2.f)
                            for (auto& user : old_viewList) {
                                if (m_pMediator->IsType(user, OBJECT_TYPE::CLIENT))
                                {
                                    //cout << reckoner << "번 플레이어의 데드레커닝 동기화 패킷 전송" << endl;

                                    m_pSendMgr->Send_Monster_Move_Packet(user, monster, (char)CAST_MONSTER(m_pMediator->Find(monster))->GetDir());

                                    CAST_MONSTER(m_pMediator->Find(monster))->SetRefreshPacketCnt_Zero();
                                }
                                /*if (m_pMediator->IsType(user, OBJECT_TYPE::CLIENT))
                                    m_pSendMgr->Send_Monster_Move_Packet(user, monster, (char)monsterDir);*/
                            }
                        g_QuadTree.Insert(m_pMediator->Find(monster));

                        //tempLock.unlock();

                        for (auto& new_ids : g_QuadTree.search(CBoundary(m_pMediator->Find(monster))))
                        {

                            new_viewList.insert(new_ids);
                        }
                        for (auto& id : new_viewList)
                        {
                            if (old_viewList.count(id) == 0)
                            {
                                if (m_pMediator->IsType(id, OBJECT_TYPE::CLIENT))
                                {
                                    //cout << "new_viewlist ID : " << id << endl;
                                    CAST_CLIENT(m_pMediator->Find(id))->GetViewList().emplace(monster);
                                    m_pSendMgr->Send_Enter_Packet(id, monster);
                                }
                            }
                        }
                        for (auto& id : old_viewList)
                        {
                            if (new_viewList.count(id) == 0)
                            {
                                if (m_pMediator->IsType(id, OBJECT_TYPE::CLIENT))
                                {
                                    cout << "Leave packet send" << endl;
                                    //cout << "old_viewlist ID : " << id << endl;
                                    CAST_MONSTER(m_pMediator->Find(monster))->SetIsMoving(false);
                                    CAST_CLIENT(m_pMediator->Find(id))->GetViewList().erase(monster);
                                    m_pMediator->Delete_MonsterReckoner(monster);
                                    m_pMediator->Delete_Obj(monster);

                                    cout << "\t\t\tProcess packet monster dead reckoining" << endl;
                                    m_pSendMgr->Send_Leave_Packet(id, monster);
                                }
                            }
                        }


                    }
                }
            }
        }
    }
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
                    PostQueuedCompletionStatus(g_IocpHandle, 1, ev.obj_id, &over->over);
                }
                if (ev.event_id == ENUMOP::OP_RAMDON_MOVE_MONSTER)
                {
                    EXOVER* over = new EXOVER();
                    over->op = ENUMOP::OP_RAMDON_MOVE_MONSTER;
                    PostQueuedCompletionStatus(g_IocpHandle, 1, ev.obj_id, &over->over);
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
        Add_Timer(id, OP_RAMDON_MOVE_NPC, system_clock::now() + (seconds)(rand() % 4 + 5));
    }
}

void CNetMgr::WakeUp_Monster(const uShort& id)
{
    int status = OBJSTATUS::ST_SLEEP;
    srand((unsigned int)time(NULL));
    if(CAST_MONSTER(m_pMediator->Find(id))->GetBisMoving())return;

    //CAST_MONSTER(m_pMediator->Find(id))->SetBisMoving(false);
    if (CAS((int*)(&(m_pMediator->Find(id)->GetStatus())), status, (int)ST_ACTIVE))
    {
        //CAS(메모리, expected, update)
        //    메모리의 값이 expected면 update로 바꾸고, true 리턴
        //    메모리의 값이 expected가 아니면 false 리턴
        //    WAIT FREE를 유지하는 알고리즘

        Add_Timer(id, ENUMOP::OP_RAMDON_MOVE_MONSTER, system_clock::now());

       /* if (id < 1001)
            Add_Timer(id, ENUMOP::OP_RAMDON_MOVE_MONSTER, system_clock::now() + (seconds)(rand() % 4 + 5));
        else
            Add_Timer(id, ENUMOP::OP_RAMDON_MOVE_MONSTER, system_clock::now() + (seconds)(rand() % 2 + 3));*/
    }
}