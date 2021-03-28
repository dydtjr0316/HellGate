#include "stdafx.h"
#include "CNetMgr.h"

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


bool CNetMgr::is_near( const int& p1, const int& p2)
{
    int dist = (Find(p1)->GetX() - Find(p2)->GetX()) * (Find(p1)->GetX() - Find(p2)->GetX());
    dist += (Find(p1)->GetY() - Find(p2)->GetY()) * (Find(p1)->GetY() - Find(p2)->GetY());

    return dist <= (VIEW_LIMIT * VIEW_LIMIT);
}


void CNetMgr::Add(CGameObject* pObj,  const int& id)
{
    if (pObj != nullptr)
        g_Object.emplace(id, pObj);
    
}

CGameObject* CNetMgr::Find(const int& id)
{
    if (g_Object.count(id) != 0)
    {
        return  g_Object.find(id)->second;
    }
    return nullptr;
}

void CNetMgr::Delete_Obj(  const int& id)
{
    if (g_Object.count(id) != 0)
    {
        SafeDelete(g_Object.find(id)->second);
        g_Object.erase(id);
    }
}

const size_t CNetMgr::Count(const int& id)
{
    return g_Object.count(id);
}

const size_t CNetMgr::Size()
{
    return g_Object.size();
}

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

    cout << exover->wsabuf.len << endl;

    // ?????왜 터지노
    Find(id)->GetLock().lock();
    WSASend(Find(id)->GetSocket(), &exover->wsabuf, 1, NULL, 0,
        &exover->over, NULL);
    Find( id)->GetLock().unlock();
    
}

void CNetMgr::Send_LevelUP_Packet(const int& id)
{
    sc_packet_level_up p;
    p.id = id;
    p.size = sizeof(sc_packet_level_up);
    p.type = SC_PACKET_LEVEL_UP;

    
    p.max_hp = dynamic_cast<CClient*>(Find( id))->GetMaxHP();
    p.level = dynamic_cast<CClient*>(Find( id))->GetLevel();
    p.attack_damage = dynamic_cast<CClient*>(Find(id))->GetAttackDamage();
    p.hp = dynamic_cast<CClient*>(Find(id))->GetHP();
    p.exp = dynamic_cast<CClient*>(Find( id))->GetEXP();
    p.max_exp = dynamic_cast<CClient*>(Find( id))->GetMaxEXP();;

    Send_Packet( id, &p);
}

void CNetMgr::Send_Attacked_Packet_Monster(const int& monster_id)
{
    sc_packet_attack p;
    p.id = monster_id;
    p.size = sizeof(sc_packet_attack);
    p.type = SC_PACKET_ATTACK;
    p.hp = dynamic_cast<CMonster*>(Find(monster_id))->GetHP();
    Send_Packet(monster_id ,&p);
}

void CNetMgr::Send_Attacked_Packet_Client(const int& client_id)
{
    sc_packet_attack p;
    p.id = client_id;
    p.size = sizeof(sc_packet_attack);
    p.type = SC_PACKET_ATTACK;
    p.hp = dynamic_cast<CClient*>(Find(client_id))->GetHP();
    Send_Packet(client_id, &p);
}

void CNetMgr::Send_ID_Packet(const int& user_id)
{
    sc_packet_id p;
    CClient* pClient = dynamic_cast<CClient*>(Find(user_id));
    p.id = user_id;
    p.size = sizeof(sc_packet_id);
    p.type = SC_PACKET_ID;

    Send_Packet(user_id, &p);
}

void CNetMgr::Send_LoginOK_Packet(const int& user_id)
{
    sc_packet_login_ok p;
    CClient* pClient = dynamic_cast<CClient*>(Find(user_id));
    p.id = user_id;
    p.size = sizeof(sc_packet_login_ok);
    p.type = SC_PACKET_LOGIN_OK;

    p.exp = pClient->GetEXP();
    p.hp = pClient->GetHP();
    p.level = pClient->GetLevel();
    p.x = pClient->GetX();
    p.y = pClient->GetY();

    p.iMax_exp = pClient->GetMaxEXP();
    p.Attack_Damage = pClient->GetAttackDamage();

    Send_Packet(user_id, &p);
}

void CNetMgr::Send_Enter_Packet( const int& user_id,  const int& other_id)
{
    sc_packet_enter p;
    p.id = other_id;
    p.size = sizeof(p);
    p.type = SC_PACKET_ENTER;
    p.x = Find(other_id)->GetX();
    p.y = Find(other_id)->GetY();

    strcpy_s(p.name, Find(other_id)->GetName());    // data race???
    p.o_type = O_PLAYER;

    Send_Packet(user_id, &p);
}

void CNetMgr::Send_Leave_Packet( const int& user_id, const int& other_id)
{
    sc_packet_leave p;
    p.id = other_id;
    p.size = sizeof(p);
    p.type = SC_PACKET_LEAVE;

    Send_Packet(user_id, &p);
}



void CNetMgr::Send_Move_Packet( const int& user_id,  const int& mover_id)
{
    sc_packet_move p;
    p.id = mover_id;
    p.size = sizeof(p);
    p.type = SC_PACKET_MOVE;
    p.x = Find(mover_id)->GetX();
    p.y = Find(mover_id)->GetY();
    p.z = Find(mover_id)->GetZ();
    cout << p.z << endl;

    p.move_time = Find(mover_id)->GetClientTime();

    Send_Packet(user_id, &p);
}

void CNetMgr::Random_Move_NPC(const int& id)
{
    CGameObject* NPCObj = Find( id);
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
    
    NPCObj->Change_Sector(oldSector);

    for (auto& vSec : NPCObj->Search_Sector()) {
        for (auto& clientID : vSec)
        {
            if (!IsClient(clientID))continue;

            CGameObject* ClientObj = Find(clientID);
            if (ClientObj->GetStatus() != OBJSTATUS::ST_ACTIVE)continue;

            if (is_near(clientID, id))
            {
                ClientObj->GetLock().lock();
                if (0 == dynamic_cast<CClient*>(ClientObj)->GetViewList().count(id))  // map으로 바꾸기
                {
                    //cout << "뷰리스트에 몬스터 없어" << endl;
                    ClientObj->GetLock().unlock();
                    dynamic_cast<CClient*>(ClientObj)->GetViewList().insert(id);
                    Send_Enter_Packet(clientID, id);
                }
                else
                {
                    //cout << "뷰리스트에 npc 있어" << endl;
                    ClientObj->GetLock().unlock();
                    Send_Move_Packet(clientID, id);
                }
            }
            else
            {
                ClientObj->GetLock().lock();
                if (0 == dynamic_cast<CClient*>(ClientObj)->GetViewList().count(id))
                {
                    ClientObj->GetLock().unlock();
                }
                else
                {
                    ClientObj->GetLock().unlock();
                    dynamic_cast<CClient*>(ClientObj)->GetViewList().erase(id);
                    Send_Leave_Packet(clientID, id);
                }

            }
        }
    }
}

void CNetMgr::Random_Move_Monster(const int& id)
{
    CGameObject* MonsterObj = Find( id);

    int x = MonsterObj->GetX();
    int y = MonsterObj->GetY();

    _tSector oldSector = MonsterObj->GetSector();

    switch (rand() % 4)
    {
    case 0: if (x > 0)x--; break;
    case 1: if (x < WORLD_WIDTH - 1)x++; break;
    case 2: if (y > 0)y--; break;
    case 3: if (y < WORLD_HEIGHT - 1)y++; break;
    }

    MonsterObj->SetX(x);
    MonsterObj->SetY(y);

    MonsterObj->Change_Sector(oldSector);

    for (auto& vSec : MonsterObj->Search_Sector()) {
        for (auto& sec : vSec)
        {
            if (!IsClient(sec))continue;

            CClient* pClient = dynamic_cast<CClient*>(Find(sec));
            if (pClient->GetStatus() != OBJSTATUS::ST_ACTIVE)continue;

            if (is_near(sec, id))    // is near이 필요한가?
            {
                pClient->GetLock().lock();
                if (0 == pClient->GetViewList().count(id))  // 뷰리스트에 몬스터가 없다면 엔터 패킷
                {
                    pClient->GetLock().unlock();
                    pClient->GetViewList().insert(id);
                    Send_Enter_Packet(sec, id);
                }
                else                                        // 뷰리스트에 몬스터가 있으면 무브 패킷
                {
                    pClient->GetLock().unlock();
                    Send_Move_Packet(sec, id);
                }
            }
            else
            {
                pClient->GetLock().lock();
                if (0 == pClient->GetViewList().count(id))
                {
                    pClient->GetLock().unlock();
                    
                }
                else
                {
                    pClient->GetLock().unlock();
                    pClient->GetViewList().erase(id);
                    Send_Leave_Packet(sec, id);
                }
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
}

void CNetMgr::Do_Move(const int& user_id, const int& dir)
{
    CClient* pClient = dynamic_cast<CClient*>(Find(user_id));

    unordered_set<int> old_viewList = pClient->GetViewList();


    int x = pClient->GetX();
    int y = pClient->GetY();
    int z = pClient->GetZ();

    _tSector oldSector = pClient->GetSector();


    switch (dir)
    {
    case MV_UP: if (y > 0) y--; break;
    case MV_DOWN: if (y < (WORLD_HEIGHT - 1)) y++; break;
    case MV_LEFT: if (x > 0) x--; break;
    case MV_RIGHT: if (x < (WORLD_WIDTH - 1)) x++; break;
    case MV_FRONT: if (z >0) z--; break;
    case MV_BACK: if (z < (WORLD_WIDTH - 1)) z++; break;
        
    default:
        cout << "Unknown Direction from Client move packet!\n";
        DebugBreak();
        exit(-1);
    }

    pClient->SetX(x);
    pClient->SetY(y);
    pClient->SetZ(z);

    pClient->Change_Sector(oldSector);

    unordered_set<int> new_viewList;

    Send_Move_Packet(user_id, user_id);

    vector<unordered_set<int>> vSectors = pClient->Search_Sector();
    
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
                        if (Find(user)->GetStatus() == OBJSTATUS::ST_SLEEP)
                        {
                            if (IsMonster(user))
                                WakeUp_Monster(user);
                            else 
                            {
                                //cout << "do move 함수 호출" << endl;
                                //cout << Find(user)->GetStatus() << endl;
                                WakeUp_NPC(user);
                            }
                        }
                    }
                    else
                    {
                        //if (Find(user)->GetStatus() != ST_ACTIVE)continue;
                    }
                        new_viewList.insert(user);
                }
            }
        }
    }

    for (auto& ob : new_viewList)
    {
        //시야에 새로 들어온 객체 구분
        
        if (0 == old_viewList.count(ob)) // 새로 들어온 아이디
        {
            pClient->GetViewList().insert(ob);
            Send_Enter_Packet(user_id, ob);
            if (IsClient(ob))
            {
                if (dynamic_cast<CClient*>(Find(ob))->GetViewList().count(user_id) == 0)
                {
                    dynamic_cast<CClient*>(Find(ob))->GetViewList().insert(user_id);
                    Send_Enter_Packet(ob, user_id);
                }
                else
                    Send_Move_Packet(ob, user_id);
            }
        }
        else // 이전에도 있던 아이디 
        {
            if (IsClient(ob))
            {
                if (dynamic_cast<CClient*>(Find(ob))->GetViewList().count(user_id) == 0)
                {
                    dynamic_cast<CClient*>(Find(ob))->GetViewList().insert(user_id);
                    Send_Enter_Packet(ob, user_id);
                }
                else
                    Send_Move_Packet(ob, user_id);
            }
        }
    }
    for (auto& ob : old_viewList)
    {
        if (new_viewList.count(ob) == 0)
        {
            pClient->GetViewList().erase(ob);
            Send_Leave_Packet(user_id, ob);

            if (IsClient(ob))
            {
                if (dynamic_cast<CClient*>(Find(ob))->GetViewList().count(user_id) != 0)
                {
                    dynamic_cast<CClient*>(Find(ob))->GetViewList().erase(user_id);
                    Send_Leave_Packet(ob, user_id);
                }
            }
        }
    }


}

void CNetMgr::Disconnect(const int& user_id)
{
    CGameObject* pUser = Find( user_id);
    pUser->SetStatus(OBJSTATUS::ST_ALLOC);

    Send_Leave_Packet( user_id, user_id);
    pUser->GetLock().lock();
    for (int i = 0; i < MAX_USER; ++i)
    {
        if (Find( i)->GetStatus() == OBJSTATUS::ST_ACTIVE)
        {
            if (i != user_id)
            {
                if(dynamic_cast<CClient*>(Find( i))->GetViewList().count(user_id))
                {
                    dynamic_cast<CClient*>(Find( i))->GetViewList().erase(user_id);
                    Send_Leave_Packet( i, user_id);
                }
            }
        }
    }

    pUser->SetStatus(OBJSTATUS::ST_FREE);
    dynamic_cast<CClient*>(pUser)->GetViewList().clear();
    pUser->GetLock().unlock();

    closesocket(pUser->GetSocket());
    pUser->SetSocket_Zero();
    // database자리 
}

void CNetMgr::Enter_Game(const int& user_id, char name[])
{
    CClient* pUser = dynamic_cast<CClient*>(Find(user_id));
    pUser->GetLock().lock();

    pUser->SetName(name);
    pUser->GetLock().unlock();
    pUser->GetName()[MAX_ID_LEN] = NULL;
    //Send_ID_Packet(user_id);

    Send_LoginOK_Packet(user_id);
    pUser->Insert_Sector();

    unordered_set<int> new_viewList;

    vector<unordered_set<int>> vSectors = pUser->Search_Sector();

    for (auto& vSec : vSectors)
    {
        for (auto& id : vSec) {
            if (Find(id)->GetID() == Find(user_id)->GetID())continue;
            if (Find(id)->GetStatus() != OBJSTATUS::ST_ACTIVE)continue;
            if (is_near(user_id, id))
            {
                if (Find(id)->GetStatus() == OBJSTATUS::ST_SLEEP)
                {
                    if(IsMonster(id))
                        WakeUp_Monster(id);
                    else if (IsNpc(id))
                        WakeUp_NPC(id);
                }
                if (IsClient(id))
                {
                    if (dynamic_cast<CClient*>(Find(id))->GetViewList().count(user_id) == 0)
                    {
                        dynamic_cast<CClient*>(Find(id))->GetViewList().insert(user_id);
                        Send_Enter_Packet(id, user_id);
                    }
                }
                if (pUser->GetViewList().count(id) == 0)
                {
                    pUser->GetViewList().insert(id);
                    Send_Enter_Packet(user_id, id);
                }
            }
        }
    }

    pUser->SetStatus(OBJSTATUS::ST_ACTIVE);
}

void CNetMgr::Process_Packet(const int& user_id, char* buf)
{
    switch (buf[1]) {
    case CS_LOGIN: {
        cs_packet_login* packet = reinterpret_cast<cs_packet_login*>(buf);
        cout << packet->name << endl;
        
        // 여기에 send id 해보기
        Enter_Game(user_id, packet->name);
    }
                 break;
    case CS_MOVE: {
        cs_packet_move* packet = reinterpret_cast<cs_packet_move*>(buf);
        Find( user_id)->SetClientTime(packet->move_time);
        Do_Move(user_id, packet->direction);

    }
                break;
    case CS_ATTACK:
    {
        cs_packet_attack* packet = reinterpret_cast<cs_packet_attack*>(buf);
        Do_Attack(user_id);
    }
    break;
    default:
        cout << "Unknown Packet Type Error!\n";
        DebugBreak();
        exit(-1);
    }
}

void CNetMgr::Recv_Packet_Construct(const int& user_id, const int& io_byte)
{
    CGameObject* pObj = dynamic_cast<CGameObject*>(Find( user_id));
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
        //pObj->Insert_Sector(); 엔터 게임에서 넣어줌
        Add(pObj,  i);
    }
}
void CNetMgr::Init_Monster()
{
    CGameObject* pObj = nullptr;
    for (int i = START_MONSTER; i < END_MONSTER; ++i) {
        // 좌표 어캐할지 생각
        pObj = new CMonster;
        pObj->SetX(rand() % WORLD_WIDTH);
        pObj->SetY(rand() % WORLD_HEIGHT);
        pObj->SetID(i);
        pObj->SetStatus(OBJSTATUS::ST_SLEEP);
        pObj->Insert_Sector();
        Add(pObj, i);
  
    }
}
void CNetMgr::Init_NPC()
{
    CGameObject* pObj = nullptr;

    for (int i = START_NPC; i < END_NPC; ++i)
    {
        pObj = new CNPC;
        // 좌표 어캐 할지 생각
        pObj->SetX(rand() % WORLD_WIDTH);
        pObj->SetY(rand() % WORLD_HEIGHT);
        pObj->SetID(i);
        char npc_name[50];
        sprintf_s(npc_name, "N%d", i);
        pObj->SetName(npc_name);
        pObj->SetStatus(OBJSTATUS::ST_SLEEP);
        pObj->Insert_Sector();
        Add(pObj, i);

    }
}

bool CNetMgr::IsClient(const int& id)
{
    if (id >= 0 && id < MAX_USER)
        return true;
    else
        return false;
}

bool CNetMgr::IsMonster(const int& id)
{
    if (id >= START_MONSTER && id < END_MONSTER)
        return true;
    else
        return false;
}

bool CNetMgr::IsNpc(const int& id)
{
    if (id >= START_NPC && id < END_NPC)
        return true;
    else
        return false;
}

void CNetMgr::Add_Timer(const int& obj_id, const int& status, system_clock::time_point t)
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
        int user_id = static_cast<int>(key);
      
        CClient* pUser = dynamic_cast<CClient*>(Find( user_id));
        switch (exover->op) {
        case ENUMOP::OP_RECV:
            if (0 == io_byte) Disconnect(user_id);
            else {
                CClient* pClient = dynamic_cast<CClient*>(Find( user_id));
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
            int user_id = -1;
            int i;
            for (i = 0; i < MAX_USER; ++i) {
                Find( i)->GetLock().lock();
                if (Find( i)->GetStatus() == OBJSTATUS::ST_FREE) {
                    Find( i)->SetStatus(OBJSTATUS::ST_ALLOC);
                    user_id = i;
                    //
                    Find( i)->GetLock().unlock();
                    // 여기

                    break;
                }
                Find( i)->GetLock().unlock();
            }

            SOCKET c_socket = exover->c_socket;

            if (MAX_USER == i) {
                cout << "Max user limit exceeded.\n";
                closesocket(c_socket);
            }

            if (-1 == user_id)
                closesocket(c_socket);
            else {
                CClient* pClient = dynamic_cast<CClient*>(Find( user_id));

                pClient->SetPrev_Size(0);
                pClient->GetExover().op = ENUMOP::OP_RECV;
                pClient->ZeroMemory_recv_over();
                pClient->GetExover().wsabuf.buf = pClient->GetExover().io_buf;
                pClient->GetExover().wsabuf.len = MAX_BUF_SIZE;
                pClient->SetSocket(c_socket);
                pClient->SetX(rand() % WORLD_WIDTH);
                pClient->SetY(rand() % WORLD_HEIGHT);
                pClient->SetFirstXY(pClient->GetX(), pClient->GetY());

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
                /*   DWORD flags = 0;
                   WSARecv(c_socket, &nc.m_recv_over.wsabuf, 1, NULL, &flags, &nc.m_recv_over.over, NULL);
                */
            }

            c_socket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
            exover->c_socket = c_socket;
            ZeroMemory(&exover->over, sizeof(exover->over));
            AcceptEx(l_socket, c_socket, exover->io_buf, NULL,
                sizeof(sockaddr_in) + 16, sizeof(sockaddr_in) + 16, NULL, &exover->over);
        
            Send_ID_Packet(user_id);
        }
        break;
        case ENUMOP::OP_RAMDON_MOVE_NPC:
        {
            Random_Move_NPC(user_id); 
            bool keep_alive = false;
            //active인 플레이어가 주변에 있으면 계속 깨워두기
            for (auto& vSec : Find(user_id)->Search_Sector())
            {
                for (auto& id : vSec)
                {
                    if (IsClient(id))
                    {
                        if (is_near(user_id, id))
                        {
                            if (Find(id)->GetStatus() == OBJSTATUS::ST_ACTIVE)
                            {
                                keep_alive = true; 
                                break;
                            }
                        }
                    }
                }
           }
            if (true == keep_alive) Add_Timer(user_id, ENUMOP::OP_RAMDON_MOVE_NPC, system_clock::now() + 1s);
            else Find( user_id)->SetStatus(OBJSTATUS::ST_SLEEP);
            //주위에 이제 아무도 없으면 SLEEP으로 멈춰두기 
            delete exover;

        }
        break;
        case ENUMOP::OP_RAMDON_MOVE_MONSTER:
        {
            Random_Move_Monster(user_id);
            bool keep_alive = false;
            //active인 플레이어가 주변에 있으면 계속 깨워두기
            for (auto& vSec : Find(user_id)->Search_Sector())
            {
                for (auto& id : vSec)
                {
                    if (IsClient(id))
                        if (is_near(user_id, id))
                            if (Find(id)->GetStatus() == OBJSTATUS::ST_ACTIVE)
                            {
                                keep_alive = true;
                                break;
                            }
                }
            }
            if (true == keep_alive) Add_Timer(user_id, ENUMOP::OP_RAMDON_MOVE_MONSTER, system_clock::now() + 1s);
            else Find( user_id)->SetStatus(OBJSTATUS::ST_SLEEP);
            //주위에 이제 아무도 없으면 SLEEP으로 멈춰두기 
            delete exover;
        }
        break;
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

               // cout << "timer worker -> objid -> " << ev.obj_id << endl;

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
void CNetMgr::WakeUp_NPC(const int& id)
{
    int status = OBJSTATUS::ST_SLEEP;
   // cout << "wakeup npc-> " << id << endl;
    //cout << Find(id)->GetX() << " , " << Find(id)->GetY() << endl;
    if (CAS((int*)(&(Find(id)->GetStatus())), status, (int)ST_ACTIVE))
    {
        Add_Timer(id, OP_RAMDON_MOVE_NPC, system_clock::now() + 1s);
    }
    cout<<Find(id)->GetStatus()<<endl;
}

void CNetMgr::WakeUp_Monster(const int& id)
{
    int status = OBJSTATUS::ST_SLEEP;
   // cout << "wakeup monster-> " << id << endl;
    if (CAS((int*)(&(Find(id)->GetStatus())), status, (int)ST_ACTIVE))
    {
        Add_Timer(id, OP_RAMDON_MOVE_MONSTER, system_clock::now() + 1s);
    }
}
