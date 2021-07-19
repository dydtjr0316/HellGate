#pragma once
#include <windows.h>

#include <WS2tcpip.h>
#include <MSWSock.h>
#include <thread>
#include <mutex>
#include <iostream>
#include <chrono>
#include <stdio.h>  

#include <unordered_map>

// lib
#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "MSWSock.lib")
using namespace std;
using namespace chrono;
class Vector2;
class Vector3;
class Vector4;
class Matrix;
typedef unsigned short uShort;
#include "../../../../Server/Server/Server/Protocol.h"
#include "../Engine/Enum.h"
#include "../Engine/Struct1.h"


#include "Extern.h"