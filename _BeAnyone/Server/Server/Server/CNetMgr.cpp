#include "stdafx.h"
#include "CNetMgr.h"
float packetTimeCnt = 0.f;
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
    Vector3 MonsterPos = MonsterObj->GetLocalPosVector();
    unordered_set<uShort> old_viewList;
    unordered_set<uShort> new_viewList;
    for (auto& old_ids : g_QuadTree.search(CBoundary(m_pMediator->Find(Monster_id))))
    {
        if (m_pMediator->IsType(old_ids, OBJECT_TYPE::CLIENT))
            old_viewList.insert(old_ids);
    }
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(0, 3);
    MONSTER_AUTOMOVE_DIR dir = (MONSTER_AUTOMOVE_DIR)(dis(gen));
    
    CAST_MONSTER(MonsterObj)->SetDir(dir);
    CAST_MONSTER(MonsterObj)->SetIsMoving(true);

    for (auto& new_ids : g_QuadTree.search(CBoundary(m_pMediator->Find(Monster_id))))
    {
        if (m_pMediator->IsType(new_ids, OBJECT_TYPE::CLIENT))
            new_viewList.insert(new_ids);
    }

    // new viewlist�� �˾ƾ��� ..
    for (auto& id : new_viewList)
    {
        if (old_viewList.count(id) != 0)
        {
            m_pSendMgr->Send_Monster_Move_Packet(id, Monster_id, (char)dir);
        }
    }

    for (auto& id : old_viewList)
    {
        if (new_viewList.count(id) == 0)
        {
            dynamic_cast<CClient*>(m_pMediator->Find(id))->GetViewList().erase(Monster_id);
            m_pSendMgr->Send_Monster_Move_Packet(id, Monster_id, (char)MONSTER_AUTOMOVE_DIR::IDLE);
            m_pSendMgr->Send_Leave_Packet(id, Monster_id);
        }
    }
}

void CNetMgr::Random_Move_NPC(const uShort& NPC_id)
{
    CGameObject* NpcObj = m_pMediator->Find(NPC_id);
    Vector3 MonsterPos = NpcObj->GetLocalPosVector();
    unordered_set<uShort> old_viewList;
    unordered_set<uShort> new_viewList;
    NPC_AUTOMOVE_DIR CurrentDir = CAST_NPC(NpcObj)->GetAutoDir();
    switch (CurrentDir)
    {
    case NPC_AUTOMOVE_DIR::FRONT:CAST_NPC(NpcObj)->SetAutoDir(NPC_AUTOMOVE_DIR::BACK); break;
    case NPC_AUTOMOVE_DIR::BACK:CAST_NPC(NpcObj)->SetAutoDir(NPC_AUTOMOVE_DIR::IDLE); break;
    case NPC_AUTOMOVE_DIR::IDLE:CAST_NPC(NpcObj)->SetAutoDir(NPC_AUTOMOVE_DIR::FRONT); break;
    }

    for (auto& old_ids : g_QuadTree.search(CBoundary(m_pMediator->Find(NPC_id))))
    {
        if (m_pMediator->IsType(old_ids, OBJECT_TYPE::CLIENT))
            old_viewList.insert(old_ids);
    }

    CAST_NPC(NpcObj)->SetIsMoving(true);

    for (auto& new_ids : g_QuadTree.search(CBoundary(m_pMediator->Find(NPC_id))))
    {
        if (m_pMediator->IsType(new_ids, OBJECT_TYPE::CLIENT))
            new_viewList.insert(new_ids);
    }

    // new viewlist�� �˾ƾ��� ..
    for (auto& id : new_viewList)
    {
        if (old_viewList.count(id) != 0)
        {
            m_pSendMgr->Send_Monster_Move_Packet(id, NPC_id, (char)CAST_NPC(NpcObj)->GetAutoDir());
        }
    }

    for (auto& id : old_viewList)
    {
        if (new_viewList.count(id) == 0)
        {
            dynamic_cast<CClient*>(m_pMediator->Find(id))->GetViewList().erase(NPC_id);
            m_pSendMgr->Send_Npc_Move_Packet(id, NPC_id, (char)NPC_AUTOMOVE_DIR::IDLE);
            m_pSendMgr->Send_Leave_Packet(id, NPC_id);
        }
    }
}

void CNetMgr::Do_Attack(const uShort& attacker, const uShort& victim)
{
    CMonster* monster = dynamic_cast<CMonster*>(m_pMediator->Find(victim));
    unordered_set<uShort> vSectors = g_QuadTree.search(CBoundary(m_pMediator->Find(attacker)));
    if (monster->GetHP() - 12 >= 0)
    {
        monster->SetHP(monster->GetHP() - 12);
        for (auto& clientID : vSectors)
        {
                m_pSendMgr->Send_Attacked_Packet_Monster(clientID, victim);
        }
    }
    else
    {
            monster->SetHP(0);
        for (auto& clientID : vSectors)
        {
            m_pSendMgr->Send_Attacked_Packet_Monster(clientID, victim);
            m_pSendMgr->Send_Leave_Packet(clientID, victim, true);
        }   
    }
}
void CNetMgr::Kill_Monster(const uShort& monster_id)
{
    if (m_pMediator->Count(monster_id) != 0)
    {
        unordered_set<uShort> vSectors = g_QuadTree.search(CBoundary(m_pMediator->Find(monster_id)));

        g_QuadTree.Delete(m_pMediator->Find(monster_id));
        m_pMediator->Delete_Obj(monster_id);
    }
}
void CNetMgr::Do_Move(const uShort& user_id, const char& dir, Vector3& localVec, const float& rotateY)
{
    CClient* pClient = CAST_CLIENT(m_pMediator->Find(user_id));
    unordered_set<uShort> old_viewList = pClient->GetViewList();
    unordered_set<uShort> new_viewList;
    unordered_set<uShort>vSectors = g_QuadTree.search(CBoundary(m_pMediator->Find(user_id)));

    pClient->SetPosV(localVec);
    pClient->SetRotateY(rotateY);

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
                    else if(m_pMediator->IsType(user, OBJECT_TYPE::NPC))
                        WakeUp_NPC(user);
                }
            }
            new_viewList.insert(user);
        }
    }

    for (auto& ob : new_viewList) //�þ߿� ���� ���� ��ü ����
    {
        if (0 == old_viewList.count(ob)) // ���� ���� ���̵�
        {
            pClient->GetViewList().insert(ob);
            m_pSendMgr->Send_Enter_Packet(user_id, ob);
            if (m_pMediator->IsType(ob, OBJECT_TYPE::CLIENT) && ob != user_id)
            {
                if (CAST_CLIENT(m_pMediator->Find(ob))->GetViewList().count(user_id) == 0)
                {
                    CAST_CLIENT(m_pMediator->Find(ob))->GetViewList().insert(user_id);
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
            if (m_pMediator->IsType(ob, OBJECT_TYPE::CLIENT) && ob != user_id)
            {
                if (CAST_CLIENT(m_pMediator->Find(ob))->GetViewList().count(user_id) == 0)
                {
                    CAST_CLIENT(m_pMediator->Find(ob))->GetViewList().insert(user_id);
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

            if (m_pMediator->IsType(ob, OBJECT_TYPE::CLIENT))
            {
                if (CAST_CLIENT(m_pMediator->Find(ob))->GetViewList().count(user_id) != 0)
                {
                    CAST_CLIENT(m_pMediator->Find(ob))->GetViewList().erase(user_id);
                    m_pSendMgr->Send_Leave_Packet(ob, user_id);
                }
            }
        }
    }
}

void CNetMgr::Do_Stop(const uShort& user_id, const bool& isMoving)
{
    CClient* pClient = CAST_CLIENT(m_pMediator->Find(user_id));

    unordered_set<uShort> old_viewList = pClient->GetViewList();

    pClient->SetIsMoving(isMoving);
    unordered_set<uShort>vSectors = g_QuadTree.search(CBoundary(m_pMediator->Find(user_id)));

    for (auto& ob : vSectors)
    {
        //�þ߿� ���� ���� ��ü ����

        if (0 == old_viewList.count(ob)) // ���� ���� ���̵�
        {
            pClient->GetViewList().insert(ob);
            m_pSendMgr->Send_Enter_Packet(user_id, ob);
            if (m_pMediator->IsType(ob, OBJECT_TYPE::CLIENT) && ob != user_id)
            {
                if (CAST_CLIENT(m_pMediator->Find(ob))->GetViewList().count(user_id) == 0)
                {
                    CAST_CLIENT(m_pMediator->Find(ob))->GetViewList().insert(user_id);
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
            if (m_pMediator->IsType(ob, OBJECT_TYPE::CLIENT) && ob != user_id)
            {
                if (CAST_CLIENT(m_pMediator->Find(ob))->GetViewList().count(user_id) == 0)
                {
                    CAST_CLIENT(m_pMediator->Find(ob))->GetViewList().insert(user_id);
                    m_pSendMgr->Send_Enter_Packet(ob, user_id);
                }
                else
                {
                    m_pSendMgr->Send_Stop_Packet(ob, user_id, isMoving);  // ���⼭ �� ����
                }
            }
        }
    }
    for (auto& ob : old_viewList)
    {
        if (vSectors.count(ob) == 0)
        {
            pClient->GetViewList().erase(ob);
            m_pSendMgr->Send_Leave_Packet(user_id, ob);

            if (m_pMediator->IsType(ob, OBJECT_TYPE::CLIENT))
            {
                if (CAST_CLIENT(m_pMediator->Find(ob))->GetViewList().count(user_id) != 0)
                {
                    CAST_CLIENT(m_pMediator->Find(ob))->GetViewList().erase(user_id);
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
             if(m_pMediator->IsType(id, OBJECT_TYPE::MONSTER))
                 WakeUp_Monster(id);
             else if (m_pMediator->IsType(id, OBJECT_TYPE::NPC))
                 WakeUp_NPC(id);
         }
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
    }

    if (m_pMediator->IsType(user_id, OBJECT_TYPE::CLIENT)) pUser->SetStatus(OBJSTATUS::ST_ACTIVE);
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
        cs_packet_login* packet = reinterpret_cast<cs_packet_login*>(buf) ;
        cout << "send loginok packet" << endl;
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
    case CS_MONSTER_DEAD:
    {
        cs_packet_MonsterDead* packet = reinterpret_cast<cs_packet_MonsterDead*>(buf);
        if(m_pMediator->Count(packet->id)!=0)
            Kill_Monster(packet->id);
    }
    break;
    case CS_ATTACK_ANIMATION:
    {
        cs_packet_AttackAni* packet = reinterpret_cast<cs_packet_AttackAni*>(buf);
        if (m_pMediator->Count(packet->id) == 0)break;
        CClient* monster = CAST_CLIENT(m_pMediator->Find(packet->id));
        unordered_set<uShort> new_viewList = g_QuadTree.search(m_pMediator->Find(user_id));
        for (auto& user : new_viewList)
        {
            if (m_pMediator->IsType(user, OBJECT_TYPE::CLIENT))
                m_pSendMgr->Send_Attack_Animation_Packet(user, packet->id, packet->isAttack);
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
            {
                cout << "monster animation user id -> " << user << endl;
                cout << "monster animation monster id -> " << packet->id << endl;
                cout << "monster animation ani type -> " << (int)packet->aniType << endl;
                m_pSendMgr->Send_Monster_Animation_Packet(packet->id, user, packet->aniType);
            }
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
            uShort user_id = -1;
            uShort i;
            for (i = 0; i < MAX_USER; ++i) {
                m_pMediator->Find(i)->GetLock().lock();
                if (m_pMediator->Find(i)->GetStatus() == OBJSTATUS::ST_FREE) {
                    m_pMediator->Find(i)->SetStatus(OBJSTATUS::ST_ALLOC);
                    user_id = i;
                    //
                    m_pMediator->Find(i)->GetLock().unlock();
                    // ����

                    break;
                }
                m_pMediator->Find(i)->GetLock().unlock();
            }

            SOCKET c_socket = exover->c_socket;

            if (MAX_USER == i) {
                cout << "Max user limit exceeded.\n";
                closesocket(c_socket);
            }

            if (-1 == user_id)
                closesocket(c_socket);
            else {
                CClient* pClient = CAST_CLIENT(m_pMediator->Find(user_id));

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
                    (float)(rand() % 1000), // ���� real float
                    (float)(290.f),
                    (float)(rand() % 1000));

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
            if (!m_pMediator->IsType(user_id, OBJECT_TYPE::NPC))break;
            if (m_pMediator->Count(user_id) == 0)break;
            m_pMediator->NPCReckonerAdd(user_id);
            Random_Move_NPC(user_id);
            bool keep_alive = false;
            //active�� �÷��̾ �ֺ��� ������ ��� �����α�
            unordered_set<uShort> view = g_QuadTree.search(CBoundary(Netmgr.GetMediatorMgr()->Find(user_id)));
            for (auto& id : view)
            {
                if (m_pMediator->IsType(id, OBJECT_TYPE::CLIENT))
                    if (m_pMediator->Find(id)->GetStatus() == OBJSTATUS::ST_ACTIVE)
                    {
                        keep_alive = true;
                        break;
                    }
            }

            if (true == keep_alive) Add_Timer(user_id, ENUMOP::OP_RAMDON_MOVE_NPC, system_clock::now() + monsterAutoMoveTimer);
            else {
                m_pMediator->Find(user_id)->SetStatus(OBJSTATUS::ST_SLEEP);
                m_pMediator->Delete_MonsterReckoner(user_id);
                CAST_NPC(m_pMediator->Find(user_id))->SetIsMoving(false);
                //for auto viewand send packet;
            }
            //������ ���� �ƹ��� ������ SLEEP���� ����α� 
            delete exover;
        }
        break;
        case ENUMOP::OP_RAMDON_MOVE_MONSTER:
        {
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
                        break;
                    }
            }
            
            if (true == keep_alive) Add_Timer(user_id, ENUMOP::OP_RAMDON_MOVE_MONSTER, system_clock::now() + monsterAutoMoveTimer);
            else {
                m_pMediator->Find(user_id)->SetStatus(OBJSTATUS::ST_SLEEP);
                m_pMediator->Delete_MonsterReckoner(user_id);
                CAST_MONSTER(m_pMediator->Find(user_id))->SetIsMoving(false);
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
                    obj->SetPosV(obj->GetLocalPosVector() + drmPacket->DirVec * obj->GetSpeed() * DeltaTime);

                    if (CAST_CLIENT(obj)->GetRefreshPacketCnt() > 2.f)
                    {
                        CAST_CLIENT(obj)->CountRefreshPacketCnt(DeltaTime);
                        cout << reckoner << "�� �÷��̾��� ���巹Ŀ�� ����ȭ ��Ŷ ����" << endl;
                        m_pSendMgr->Send_Move_Packet(reckoner, reckoner, drmPacket->dir);
                        CAST_CLIENT(obj)->SetRefreshPacketCnt_Zero();
                    }
                }
            }
        }

        if (m_pMediator->MonsterReckonerSize() != 0)
        {
            Vector3 monsterPos;
            for (auto& monster : m_pMediator->GetMonsterReckonerList())
            {
                monsterPos = m_pMediator->Find(monster)->GetLocalPosVector();
                if (CAST_MONSTER(m_pMediator->Find(monster))->GetIsMoving())
                {
                    switch (CAST_MONSTER(m_pMediator->Find(monster))->GetDir())
                    {
                    case MONSTER_AUTOMOVE_DIR::FRONT:
                        monsterPos.z += 20.f * DT;
                        break;
                    case MONSTER_AUTOMOVE_DIR::BACK:
                        monsterPos.z -= 20.f * DT;
                        break;
                    case MONSTER_AUTOMOVE_DIR::LEFT:
                        monsterPos.x -= 20.f * DT;
                        break;
                    case MONSTER_AUTOMOVE_DIR::RIGHT:
                        monsterPos.x += 20.f * DT;
                        break;
                    case MONSTER_AUTOMOVE_DIR::AUTO:
                        break;
                    case MONSTER_AUTOMOVE_DIR::IDLE:
                        break;
                    default:
                        break;
                    }
                    CAST_MONSTER(m_pMediator->Find(monster))->SetPosV(monsterPos);
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
        Add_Timer(id, OP_RAMDON_MOVE_NPC, system_clock::now() + monsterAutoMoveTimer);
    }
}

void CNetMgr::WakeUp_Monster(const uShort& id)
{
    int status = OBJSTATUS::ST_SLEEP;
    if (CAS((int*)(&(m_pMediator->Find(id)->GetStatus())), status, (int)ST_ACTIVE))
    {
        //CAS(�޸�, expected, update)
        //    �޸��� ���� expected�� update�� �ٲٰ�, true ����
        //    �޸��� ���� expected�� �ƴϸ� false ����
        //    WAIT FREE�� �����ϴ� �˰���
        Add_Timer(id, OP_RAMDON_MOVE_MONSTER, system_clock::now() + monsterAutoMoveTimer);
    }
}