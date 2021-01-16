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

vector<unordered_multiset<int>> CNetMgr::Search_Sector(const int& user_id)
{
    vector<unordered_multiset<int>> vSectors;

    CGameObject* pUser = Find(OBJID::CLIENT, user_id);

    int x1 = (pUser->GetX() - VIEW_LIMIT) / SECTOR_COL_Length;
    int z1 = (pUser->GetY() - VIEW_LIMIT) / SECTOR_ROW_Length;

    int x2 = (pUser->GetY() + VIEW_LIMIT) / SECTOR_COL_Length;
    int z2 = (pUser->GetY() + VIEW_LIMIT) / SECTOR_ROW_Length;

    // 여기 int 형 으로 그냥 맵 반환해서 int 형 변수로 섹터 검샋해도될거같은대ㅔ?

    if (x1 == x2 && z1 == z2) // 섹터 하나 검색
    {
            vSectors.push_back(g_Sector[CLIENT][x1][z1]);
    }
    else if (x1 == x2) // 상*하 검색
    {

            vSectors.push_back(g_Sector[CLIENT][x1][z1]);
            vSectors.push_back(g_Sector[CLIENT][x1][z2]);

    }
    else if (z1 == z2) // 좌*우 검색
    {

            vSectors.push_back(g_Sector[CLIENT][x1][z1]);
            vSectors.push_back(g_Sector[CLIENT][x2][z1]);
        
    }
    else // 사방향 검색
    {

        vSectors.push_back(g_Sector[CLIENT][x1][z2]);
        vSectors.push_back(g_Sector[CLIENT][x2][z2]);
        vSectors.push_back(g_Sector[CLIENT][x1][z1]);
        vSectors.push_back(g_Sector[CLIENT][x2][z1]);

    }

    return vSectors;
}

bool CNetMgr::is_near(const OBJID& p1eID, const int& p1,const OBJID& p2eID, const int& p2)
{
    int dist = (Find(p1eID, p1)->GetX() - Find(p2eID, p2)->GetX()) * (Find(p1eID, p1)->GetX() - Find(p2eID, p2)->GetX());
    dist += (Find(p1eID, p1)->GetY() - Find(p2eID, p2)->GetY()) * (Find(p1eID, p1)->GetY() - Find(p2eID, p2)->GetY());

    return dist <= (VIEW_LIMIT * VIEW_LIMIT);
}

void CNetMgr::Insert_Sector(const OBJID& eID, const int& id)
{
    CGameObject* pObj = Find(eID, id);
    pObj->SetSector(pObj->GetX() / SECTOR_ROW_Length, pObj->GetY() / SECTOR_COL_Length);
    g_Sector[eID][pObj->GetSector().x][pObj->GetSector().y].emplace(id);
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
        return  m_ObjLst[eID].find(id)->second;
    }
    cout << "null이라고???" << endl;
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
        if (is_near(OBJID::CLIENT, sec, OBJID::MONSTER, id))    // is near이 필요한가?
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
    unordered_set<int> old_viewList[OBJID::END];
    for (int i = 0; i < END; ++i)
    {
        old_viewList[i] = dynamic_cast<CClient*>(Find(OBJID::CLIENT, user_id))->GetViewList(OBJID(i));
    }

    CGameObject* pUser = Find(OBJID::CLIENT, user_id);

    int x = pUser->GetX();
    int y = pUser->GetY();

    switch (dir)
    {
    case MV_UP: if (y > 0) y--; break;
    case MV_DOWN: if (y < (WORLD_HEIGHT - 1)) y++; break;
    case MV_LEFT: if (x > 0) x--; break;
    case MV_RIGHT: if (x < (WORLD_WIDTH - 1)) x++; break;
    default:
        cout << "Unknown Direction from Client move packet!\n";
        DebugBreak();
        exit(-1);
    }

    pUser->SetX(x);
    pUser->SetY(y);
    unordered_set<int> new_viewList[OBJID::END];

    Send_Move_Packet(OBJID::CLIENT, user_id, OBJID::CLIENT, user_id);

    vector<unordered_multiset<int>> vSectors = Search_Sector(user_id);

    for (auto& vSec : vSectors)
    {
        if (vSec.size() != 0)
        {
            for (int i = 0; i < OBJID::END; ++i)
            {
                for (auto& user : vSec)
                {
                    if (Find((OBJID)i, user)->GetStatus() == ENUM_STATUS::ST_SLEEP)
                    {
                        if (i == OBJID::MONSTER)
                        {
                            WakeUp_Monster(Find(OBJID::MONSTER, user)->GetID());
                        }
                        else if (i == OBJID::NPC)
                        {
                            WakeUp_Monster(Find(OBJID::NPC, user)->GetID());
                        }
                    }
                    else
                        continue;
                    if (is_near(OBJID::CLIENT, user_id, OBJID(i), user))
                    {
                        new_viewList[i].insert(user);
                    }
                }
            }

        }
    }
    for (int i = 0; i < OBJID::END; ++i) {
        for (int ob : new_viewList[i])
        {
            //시야에 새로 들어온 객체 구분
            if (0 == old_viewList[i].count(ob))
            {
                dynamic_cast<CClient*>(pUser)->GetViewList(OBJID(i)).insert(ob);
                Send_Enter_Packet(OBJID::CLIENT, user_id, OBJID(i), ob);
                if (OBJID(i) == OBJID::CLIENT)
                {
                    if (dynamic_cast<CClient*>(Find(OBJID::CLIENT, ob))->GetViewList(OBJID::CLIENT).count(user_id) == 0)
                    {
                        dynamic_cast<CClient*>(Find(OBJID::CLIENT, ob))->GetViewList(OBJID::CLIENT).insert(user_id);
                        Send_Enter_Packet(OBJID::CLIENT, ob, OBJID::CLIENT, user_id);
                    }
                    else
                        Send_Move_Packet(OBJID::CLIENT, ob, OBJID::CLIENT, user_id);
                }
            }
            else
            {
                if (OBJID(i) == OBJID::CLIENT)
                {
                    if (dynamic_cast<CClient*>(Find(OBJID::CLIENT, ob))->GetViewList(OBJID::CLIENT).count(user_id) == 0)
                    {
                        dynamic_cast<CClient*>(Find(OBJID::CLIENT, ob))->GetViewList(OBJID::CLIENT).insert(user_id);
                        Send_Enter_Packet(OBJID::CLIENT, ob, OBJID::CLIENT, user_id);
                    }
                    else
                        Send_Move_Packet(OBJID::CLIENT, ob, OBJID::CLIENT, user_id);
                }
            }
        }
        
    }
    for (int i = 0; i < OBJID::END; ++i) 
    {
        for (int ob : old_viewList[i])
        {
            if (new_viewList[i].count(ob))
            {
                dynamic_cast<CClient*>(pUser)->GetViewList(OBJID(i)).erase(ob);
                if (OBJID(i) == OBJID::CLIENT)
                {
                    if (dynamic_cast<CClient*>(Find(OBJID::CLIENT, ob))->GetViewList(OBJID::CLIENT).count(user_id) != 0)
                    {
                        dynamic_cast<CClient*>(Find(OBJID::CLIENT, ob))->GetViewList(OBJID(i)).erase(user_id);
                        Send_Leave_Packet(OBJID::CLIENT, ob, user_id);
                    }
                }
            }
        }
    }
}

void CNetMgr::Disconnect(const int& user_id)
{
    CGameObject* pUser = Find(OBJID::CLIENT, user_id);
    pUser->SetStatus(ENUM_STATUS::ST_ALLOC);

    Send_Leave_Packet(OBJID::CLIENT, user_id, user_id);
    pUser->GetLock().lock();
    for (int i = 0; i < MAX_USER; ++i)
    {
        if (Find(OBJID::CLIENT, i)->GetStatus() == ENUM_STATUS::ST_ACTIVE)
        {
            if (i != user_id)
            {
                if(dynamic_cast<CClient*>(Find(OBJID::CLIENT, i))->GetViewList(OBJID::CLIENT).count(user_id))
                {
                    dynamic_cast<CClient*>(Find(OBJID::CLIENT, i))->GetViewList(OBJID::CLIENT).erase(user_id);
                    Send_Leave_Packet(OBJID::CLIENT, i, user_id);
                }
            }
        }
    }

    pUser->SetStatus(ENUM_STATUS::ST_FREE);
    dynamic_cast<CClient*>(pUser)->GetViewList(OBJID::CLIENT).clear();
    pUser->GetLock().unlock();

    closesocket(pUser->GetSocket());
    pUser->SetSocket_Zero();
    // database자리 
}

void CNetMgr::Enter_Game(const int& user_id, char name[])
{
    CGameObject* pUser = Find(OBJID::CLIENT, user_id);
    pUser->GetLock().lock();
    pUser->SetName(name);
    pUser->GetLock().unlock();
    pUser->GetName()[MAX_ID_LEN] = NULL;

    Send_LoginOK_Packet(user_id);
    Insert_Sector(OBJID::CLIENT, user_id);

    unordered_multiset<int> new_viewList[OBJID::END];

    vector<unordered_multiset<int>> vSectors = Search_Sector(user_id);

    for (int i = 0; i < OBJID::END; ++i) {
        for (auto& vSec : vSectors)
        {
            for (auto& id : vSec) {
                if (Find(OBJID(i), id)->GetStatus() != ENUM_STATUS::ST_ACTIVE)continue;
                if (is_near(OBJID::CLIENT, user_id, OBJID(i), id)) {
                    switch (OBJID(i))
                    {
                    case CLIENT:
                        if (dynamic_cast<CClient*>(Find(OBJID(i), id))->GetViewList(OBJID(i)).count(user_id))
                        {
                            dynamic_cast<CClient*>(Find(OBJID(i), id))->GetViewList(OBJID(i)).insert(user_id);
                            Send_Enter_Packet(OBJID(i), id, OBJID::CLIENT, user_id);
                        }
                        break;
                    case MONSTER:
                        if (Find(OBJID(i), id)->GetStatus() == ENUM_STATUS::ST_SLEEP)
                        {
                            WakeUp_Monster(Find(OBJID::MONSTER, id)->GetID());
                        }
                        break;
                    case NPC:
                        if (Find(OBJID(i), id)->GetStatus() == ENUM_STATUS::ST_SLEEP)
                        {
                            WakeUp_Monster(Find(OBJID::NPC, id)->GetID());
                        }

                        break;

                    }
                    // 여기 시발 다시한번 보기 좀 이상함

                    if (dynamic_cast<CClient*>(pUser)->GetViewList(OBJID::CLIENT).count(id) == 0)
                    {
                        dynamic_cast<CClient*>(pUser)->GetViewList(OBJID(i)).insert(id);
                        Send_Enter_Packet(OBJID::CLIENT, user_id, OBJID(i), id);
                    }
                }
            }
        }
    }
    pUser->SetStatus(ENUM_STATUS::ST_ACTIVE);
}

void CNetMgr::Process_Packet(const int& user_id, char* buf)
{
    switch (buf[1]) {
    case CS_LOGIN: {
        cs_packet_login* packet = reinterpret_cast<cs_packet_login*>(buf);
        Enter_Game(user_id, packet->name);
    }
                 break;
    case CS_MOVE: {
        cs_packet_move* packet = reinterpret_cast<cs_packet_move*>(buf);
        Find(OBJID::CLIENT, user_id)->SetTime(packet->move_time);
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
    CClient* pUser = dynamic_cast<CClient*>(Find(OBJID::CLIENT, user_id));
    //
    EXOVER r_o = (pUser->GetExover());

    int rest_byte = io_byte;
    char* p = r_o.io_buf;
    int packet_size = 0;
    if (0 != pUser->Get_Prev_Size()) packet_size = pUser->Get_Packet_buf()[0];
    while (rest_byte > 0) {
        if (0 == packet_size) packet_size = *p;
        if (packet_size <= rest_byte + pUser->Get_Prev_Size()) {
            memcpy(pUser->Get_Packet_buf() + pUser->Get_Prev_Size(), p, packet_size - pUser->Get_Prev_Size());
            p += packet_size - pUser->Get_Prev_Size();
            rest_byte -= packet_size - pUser->Get_Prev_Size();
            packet_size = 0;
            Process_Packet(user_id, pUser->Get_Packet_buf());
            pUser->SetPrev_Size(0);
        }
        else {
            memcpy(pUser->Get_Packet_buf() + pUser->Get_Prev_Size(), p, rest_byte);
            pUser->SetPrev_Size(pUser->Get_Prev_Size() + rest_byte);
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
        Add(pObj, OBJID::CLIENT, i);
        Find(OBJID::CLIENT, i)->SetID(i);
        Find(OBJID::CLIENT, i)->SetStatus(ENUM_STATUS::ST_FREE);
    }
}
void CNetMgr::Init_Monster()
{
    CGameObject* pObj = nullptr;

    for (int i = 0; i < MAX_MONSTER; ++i) {
        // 좌표 어캐할지 생각
        pObj = new CMonster;
        Add(pObj, OBJID::MONSTER, i);

        Find(OBJID::MONSTER, i)->SetX(rand() % WORLD_WIDTH);
        Find(OBJID::MONSTER, i)->SetY(rand() % WORLD_HEIGHT);
        Find(OBJID::MONSTER, i)->SetZ(rand() % WORLD_HEIGHT);
        Find(OBJID::MONSTER, i)->SetID(i);
        Find(OBJID::MONSTER, i)->SetStatus(ENUM_STATUS::ST_SLEEP);

        Insert_Sector(OBJID::MONSTER, i);

    }
}
void CNetMgr::Init_NPC()
{
    CGameObject* pObj = nullptr;

    for (int i = 0; i < MAX_NPC; ++i)
    {
        pObj = new CNPC;
        // 좌표 어캐 할지 생각
        Add(pObj, OBJID::NPC, i);

        Find(OBJID::NPC, i)->SetX(rand() % WORLD_WIDTH);
        Find(OBJID::NPC, i)->SetX(rand() % WORLD_WIDTH);
        Find(OBJID::NPC, i)->SetY(rand() % WORLD_HEIGHT);
        Find(OBJID::NPC, i)->SetZ(rand() % WORLD_HEIGHT);

        char npc_name[50];
        sprintf_s(npc_name, "N%d", i);
        dynamic_cast<CNPC*>(Find(OBJID::NPC, i))->SetName(npc_name);
        Find(OBJID::NPC, i)->SetStatus(ENUM_STATUS::ST_SLEEP);

        Insert_Sector(OBJID::NPC, i);
    }
}

void CNetMgr::Add_Timer(const int& obj_id, const ENUMOP& status, system_clock::time_point t)
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
        cout << user_id << endl;
      
        CClient* pUser = dynamic_cast<CClient*>(Find(CLIENT, user_id));
        switch (exover->op) {
        case ENUMOP::OP_RECV:
            if (0 == io_byte) Disconnect(user_id);
            else {
                CClient* pClient = dynamic_cast<CClient*>(Find(CLIENT, user_id));
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
                Find(OBJID::CLIENT, i)->GetLock().lock();
                if (Find(OBJID::CLIENT, i)->GetStatus() == ENUM_STATUS::ST_FREE) {
                    Find(OBJID::CLIENT, i)->SetStatus(ENUM_STATUS::ST_ALLOC);
                    user_id = i;
                    Find(OBJID::CLIENT, i)->GetLock().unlock();

                    break;
                }
                Find(OBJID::CLIENT, i)->GetLock().unlock();
            }

            SOCKET c_socket = exover->c_socket;

            if (MAX_USER == i) {
                cout << "Max user limit exceeded.\n";
                closesocket(c_socket);
            }

            if (-1 == user_id)
                closesocket(c_socket);
            else {
                CClient* pClient = dynamic_cast<CClient*>(Find(OBJID::CLIENT, user_id));

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
                if (pClient->GetStatus() == ENUM_STATUS::ST_ALLOC)
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
        }
        break;
        case ENUMOP::OP_RAMDON_MOVE_NPC:
        {
            Random_Move_NPC(user_id); 
            bool keep_alive = false;
            //active인 플레이어가 주변에 있으면 계속 깨워두기
            for (int i = 0; i < MAX_USER; ++i)
                if (true == is_near(OBJID::NPC, user_id, OBJID::CLIENT, i))
                    if (Find(OBJID::CLIENT, i)->GetStatus() == ENUM_STATUS::ST_ACTIVE)
                    {
                        keep_alive = true; break;
                    }
            if (true == keep_alive) Add_Timer(user_id, ENUMOP::OP_RAMDON_MOVE_NPC, system_clock::now() + 1s);
            else Find(OBJID::NPC, user_id)->SetStatus(ENUM_STATUS::ST_SLEEP);
            //주위에 이제 아무도 없으면 SLEEP으로 멈춰두기 
            delete exover;

        }
        break;
        case ENUMOP::OP_RAMDON_MOVE_MONSTER:
        {
            Random_Move_Monster(user_id);
            bool keep_alive = false;
            //active인 플레이어가 주변에 있으면 계속 깨워두기
            for (int i = 0; i < MAX_USER; ++i)
                if (true == is_near(OBJID::MONSTER, user_id, OBJID::CLIENT, i))
                    if (Find(OBJID::CLIENT, i)->GetStatus() == ENUM_STATUS::ST_ACTIVE)
                    {
                        keep_alive = true; break;
                    }
            if (true == keep_alive) Add_Timer(user_id, ENUMOP::OP_RAMDON_MOVE_MONSTER, system_clock::now() + 1s);
            else Find(OBJID::MONSTER, user_id)->SetStatus(ENUM_STATUS::ST_SLEEP);
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
    ENUM_STATUS status = ENUM_STATUS::ST_SLEEP;

    wakeup_lock.lock();
    Add_Timer(id, ENUMOP::OP_RAMDON_MOVE_NPC, system_clock::now() + 1s);
    wakeup_lock.unlock();
}

void CNetMgr::WakeUp_Monster(const int& id)
{
    ENUM_STATUS status = ENUM_STATUS::ST_SLEEP;

    wakeup_lock.lock();
    Add_Timer(id, ENUMOP::OP_RAMDON_MOVE_MONSTER, system_clock::now() + 1s);
    wakeup_lock.unlock();
}
