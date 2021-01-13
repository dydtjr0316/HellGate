#pragma once

#define SINGLE(type) private: type(); ~type();\
public:\
static type* GetInst()\
{\
	static type mgr;\
	return &mgr;\
}

// ThrowIfFailed
//class DxException
//{
//public:
//    DxException() = default;
//    DxException(HRESULT hr, const std::wstring& functionName, const std::wstring& filename, int lineNumber);
//
//    std::wstring ToString()const;
//
//    HRESULT ErrorCode = S_OK;
//    std::wstring FunctionName;
//    std::wstring Filename;
//    int LineNumber = -1;
//};
//
//inline std::wstring AnsiToWString(const std::string& str)
//{
//    WCHAR buffer[512];
//    MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, buffer, 512);
//    return std::wstring(buffer);
//}
//
//#ifndef ThrowIfFailed
//#define ThrowIfFailed(x)                                              \
//{                                                                     \
//    HRESULT hr__ = (x);                                               \
//    std::wstring wfn = AnsiToWString(__FILE__);                       \
//    if(FAILED(hr__)) { throw DxException(hr__, L#x, wfn, __LINE__); } \
//}
//#endif

// simd 사용 안함
#define _XM_NO_INTRINSICS_

#include <d3d12.h>
#include <wrl/client.h>
#include <d3dcompiler.h>
#include <dxgi.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>

#pragma comment(lib, "d3d12")
#pragma comment(lib, "dxgi")
#pragma comment(lib, "dxguid")
#pragma comment(lib, "d3dcompiler")

using namespace DirectX;
using namespace DirectX::PackedVector;
using namespace Microsoft::WRL;


#include <string>
#include <vector>
#include <list>
#include <map>


using std::vector;
using std::list;
using std::map;
using std::string;
using std::wstring;
using std::make_pair;

#include "struct.h"
