#include "stdafx.h"
// Ŭ���̾�Ʈ �߰� �� 
//#include "CClient.h"
#include "CDataBase.h"

//CDataBase g_DataBase;
SOCKET l_socket;
HANDLE g_iocp;

// mutex
mutex timer_lock;

// sector ����
unordered_set<int>				g_Sector[SECTOR_ROW][SECTOR_COL];
priority_queue<event_type>		timer_queue;

void show_error() {     // ���� ���
    printf("error\n");
}

void error_display(const char* msg, int err_no)     // ���� ���
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

bool CAS(int* addr, int exp, int update)        // cas 
{
    return atomic_compare_exchange_strong(reinterpret_cast<atomic_int*>(addr), &exp, update);
}

void is_near()
{
    // �� prac - simd ���Ͽ� ���� �����س��� getlength ���� �����ϱ� �ٵ� ����غ��� 
}

int main()
{
    return 0;
}