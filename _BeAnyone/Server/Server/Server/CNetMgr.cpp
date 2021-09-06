#include "stdafx.h"
#include "CNetMgr.h"
float packetTimeCnt = 0.f;
mutex tempLock;
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

void CNetMgr::Random_Move_Monster(const uShort& Monster_id)
{
    CGameObject* MonsterObj = m_pMediator->Find(Monster_id);
    if (MonsterObj == nullptr)return;
    Vector3 MonsterPos = MonsterObj->GetLocalPosVector();
    tempLock.lock();
    CAST_MONSTER(MonsterObj)->SetIsMoving(true);
    tempLock.unlock();
}

void CNetMgr::Do_Attack(const uShort& attacker, const uShort& victim)
{
    CMonster* monster = dynamic_cast<CMonster*>(m_pMediator->Find(victim));
    if (monster == nullptr)return;

    //vector<unordered_set<uShort>> vSectors = CSectorMgr::GetInst()->Search_Sector(m_pMediator->Find(attacker));// search sector ���� Ȯ��

    unordered_set<uShort> vSectors = g_QuadTree.search(CBoundary(m_pMediator->Find(victim)));
    //client �ΰ� monster�ΰ� 

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
                    /*//cout << "Do_Attack : "<<clientID << endl;
                    m_pSendMgr->Send_Leave_Packet(clientID, victim, false);*/
                }
            }
    }
}
void CNetMgr::Kill_Monster(const uShort& monster_id)
{
    if (m_pMediator->Find(monster_id) == nullptr)return;

    //vector<unordered_set<uShort>> vSectors = CSectorMgr::GetInst()->Search_Sector(m_pMediator->Find(monster_id));

    tempLock.lock();
    g_QuadTree.Delete(m_pMediator->Find(monster_id));
    unordered_set<uShort> vSectors = g_QuadTree.search(CBoundary(m_pMediator->Find(monster_id)));
    Netmgr.GetMediatorMgr()->Find(monster_id)->SetIsMoving(false);
    //cout << "Do_Attack : " << monster_id << endl;
    for (auto& clientID : vSectors)
    {
        if (m_pMediator->IsType(clientID, OBJECT_TYPE::CLIENT))
        {
            cout << "\t\t\tKillMonster" << endl;
            m_pSendMgr->Send_Leave_Packet(clientID, monster_id, false);
        }
    }
    m_pMediator->Delete_Obj(monster_id);
    m_pMediator->Delete_MonsterReckoner(monster_id);
    tempLock.unlock();

}
void CNetMgr::Do_Move(const uShort& user_id, const char& dir, Vector3& localVec, const float& rotateY)
{
    CClient* pClient = CAST_CLIENT(m_pMediator->Find(user_id));
    if (pClient == nullptr)return;

    if (pClient->GetLocalPosVector().x < 0 || pClient->GetLocalPosVector().z < 0 || pClient->GetLocalPosVector().x>15000 || pClient->GetLocalPosVector().z>15000)
    {
       int i = 0;
    }
    pClient->SetPosV(localVec);
    pClient->SetRotateY(rotateY);

    unordered_set<uShort>new_viewList = g_QuadTree.search(CBoundary(m_pMediator->Find(user_id)));

    for (auto& user : new_viewList)
    {
        if (!m_pMediator->IsType(user, OBJECT_TYPE::CLIENT))
        {
            if (m_pMediator->Find(user)->GetStatus() == OBJSTATUS::ST_SLEEP)
            {
                if (m_pMediator->IsType(user, OBJECT_TYPE::MONSTER))
                {
                    if (user == BOSS_ID)
                    {
                        cout << "Boss Wake Up" << endl;
                    }
                    WakeUp_Monster(user);
                }
            }
        }
    }
}

void CNetMgr::Do_Stop(const uShort& user_id, const bool& isMoving)
{
    CClient* pClient = CAST_CLIENT(m_pMediator->Find(user_id));
    if (pClient == nullptr)return;


    unordered_set<uShort>vSectors = g_QuadTree.search(CBoundary(m_pMediator->Find(user_id)));
    m_pMediator->Find(user_id)->SetIsMoving(false);
    /*cout << user_id << "�� stop packet ����" << endl;
    cout << "-*---------------*-*-*-*-" << endl;*/

    for (auto& ob : vSectors)
    {
        if (ob == user_id)continue;
        m_pSendMgr->Send_Stop_Packet(ob, user_id, false);

    }
    //for (auto& ob : g_QuadTree.search(CBoundary(m_pMediator->Find(user_id))))
    //{
    //    if (ob == user_id)continue;
    //    if (m_pMediator->IsType(ob, OBJECT_TYPE::CLIENT))
    //    {
    //        m_pSendMgr->Send_Stop_Packet(ob, user_id, false);
    //    }
    //}
}

void CNetMgr::Disconnect(const uShort& user_id)
{
    CGameObject* pUser = m_pMediator->Find( user_id);
    pUser->GetLock().lock();
    pUser->SetStatus(OBJSTATUS::ST_ALLOC);
    m_pMediator->Delete_Reckoner(user_id);

    Netmgr.GetMediatorMgr()->Find(user_id)->SetIsMoving(false);
    m_pSendMgr->Send_Leave_Packet( user_id, user_id);

    for (int i = 0; i < MAX_USER; ++i)
    {
        if (m_pMediator->Find( i)->GetStatus() == OBJSTATUS::ST_ACTIVE)
        {
            if (m_pMediator->IsType(i, OBJECT_TYPE::CLIENT)) {
                if (i != user_id)
                {
                        Netmgr.GetMediatorMgr()->Find(user_id)->SetIsMoving(false);
                        cout << "\t\t\Disconnect" << endl;
                        m_pSendMgr->Send_Leave_Packet(i, user_id);
                    
                }
            }
        }
    }
    
    pUser->SetStatus(OBJSTATUS::ST_FREE);
    CAST_CLIENT(pUser)->GetViewList().clear();
    pUser->GetLock().unlock();

    closesocket(pUser->GetSocket());
    pUser->SetSocket_Zero();
    // database�ڸ� 
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
             if (m_pMediator->IsType(id, OBJECT_TYPE::MONSTER))
             {
                 cout << "enter game" << endl;
                 WakeUp_Monster(id);
             }/*else if (IsNpc(id))
                 WakeUp_NPC(id);*/
         }
         tempLock.lock();
        if (m_pMediator->IsType(id, OBJECT_TYPE::CLIENT))
        {

                m_pSendMgr->Send_Enter_Packet(id, user_id);
            
        }

            m_pSendMgr->Send_Enter_Packet(user_id, id);
        
        tempLock.unlock();
    }

    if (m_pMediator->IsType(user_id, OBJECT_TYPE::CLIENT)) pUser->SetStatus(OBJSTATUS::ST_ACTIVE);
}

void CNetMgr::Process_Packet(const uShort& user_id, char* buf)
{
    switch (buf[1]) {
    case CS_MOVE: {
        cs_packet_move* packet = reinterpret_cast<cs_packet_move*>(buf);
        tempLock.lock();
        m_pMediator->Find(user_id)->SetIsMoving(true);
        CAST_CLIENT(m_pMediator->Find(user_id))->SetIsRefresh(true);
        CAST_CLIENT(m_pMediator->Find(user_id))->SetDir(packet->dir);
        m_pMediator->Find(user_id)->SetClientTime(packet->move_time);
        m_pMediator->Find(user_id)->SetSpeed(packet->speed);
        m_pMediator->Find(user_id)->SetHalfRTT(packet->Start);
        m_pMediator->Find(user_id)->SetDirV(packet->DirVec);
        m_pMediator->Find(user_id)->SetDeadReckoningPacket(packet);
        //cout << "\t\t\t��Ŷ���� ���� localvec : " << packet->localVec.x << ", " << packet->localVec.z << endl;
        ////

        Do_Move(user_id, packet->dir, packet->localVec, packet->rotateY);
        tempLock.unlock();

    }
                break;
    case CS_STOP:
    {
        cs_packet_stop* packet = reinterpret_cast<cs_packet_stop*>(buf);
        tempLock.lock();
        m_pMediator->Find(packet->id)->SetIsMoving(false);
   
        Do_Stop(packet->id, m_pMediator->Find(packet->id)->GetIsMoving());
        tempLock.unlock();
    }
    break;
    case CS_LOGIN: {
        cs_packet_login* packet = reinterpret_cast<cs_packet_login*>(buf) ;
        ////cout << "send loginok packet" << endl;
        m_pSendMgr->Send_LoginOK_Packet(user_id);
        m_pMediator->ReckonerAdd(user_id);

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
        tempLock.lock();
        cout << m_pMediator->Find(packet->id)->GetLocalPosVector().x << ", "
            << m_pMediator->Find(packet->id)->GetLocalPosVector().z << endl;
        unordered_set<uShort> new_viewList = g_QuadTree.search(m_pMediator->Find(packet->id));
        tempLock.unlock();
        cout << "-------------" << endl;
        cout << user_id << "���� ��Ŷ ����   " << (int)packet->aniType << endl;
        cout << "View List Size = " << new_viewList.size() << endl;
        if (packet->aniType == MONSTER_ANI_TYPE::DEAD)
        {
            int i = 0;
       }
        cout << "-------------" << endl;
        for (auto& user : new_viewList)
        {
            if (m_pMediator->IsType(user, OBJECT_TYPE::CLIENT))
            {
                m_pMediator->Find(packet->id)->SetIsMoving(packet->isMoving);
                
                if (MONSTER_ANI_TYPE::ATTACK == packet->aniType)
                {
                    //cout << "\t\t\t\t " << user_id << endl;
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
            //cout << packet->vPos.x << " - " << packet->vPos.y << " - " << packet->vPos.z << " - " << endl;
            m_pSendMgr->Send_ItemDelete_Packet(obj, packet->vPos);

        }
    }
    break;

    case CS_MONSTERDIR:
    {
        cs_pcaket_MonsterDir* packet = reinterpret_cast<cs_pcaket_MonsterDir*>(buf);
        Vector3 temp = packet->dir;
        tempLock.lock();
        //cout << "\t\t\t\t\t**********POS***********" << endl;
        //cout << packet->pos.x << ",  " << packet->pos.z << endl;

        //cout << m_pMediator->Find(packet->id)->GetLocalPosVector().x << ", " << m_pMediator->Find(packet->id)->GetLocalPosVector().z << endl;
        //cout << "\t\t\t\t\t*********DIR*********" << endl;
        //cout << packet->dir.x << ",  " << packet->dir.y << ",  " << packet->dir.z << endl;
        //cout << m_pMediator->Find(packet->id)->GetDirVector().x << ",  " << m_pMediator->Find(packet->id)->GetDirVector().y << ", " << m_pMediator->Find(packet->id)->GetDirVector().z << endl;

        //cout << "\t\t\t\t\t*-------------------" << endl;
        //cout << "\t\t\t\t\t*-------------------" << endl;
        m_pMediator->Find(packet->id)->SetDirV(temp);
        if (!CAST_MONSTER(m_pMediator->Find(packet->id))->GetIsDir())
        {
            cout << "��Ŷ�ް���" << endl;
            CAST_MONSTER(m_pMediator->Find(packet->id))->SetIsDir(true);
        }
        tempLock.unlock();
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
                tempLock.lock();
                m_pSendMgr->Send_Attack_Effect(obj, packet->pos);
                tempLock.unlock();
            }
        }


    }
    break;
    case CS_BOSS_STATE:
    {
        cs_pcaket_Boss_State* packet = reinterpret_cast<cs_pcaket_Boss_State*>(buf);
        unordered_set<uShort> new_viewList = g_QuadTree.search(m_pMediator->Find(packet->id));
        for (auto& obj : new_viewList)
        {
            if (m_pMediator->IsType(obj, OBJECT_TYPE::CLIENT))
            {
                tempLock.lock();
                m_pSendMgr->Send_Boss_State_Packet(obj, packet->id, packet->aniState, packet->attackstate);
                tempLock.unlock();
            }
        }

    }
    break;
    case CS_BOSS_TURN:
    {
        cs_pcaket_Boss_Turn* packet = reinterpret_cast<cs_pcaket_Boss_Turn*>(buf);
        unordered_set<uShort> new_viewList = g_QuadTree.search(m_pMediator->Find(packet->id));
        for (auto& obj : new_viewList)
        {
            if (m_pMediator->IsType(obj, OBJECT_TYPE::CLIENT))
            {
                tempLock.lock();
                m_pSendMgr->Send_Boss_Turn_Packet(obj, packet->id,packet->rotate);
                tempLock.unlock();
            }
        }
    }
    break;
    default:
        //cout << "Unknown Packet Type Error!\n";
        //cout << (int)buf[1] << endl;
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
            uShort user_id = -1;
            uShort i;
            for (i = 0; i < MAX_USER; ++i) {
                m_pMediator->Find( i)->GetLock().lock();
                if (m_pMediator->Find( i)->GetStatus() == OBJSTATUS::ST_FREE) {
                    m_pMediator->Find( i)->SetStatus(OBJSTATUS::ST_ALLOC);
                    user_id = i;
                    //
                    m_pMediator->Find( i)->GetLock().unlock();
                    break;
                }
                m_pMediator->Find( i)->GetLock().unlock();
            }

            SOCKET c_socket = exover->c_socket;

            if (MAX_USER == i) {
                //cout << "Max user limit exceeded.\n";
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
                // 0623�뼮
                g_QuadTree.Insert(pClient);

                ////////////////////////////////////////////////////////
                pClient->SetPosV(
                    (float)(rand() % 1000)+2000.f, // ���� real float
                    (float)(2800.f),
                    (float)(rand() % 1000) + 1000.f);

                //cout << pClient->GetLocalPosVector().x << ", " << pClient->GetLocalPosVector().z << endl;
                
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
        case ENUMOP::OP_RAMDON_MOVE_MONSTER:
        {
            if (m_pMediator->Find(user_id) == nullptr)break;
            if (!m_pMediator->IsType(user_id, OBJECT_TYPE::MONSTER))break;
            if (m_pMediator->Count(user_id) == 0)break;
            m_pMediator->MonsterReckonerAdd(user_id);
            Random_Move_Monster(user_id);
            bool keep_alive = false;
            //active�� �÷��̾ �ֺ��� ������ ��� �����α�
            unordered_set<uShort> view = g_QuadTree.search(CBoundary(Netmgr.GetMediatorMgr()->Find(user_id)));
            for (auto& id : view)
            {
                if (m_pMediator->IsType(id, OBJECT_TYPE::CLIENT))
                    if (m_pMediator->Find(id)->GetStatus() == OBJSTATUS::ST_ACTIVE)
                    {
                        keep_alive = true;
                        cout << "MonsterMove ����" << endl;
                        char temp = (char)(rand() % 4);
                        CAST_MONSTER(m_pMediator->Find(user_id))->SetIsMoving(true);

                        CAST_MONSTER(m_pMediator->Find(user_id))->SetDir((MONSTER_AUTOMOVE_DIR)temp);
                        CAST_MONSTER(m_pMediator->Find(user_id))->SetIsDir(true);
                        if(user_id!=BOSS_ID)
                             m_pSendMgr->Send_Monster_Move_Packet(id, user_id, (char)CAST_MONSTER(m_pMediator->Find(user_id))->GetDir());

                        break;
                    }
                        cout << "00000000000000000" << endl;
            }
            
            if (true == keep_alive) {
                if (user_id < 1010)
                    Add_Timer(user_id, ENUMOP::OP_RAMDON_MOVE_MONSTER, system_clock::now() + (seconds)(rand() % 5 + 5));
                else
                    Add_Timer(user_id, ENUMOP::OP_RAMDON_MOVE_MONSTER, system_clock::now() + (seconds)(rand() % 7 + 3));

            }
            else {
                m_pMediator->Find(user_id)->SetStatus(OBJSTATUS::ST_SLEEP);
                CAST_MONSTER(m_pMediator->Find(user_id))->SetDir(MONSTER_AUTOMOVE_DIR::AUTO);
                m_pMediator->Delete_MonsterReckoner(user_id);
                CAST_MONSTER(m_pMediator->Find(user_id))->SetIsMoving(false);
                CAST_MONSTER(m_pMediator->Find(user_id))->SetIsDir(false);
                //for auto viewand send packet;
            }
            //������ ���� �ƹ��� ������ SLEEP���� ����α� 
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
            for (auto& reckoner : m_pMediator->GetReckonerList())
            {
                CGameObject* obj = nullptr;
                Vector3 objPos;

                if (m_pMediator->Find(reckoner)->GetDeadReckoningPacket() == nullptr)continue;
                obj = m_pMediator->Find(reckoner);
                objPos = obj->GetLocalPosVector();
                obj->GetLock().lock();
                if (obj->GetIsMoving())
                {
                    unordered_set<uShort> old_viewList = g_QuadTree.search(CBoundary(m_pMediator->Find(reckoner)));
                    obj->SetRotateY(obj->GetPacketRotateY());// ���ص��Ǵ°�?
                    if (obj->GetLocalPosVector().x < 0 || objPos.z < 0 || objPos.x>15000 || objPos.z>15000)
                    {
                        objPos;
                        old_viewList;

                        float tempf = obj->GetPacketRotateY();
                    }

                    g_QuadTree.Delete(obj);
                    obj->SetPosV(obj->GetLocalPosVector() + obj->GetPacketDirVec() * obj->GetSpeed() * (DeltaTime));
                    //cout << reckoner << "�� ���� Pos : : " << obj->GetLocalPosVector().x << " -- " << obj->GetLocalPosVector().z << "\t DT:  " << DeltaTime << endl;

                    g_QuadTree.Insert(obj);
                    unordered_set<uShort> new_viewList = g_QuadTree.search(CBoundary(m_pMediator->Find(reckoner)));

                    if (obj->GetLocalPosVector().x < 0 || objPos.z < 0 || objPos.x>15000 || objPos.z>15000)
                    {
                        objPos;
                        old_viewList;
                        new_viewList;
                        float tempf = obj->GetPacketRotateY();
                    }
                    {
                        /*            if(m_pMediator->ReckonerSize() == 1)
                                        cout << "reckoner size : " << m_pMediator->ReckonerSize() << endl;
                                    if (old_viewList.size() == 1)
                                        cout << "old_viewList size : " << m_pMediator->ReckonerSize() << endl;
                                    if (new_viewList.size() == 1)
                                        cout << "new_viewList size : " << m_pMediator->ReckonerSize() << endl;*/
                        for (auto& ob : new_viewList) //�þ߿� ���� ���� ��ü ����
                        {
                            if (ob == reckoner)continue;
                            if (0 == old_viewList.count(ob)) // ���� ���� ���̵�
                            {
                                tempLock.lock();

                                m_pSendMgr->Send_Enter_Packet(reckoner, ob); // user���� ob enter 
                                if (m_pMediator->IsType(ob, OBJECT_TYPE::CLIENT))
                                {

                                    //Netmgr.GetMediatorMgr()->Find(reckoner)->SetIsMoving(false);// �뼮 �ʿ��Ѱ�?
                                    m_pSendMgr->Send_Enter_Packet(ob, reckoner);

                                }
                                tempLock.unlock();

                            }
                            else // new, old �Ѵ� ������ 
                            {
                                if (m_pMediator->IsType(ob, OBJECT_TYPE::CLIENT))
                                {

                                    //cout << "�������� �ִ� ���̵�" << endl;
                                    //cout << "--------------------" << endl;
                                    if (CAST_CLIENT(obj)->GetIsRefresh())
                                        m_pSendMgr->Send_Move_Packet(ob, reckoner, CAST_CLIENT(obj)->GetDir());// idle �̰� �ʿ������ �� ���� ������ 

                                }
                            }
                        }
                        for (auto& ob : old_viewList)
                        {
                            if (ob == reckoner)continue;
                            if (new_viewList.count(ob) == 0)
                            {
                                tempLock.lock();

                                m_pMediator->Find(ob)->SetIsMoving(false);// �뼮 �ʿ��Ѱ�?
                                cout << "********************" << endl;
                                cout << "********************" << endl;
                                cout << "Reckoner Move" << endl;
                                m_pSendMgr->Send_Leave_Packet(reckoner, ob);
                                if (m_pMediator->IsType(ob, OBJECT_TYPE::CLIENT))
                                {

                                    Netmgr.GetMediatorMgr()->Find(reckoner)->SetIsMoving(false);// �뼮 �ʿ��Ѱ�?
                                    m_pSendMgr->Send_Leave_Packet(ob, reckoner);

                                }
                                tempLock.unlock();
                            }

                        }
                        tempLock.lock();
                        CAST_CLIENT(obj)->SetIsRefresh(false);
                        tempLock.unlock();


                        // ����ȭ ��Ŷ �Ͻ����� 
                        CAST_CLIENT(obj)->CountRefreshPacketCnt(DeltaTime);
                        if (CAST_CLIENT(obj)->GetRefreshPacketCnt() > 5.f)
                        {
                            cout << "\t\t\t����ȭ ��Ŷ" << endl;
                            for (auto& id : g_QuadTree.search(CBoundary(m_pMediator->Find(reckoner))))
                                if (m_pMediator->IsType(id, OBJECT_TYPE::CLIENT))
                                {
                                    m_pSendMgr->Send_Move_Packet(id, reckoner, CAST_CLIENT(obj)->GetDir());
                                }
                            CAST_CLIENT(obj)->SetRefreshPacketCnt_Zero();
                        }
                    }
                }
                obj->GetLock().unlock();

            }
        }

        if (m_pMediator->MonsterReckonerSize() != 0)
        {

            for (auto& monster : m_pMediator->GetMonsterReckonerList())
            {
                unordered_set<uShort> old_viewList;
                unordered_set<uShort> new_viewList;
                CGameObject* MonsterObj = nullptr;
                Vector3 monsterPos;
                MONSTER_AUTOMOVE_DIR monsterDir;
                bool ismoving = false;
                float speed = 100.f;

                if (m_pMediator->Find(monster) == nullptr)continue;
                if (m_pMediator->MonsterReckonerCount(monster) == 0)continue;
                if (!m_pMediator->Find(monster)->GetIsMoving())continue;

                MonsterObj = m_pMediator->Find(monster);
                monsterPos = m_pMediator->Find(monster)->GetLocalPosVector();
                monsterDir = CAST_MONSTER(m_pMediator->Find(monster))->GetDir();
                ismoving = m_pMediator->Find(monster)->GetIsMoving();
                if (monster != BOSS_ID)
                {
                    if (ismoving)
                    {
                        tempLock.lock();
                        old_viewList = g_QuadTree.search(CBoundary(m_pMediator->Find(monster)));
                        g_QuadTree.Delete(m_pMediator->Find(monster));
                        monsterPos += speed * DT * m_pMediator->Find(monster)->GetDirVector();
                        if (monster == BOSS_ID)
                        {
                            cout << "DT : " << DT << "    **    " << m_pMediator->Find(monster)->GetDirVector().x << ", " << m_pMediator->Find(monster)->GetDirVector().z << endl;
                            cout << monsterPos.x << ", " << monsterPos.z << endl;
                        }
                        m_pMediator->Find(monster)->SetPosV(monsterPos);
                        g_QuadTree.Insert(m_pMediator->Find(monster));
                        new_viewList = g_QuadTree.search(CBoundary(m_pMediator->Find(monster)));
                        tempLock.unlock();

                        for (auto& id : new_viewList)
                        {
                            if (old_viewList.count(id) == 0)
                            {
                                if (m_pMediator->IsType(id, OBJECT_TYPE::CLIENT))
                                    m_pSendMgr->Send_Enter_Packet(id, monster);
                            }
                        }
                        for (auto& id : old_viewList)
                        {
                            if (new_viewList.count(id) == 0)
                            {
                                if (m_pMediator->IsType(id, OBJECT_TYPE::CLIENT))
                                {
                                    CAST_MONSTER(m_pMediator->Find(monster))->SetIsMoving(false);

                                    /*        cout << "********************" << endl;
                                            cout << "********************" << endl;
                                            cout << "Monster Reckoner Move" << endl;*/
                                    m_pSendMgr->Send_Leave_Packet(id, monster);
                                }
                            }
                        }
                    }




                    if (ismoving && CAST_MONSTER(m_pMediator->Find(monster))->GetIsDir())
                    {

                        cout << "\t\t\t\t    Process packet////////////////////////" << endl;
                        for (auto& user : old_viewList) {
                            if (new_viewList.count(user) != 0)
                            {
                                if (m_pMediator->IsType(user, OBJECT_TYPE::CLIENT))
                                {
                                    if (m_pMediator->IsType(monster, OBJECT_TYPE::BOSS))
                                    {
                                        m_pSendMgr->Send_Monster_Move_Packet(user, monster, (char)CAST_MONSTER(m_pMediator->Find(monster))->GetDir());

                                    }
                                    else
                                        m_pSendMgr->Send_Monster_Move_Packet(user, monster, (char)CAST_MONSTER(m_pMediator->Find(monster))->GetDir());
                                }
                            }

                            // ����ȭ �κ� �Ͻ�����
                            /*CAST_MONSTER(m_pMediator->Find(monster))->CountRefreshPacketCnt(DeltaTime);
                            if (CAST_MONSTER(m_pMediator->Find(monster))->GetRefreshPacketCnt() > 1.f)
                            {
                                m_pSendMgr->Send_Monster_Move_Packet(user, monster, (char)CAST_MONSTER(m_pMediator->Find(monster))->GetDir());
                                CAST_MONSTER(m_pMediator->Find(monster))->SetRefreshPacketCnt_Zero();
                            }*/


                        }
                        CAST_MONSTER(m_pMediator->Find(monster))->SetIsDir(false);
                    }

                }
                else
                {
                    if (ismoving)
                    {
                        tempLock.lock();
                        if (m_pMediator->Find(monster) == nullptr)continue;
                        old_viewList = g_QuadTree.search(CBoundary(m_pMediator->Find(monster)));
                        if (m_pMediator->Find(monster) == nullptr)continue;
                        g_QuadTree.Delete(m_pMediator->Find(monster));
                        if (monster == BOSS_ID)
                        {
                            cout << "DT : " << DT << "    **    " << m_pMediator->Find(monster)->GetDirVector().x << ", " << m_pMediator->Find(monster)->GetDirVector().z << endl;
                            cout << monsterPos.x << ", " << monsterPos.z << endl;
                        }
                        monsterPos += speed * DT * m_pMediator->Find(monster)->GetDirVector();
                        //cout << monsterPos.x << ", " << monsterPos.z << endl;
                        m_pMediator->Find(monster)->SetPosV(monsterPos);
                        g_QuadTree.Insert(m_pMediator->Find(monster));
                        new_viewList = g_QuadTree.search(CBoundary(m_pMediator->Find(monster)));
                        tempLock.unlock();
                    }
                    for (auto& id : new_viewList)
                    {
                        if (old_viewList.count(id) != 0)
                        {

                        }
                        else
                        {
                            if (m_pMediator->IsType(id, OBJECT_TYPE::CLIENT))
                                m_pSendMgr->Send_Enter_Packet(id, monster);
                            cout << "Boss Mobb Enter Packet" << endl;

                        }

                    }
                    for (auto& id : old_viewList)
                    {
                        if (new_viewList.count(id) == 0)
                        {
                            if (m_pMediator->IsType(id, OBJECT_TYPE::CLIENT))
                            {
                                CAST_MONSTER(m_pMediator->Find(monster))->SetIsMoving(false);

                                /*        cout << "********************" << endl;
                                        cout << "********************" << endl;
                                        cout << "Monster Reckoner Move" << endl;*/
                                m_pSendMgr->Send_Leave_Packet(id, monster);
                            }
                        }
                    }


                    if (ismoving && CAST_MONSTER(m_pMediator->Find(monster))->GetIsDir())
                    {

                        for (auto& user : old_viewList) {
                            if (new_viewList.count(user) != 0)
                            {
                                if (m_pMediator->IsType(user, OBJECT_TYPE::CLIENT))
                                {
                                    if (m_pMediator->IsType(monster, OBJECT_TYPE::BOSS))
                                    {
                                        m_pSendMgr->Send_Monster_Move_Packet(user, monster, (char)CAST_MONSTER(m_pMediator->Find(monster))->GetDir());

                                    }
                                    else
                                        m_pSendMgr->Send_Monster_Move_Packet(user, monster, (char)CAST_MONSTER(m_pMediator->Find(monster))->GetDir());
                                }
                            }

                            // ����ȭ �κ� �Ͻ�����
                            /*CAST_MONSTER(m_pMediator->Find(monster))->CountRefreshPacketCnt(DeltaTime);
                            if (CAST_MONSTER(m_pMediator->Find(monster))->GetRefreshPacketCnt() > 1.f)
                            {
                                m_pSendMgr->Send_Monster_Move_Packet(user, monster, (char)CAST_MONSTER(m_pMediator->Find(monster))->GetDir());
                                CAST_MONSTER(m_pMediator->Find(monster))->SetRefreshPacketCnt_Zero();
                            }*/


                        }
                        CAST_MONSTER(m_pMediator->Find(monster))->SetIsDir(false);
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
        //CAS(�޸�, expected, update)
        //    �޸��� ���� expected�� update�� �ٲٰ�, true ����
        //    �޸��� ���� expected�� �ƴϸ� false ����
        //    WAIT FREE�� �����ϴ� �˰���

        Add_Timer(id, ENUMOP::OP_RAMDON_MOVE_MONSTER, system_clock::now());

       /* if (id < 1001)
            Add_Timer(id, ENUMOP::OP_RAMDON_MOVE_MONSTER, system_clock::now() + (seconds)(rand() % 4 + 5));
        else
            Add_Timer(id, ENUMOP::OP_RAMDON_MOVE_MONSTER, system_clock::now() + (seconds)(rand() % 2 + 3));*/
    }
}