#include "stdafx.h"
// 클라이언트 추가 후 
//#include "CClient.h"
#include "CDataBase.h"

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

int main()
{
    return 0;
}