#include "stdafx.h"

#include "CClient.h"
#include "CMonster.h"
#include "CNPC.h"
#include "CDataBase.h"

CClient g_clients[MAX_USER];
CMonster g_monsters[MAX_MONSTER];
CNPC g_npcs[MAX_NPC];
//CDataBase g_DataBase;
SOCKET l_socket;
HANDLE g_iocp;

// mutex
mutex timer_lock;

// sector 수정
unordered_set<int>				g_Sector[SECTOR_ROW][SECTOR_COL];
priority_queue<event_type>		timer_queue;

void show_error() {     // 에러 출력
    printf("error\n");
}

void error_display(const char* msg, int err_no)     // 에러 출력
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

bool CAS(int* addr, int exp, int update)        // cas 
{
    return atomic_compare_exchange_strong(reinterpret_cast<atomic_int*>(addr), &exp, update);
}

void is_near()
{
    // 깃 prac - simd 파일에 대충 구현해놓은 getlength 파일 있으니까 다듬어서 사용해보기 
}
void initialize_clients()
{
    for (int i = 0; i < MAX_USER; ++i) {
        g_clients[i].SetID(i);
        g_clients[i].SetStatus(ENUM_STATUS::ST_FREE);
    }
}
void initialize_Monster()
{
    for (int i = 0; i < MAX_MONSTER; ++i) {
        // 좌표 어캐할지 생각
        g_monsters[i].SetX(rand() % WORLD_WIDTH);
        g_monsters[i].SetY(rand() % WORLD_HEIGHT);
        g_monsters[i].SetZ(rand() % WORLD_HEIGHT);
        g_monsters[i].SetID(i);
        g_monsters[i].SetStatus(ENUM_STATUS::ST_SLEEP);
    }
}
void initialize_NPC()
{
    for (int i = 0; i < MAX_NPC; ++i)
    {
        // 좌표 어캐 할지 생각
        g_npcs[i].SetX(rand() % WORLD_WIDTH);
        g_npcs[i].SetY(rand() % WORLD_HEIGHT);
        g_npcs[i].SetZ(rand() % WORLD_HEIGHT);

        char npc_name[50];
        sprintf_s(npc_name, "N%d", i);
        g_npcs[i].SetName(npc_name);
        g_clients[i].SetStatus(ENUM_STATUS::ST_SLEEP);
    }
}

int main()
{
    std::wcout.imbue(locale("Korean"));

    WSADATA WSAData;
    WSAStartup(MAKEWORD(2, 2), &WSAData);
    g_iocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, 0);
    l_socket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
    CreateIoCompletionPort(reinterpret_cast<HANDLE>(l_socket), g_iocp, 999, 0);

    SOCKADDR_IN s_address;
    memset(&s_address, 0, sizeof(s_address));
    s_address.sin_family = AF_INET;
    s_address.sin_port = htons(SERVER_PORT);
    s_address.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
    ::bind(l_socket, reinterpret_cast<sockaddr*>(&s_address), sizeof(s_address));
    listen(l_socket, SOMAXCONN);

    SOCKET c_socket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
    EXOVER accept_over;

    accept_over.op = ENUMOP::OP_ACCEPT;
    accept_over.c_socket = c_socket;
    accept_over.wsabuf.len = static_cast<int>(c_socket);
    ZeroMemory(&accept_over.over, sizeof(accept_over.over));
    AcceptEx(l_socket, c_socket, accept_over.io_buf, 0, 32, 32, NULL, &accept_over.over);

    cout << "Initializing" << endl;
    initialize_clients();
    initialize_NPC();
    initialize_Monster();
    cout << "Initializing Finish" << endl;

  /*  thread time_thread{ timer_worker };

    vector <thread> worker_threads;
    for (int i = 0; i < 4; ++i) worker_threads.emplace_back(worker_thread);
    for (auto& th : worker_threads) th.join();
    time_thread.join();*/

    closesocket(l_socket);
    WSACleanup();
}