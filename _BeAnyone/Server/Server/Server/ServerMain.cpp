#include "stdafx.h"
#include "CNetMgr.h"
#include "CClient.h"
#include "CMonster.h"
#include "CNPC.h"
#include "CDataBase.h"
// mutex
mutex timer_lock;
mutex wakeup_lock;
priority_queue<event_type>		timer_queue;
CNetMgr Netmgr;
CQuadTree g_QuadTree(CBoundary(WORLD_WIDTH/2, WORLD_HEIGHT/2, WORLD_WIDTH / 2, WORLD_HEIGHT / 2), MAX_PLAYER_IN_NODE);
SOCKET g_listenSocket;
HANDLE g_IocpHandle;
seconds monsterAutoMoveTimer = 5s;


int main()
{
    std::wcout.imbue(locale("Korean"));
    WSADATA WSAData;
    int start = WSAStartup(MAKEWORD(2, 2), &WSAData);
    g_IocpHandle = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, 0);
    g_listenSocket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
    CreateIoCompletionPort(reinterpret_cast<HANDLE>(g_listenSocket), g_IocpHandle, 10000, 0);

    SOCKADDR_IN s_address;
    memset(&s_address, 0, sizeof(s_address));
    s_address.sin_family = AF_INET;
    s_address.sin_port = htons(SERVER_PORT);
    s_address.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
    ::bind(g_listenSocket, reinterpret_cast<sockaddr*>(&s_address), sizeof(s_address));
    listen(g_listenSocket, SOMAXCONN);

    SOCKET c_socket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
    EXOVER accept_over;

    accept_over.op = ENUMOP::OP_ACCEPT;
    accept_over.c_socket = c_socket;
    accept_over.wsabuf.len = static_cast<int>(c_socket);
    ZeroMemory(&accept_over.over, sizeof(accept_over.over));
    AcceptEx(g_listenSocket, c_socket, accept_over.io_buf, 0, 32, 32, NULL, &accept_over.over);

    CTimeMgr::GetInst()->init();
    
    // 지우기
    Netmgr.GetMediatorMgr()->InitObject();

    cout << "**********Play************" << endl;

    thread time_thread(&CNetMgr::Timer_Worker, &Netmgr);
    vector <thread> worker_threads;
    for (int i = 0; i < 4; ++i) worker_threads.emplace_back(thread(&CNetMgr::Worker_Thread, &Netmgr));
    worker_threads.emplace_back(thread(&CNetMgr::Processing_Thead, &Netmgr));
    for (auto& th : worker_threads) th.join();
    
    time_thread.join();

    // 지우기
    closesocket(g_listenSocket);
    WSACleanup();
}