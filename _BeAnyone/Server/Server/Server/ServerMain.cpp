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


int main()
{
    Netmgr.Connect();
    Netmgr.GetMediatorMgr()->InitObject();

    cout << "**********Play************" << endl;

    thread time_thread(&CNetMgr::Timer_Worker, &Netmgr);
    vector <thread> worker_threads;
    for (int i = 0; i < 4; ++i) worker_threads.emplace_back(thread(&CNetMgr::Worker_Thread, &Netmgr));
    worker_threads.emplace_back(thread(&CNetMgr::DeadReckoning_Thread, &Netmgr));
    for (auto& th : worker_threads) th.join();
    
    time_thread.join();

    Netmgr.CloseSocket();
    
}