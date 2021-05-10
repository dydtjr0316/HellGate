#include "pch.h"
#include "CNetMgr.h"
#include "GameObject.h"
#include "Scene.h"

#include "Layer.h"
#include "Camera.h"

#include "Transform.h"
#include "MeshRender.h"

#include "PlayerScript.h"
#include "ToolCamScript.h"
const char ip[] = "127.0.0.1";

CNetMgr g_netMgr;

int testpacket;
OBJLIST g_Object;
int g_myid = -1;
CAMOBJLIST g_CamObject;

CNetMgr::CNetMgr()
{
	m_pObj = nullptr;
}
void CNetMgr::err_quit(const char* msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	MessageBox(NULL, (LPCTSTR)lpMsgBuf, (LPCTSTR)msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
	exit(1);
}

void CNetMgr::Connect()
{

	wcout.imbue(locale("korean"));
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != NULL)err_quit("WSAStartup");

	//g_Socket = WSASocket(PF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	g_Socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	ULONG l = 1;
	ioctlsocket(g_Socket, FIONBIO, (unsigned long*)&l);


	if (g_Socket == INVALID_SOCKET)err_quit("WSASocket");

	SOCKADDR_IN recvAddr;
	memset(&recvAddr, 0, sizeof(recvAddr));

	recvAddr.sin_family = AF_INET;
	recvAddr.sin_addr.s_addr = inet_addr(ip);
	recvAddr.sin_port = htons(SERVER_PORT);

	if (connect(g_Socket, (SOCKADDR*)&recvAddr, sizeof(recvAddr)) == SOCKET_ERROR)
	{
		if (WSAGetLastError() == WSAEWOULDBLOCK)
		{
			int i = 0;
		}
		else
		{
			int i = 0;
		}
	}

	WSAEVENT event = WSACreateEvent();
	memset(&m_overlapped, 0, sizeof(m_overlapped));

	m_overlapped.hEvent = event;

}

void CNetMgr::Send_Packet(void* _packet)
{
	EXOVER dataBuf;
	char* packet = reinterpret_cast<char*>(_packet);
	size_t sent;
	dataBuf.wsabuf.len = packet[0];
	dataBuf.wsabuf.buf = (char*)packet;
	dataBuf.over = m_overlapped;

	testpacket = dataBuf.wsabuf.len;
	if (WSASend(g_Socket, &dataBuf.wsabuf, 1, (LPDWORD)&sent, 0, &dataBuf.over, NULL) == SOCKET_ERROR)
	{
		if (WSAGetLastError() == WSA_IO_PENDING)
		{
			WSAWaitForMultipleEvents(1, &m_overlapped.hEvent, TRUE, WSA_INFINITE, FALSE);
			WSAGetOverlappedResult(g_Socket, &m_overlapped, (LPDWORD)&sent, FALSE, NULL);
		}
		else
			err_quit("WSASend");
	}
}

void CNetMgr::Send_LogIN_Packet()
{
	cs_packet_login l_packet;
	l_packet.size = sizeof(l_packet);
	l_packet.type = CS_LOGIN;
	//int t_id = GetCurrentProcessId();
	char tempName[MAX_ID_LEN];
	string tempstring;
	cout << "nick name : ";
	cin >> tempName;
	sprintf_s(l_packet.name, tempName);
	strcpy_s(name, l_packet.name);

	//cout << sizeof(l_packet) << endl << endl;

	Send_Packet(&l_packet);
}


void CNetMgr::Send_Move_Packet(unsigned const char& dir, const Vector3& local)
{
	cs_packet_move m_packet;
	m_packet.type = CS_MOVE;
	m_packet.size = sizeof(m_packet);
	m_packet.direction = dir;
	m_packet.localVec = local;

	Send_Packet(&m_packet);
}

void CNetMgr::Send_Rotate_Packet(unsigned const char& dir, const float& rotateY)
{
	cs_packet_rotate packet;
	packet.type = CS_ROTATE;
	packet.size = sizeof(packet);
	packet.dir = dir;
	packet.rotateY = rotateY;
	Send_Packet(&packet);
}

void CNetMgr::Send_Attack_Packet()
{
	cs_packet_attack m_packet;
	m_packet.type = CS_ATTACK;
	m_packet.size = sizeof(m_packet);
	Send_Packet(&m_packet);
}

void CNetMgr::Recevie_Data()
{
	EXOVER* dataBuf = new EXOVER{};
	DWORD	 recvByte = 0;
	DWORD	 flags = 0;
	dataBuf->over = m_overlapped;
	char recvbuf[100] = "";
	int fg = 0;

	int ret = recv(g_Socket, recvbuf, sizeof(recvbuf), fg);
	if (ret <= 0)return;

	size_t retbytesize = ret;


	if (ret < 0)
	{
		if (WSAGetLastError() == WSAEWOULDBLOCK)
		{
			int i = 0;
		}
	}
	else
	{
		Process_Data(recvbuf, retbytesize);
	}
}



void CNetMgr::ProcessPacket(char* ptr)
{
	switch (ptr[1])
	{
	case SC_PACKET_LOGIN_OK:
	{
		sc_packet_login_ok* p = reinterpret_cast<sc_packet_login_ok*>(ptr);
		cout << "ok id -> " << p->id << endl;
		m_pObj->Transform()->SetLocalPos(Vector3(p->localVec));

		g_Object.emplace(g_myid, m_pObj);
	}
	break;
	case SC_PACKET_ENTER:
	{
		sc_packet_enter* my_packet = reinterpret_cast<sc_packet_enter*>(ptr);
		int id = my_packet->id;
		//cout << "enter packet recv -> " << my_packet->id << endl;
		Ptr<CMeshData> pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\PlayerMale.fbx");
		CGameObject* pObject = new CGameObject;
		if (id == g_myid)
		{
			g_Object.find(g_myid)->second->Transform()->SetLocalPos(my_packet->localVec);
		}
		else
		{
			if (id < MAX_USER)
			{
				g_Object.emplace(id, pObject);

				g_Object.find(id)->second = pMeshData->Instantiate();
				g_Object.find(id)->second->SetName(L"PlayerMale");
				g_Object.find(id)->second->FrustumCheck(false);
				g_Object.find(id)->second->Transform()->SetLocalPos(my_packet->localVec);
				g_Object.find(id)->second->Transform()->SetLocalScale(Vector3(1.f, 1.f, 1.f));
				g_Object.find(id)->second->Transform()->SetLocalRot(Vector3(0.f, XM_PI, 0.f));
				g_Object.find(id)->second->AddComponent(new CPlayerScript);

				CSceneMgr::GetInst()->GetCurScene()->AddGameObject(L"Player", g_Object.find(id)->second, false);

				CGameObject* pTerrainObject = new CGameObject;
				pTerrainObject->SetName(L"Terrain");
				pTerrainObject->AddComponent(new CTransform);
				pTerrainObject->AddComponent(new CMeshRender);
				pTerrainObject->AddComponent(new CTerrain);
				pTerrainObject->FrustumCheck(false);
				pTerrainObject->Transform()->SetLocalPos(Vector3(0.f, 10.f, 0.f));
				pTerrainObject->Transform()->SetLocalScale(Vector3(100.f, 300.f, 100.f));
				pTerrainObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"TerrainMtrl"));
				pTerrainObject->Terrain()->init();
				CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"Default")->AddGameObject(pTerrainObject);
			
				g_Object.find(id)->second->GetScript<CPlayerScript>()->SetTerrain(pTerrainObject->Terrain());
			}
		}
	}
	break;
	case SC_PACKET_MOVE:
	{
		sc_packet_move* packet = reinterpret_cast<sc_packet_move*>(ptr);
		int other_id = packet->id;
		Vector3 temp;
		CTransform* ObjTrans = g_Object.find(other_id)->second->Transform();
		if (other_id == g_myid)
		{
			//ObjTrans->SetLocalPos(packet->localVec);
		}
		else // 여기 브로드캐스팅하려면 다시수정
		{
			//추가
			if (0 != g_Object.count(other_id))
			{
				ObjTrans->SetLocalPos(packet->localVec);
			}
		}
	}
	break;
	case SC_PACKET_MOUSE:
	{
		int other_id;
		sc_packet_rotate* rotate_packet = nullptr;
		switch (ptr[2])
		{
		case Rotate_LBTN:
			rotate_packet = reinterpret_cast<sc_packet_rotate*>(ptr);
			other_id = rotate_packet->id;

			if (other_id == g_myid)
			{
				// 세팅하지 않기
				cout << "씨~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~발" << endl;
				cout << "씨~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~발" << endl;
				cout << "씨~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~발" << endl;
				cout << "씨~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~발" << endl;
				cout << "씨~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~발" << endl;
			}
			else
			{
				g_Object.find(other_id)->second->Transform()->SetLocalRot(Vector3(0.f, rotate_packet->rotateY, 0.f));
				cout << other_id << "번 클라이언트 세팅" << endl;
				cout << "roatet packet y >> " << rotate_packet->rotateY << endl;
			}
			break;
		default:
			break;
		}
		
	}
	break;
	case SC_PACKET_LEAVE:
	{
		sc_packet_leave* my_packet = reinterpret_cast<sc_packet_leave*>(ptr);
		int other_id = my_packet->id;
		if (other_id == g_myid) {
			delete g_Object.find(g_myid)->second;
			g_Object.erase(g_myid);

		}
		else {
			if (0 != g_Object.count(other_id))
			{
				delete g_Object.find(other_id)->second;
				g_Object.erase(other_id);
			}
		}
	}
	break;
	case SC_PACKET_CHAT:
	{
		
	}
	break;
	case SC_PACKET_ATTACK:
	{

	}
	break;
	case SC_PACKET_ID:
	{
		sc_packet_id* packet = reinterpret_cast<sc_packet_id*>(ptr);
		g_myid = packet->id;

		cout << "My ID : " << g_myid << endl;
	}
	break;
	default:
		printf("Unknown PACKET type [%d]\n", ptr[1]);
	}
}

void CNetMgr::Process_Data(char* net_buf, size_t& io_byte)
{
	char* ptr = net_buf;
	static size_t in_packet_size = 0;
	static size_t saved_packet_size = 0;
	static char packet_buffer[MAX_BUF_SIZE];


	while (0 != io_byte) {
		if (0 == in_packet_size) in_packet_size = ptr[0];
		if (io_byte + saved_packet_size >= in_packet_size) {
			memcpy(packet_buffer + saved_packet_size, ptr, in_packet_size - saved_packet_size);
	
				ProcessPacket(packet_buffer);
				ptr += in_packet_size - saved_packet_size;
				io_byte -= in_packet_size - saved_packet_size;
				in_packet_size = 0;
				saved_packet_size = 0;
			
			
		}
		else {
			memcpy(packet_buffer + saved_packet_size, ptr, io_byte);
			saved_packet_size += io_byte;
			io_byte = 0;
		}
	}
}

void CNetMgr::Enter_Player(const int& id)
{

}

