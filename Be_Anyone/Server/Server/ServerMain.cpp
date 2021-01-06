#include "stdafx.h"
// 클라이언트 추가 후 
//#include "CClient.h"
#include "Global.h"
#include "CDataBase.h"

// simd 작업하던거 시간남으면 해보기
/*//__m128i add;
//__m128i add2;
//__m128i mul;
//__m128i x;
//__m128i y;
//
//__m128i resultadd;
//__m128i resultx;
//__m128i resulty;
//
//
//__declspec(align(16)) int pointX[2];
//__declspec(align(16)) int pointY[2];
//__declspec(align(16)) int result[2];
//__declspec(align(16)) int finalresult;*/

//CDataBase g_DataBase;

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

int main()
{
    return 0;
}