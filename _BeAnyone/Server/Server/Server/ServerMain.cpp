#include "stdafx.h"
#include "CNetMgr.h"
#include "CClient.h"
#include "CMonster.h"
#include "CNPC.h"
#include "CDataBase.h"

OBJLIST g_Object;
RECKONER g_Reckoner;
SOCKET l_socket;
HANDLE g_iocp;

// mutex
mutex timer_lock;
mutex wakeup_lock;

// sector ¼öÁ¤
unordered_set<uShort> g_Sector[SECTOR_ROW][SECTOR_COL];

priority_queue<event_type>		timer_queue;

CNetMgr Netmgr;




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

  
    Netmgr.Init_Client();
    Netmgr.Init_Monster();
    //Netmgr.Init_NPC();

    cout << "**********Play************" << endl;


    thread time_thread(&CNetMgr::Timer_Worker, &Netmgr);

    vector <thread> worker_threads;
    for (int i = 0; i < 4; ++i) worker_threads.emplace_back(thread(&CNetMgr::Worker_Thread, &Netmgr));
    worker_threads.emplace_back(thread(&CNetMgr::DeadReckoning_Thread, &Netmgr));
    for (auto& th : worker_threads) th.join();
    
    time_thread.join();

    closesocket(l_socket);
    WSACleanup();
}