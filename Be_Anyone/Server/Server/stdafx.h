#pragma once
// c++
#include <iostream>
#include <chrono>
#include<algorithm>
#include <stdio.h>  

// namespcae
using namespace std;
using namespace chrono;

// server

#include <WS2tcpip.h>
#include <MSWSock.h>
#include <thread>
#include <mutex>
#include <windows.h>  

// stl
#include <vector>
#include <queue>
#include <unordered_set>
#include <unordered_map>
#include <functional>
// lua 파일 추가 고려 < 스크립트 >
//#include "include/lua.hpp"
//#include "include/luaconf.h"

#include <emmintrin.h>

// DB
#include <sqlext.h>  

// lib
#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "MSWSock.lib")

/* 스크립트
#pragma comment(lib, "MSWSock.lib")
#pragma comment(lib, "lua54.lib")
#pragma comment (lib, "odbc32.lib")*/

// user
#include "protocol.h"
#include "enum.h"
#include "struct.h"
#include "Global.h" 
#include "Template.h"
