#include "pch.h"
#include "CNetMgr.h"
#include "GameObject.h"
#include "Scene.h"

#include "Layer.h"
#include "GameObject.h"
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


CNetMgr::CNetMgr()
{
	//// ===============
	//	// Test Scene 생성
	//	// ===============
	//m_pCurScene = new CScene;
	//m_pCurScene->SetName(L"Test Scene");

	//// ===============
	//// Layer 이름 지정
	//// ===============
	//m_pCurScene->GetLayer(0)->SetName(L"Default");
	//m_pCurScene->GetLayer(1)->SetName(L"Player");
	//m_pCurScene->GetLayer(2)->SetName(L"Monster");
	//m_pCurScene->GetLayer(3)->SetName(L"Bullet");
	cout << "net mgr 생성자" << endl;

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

void CNetMgr::SetLoginPacket(sc_packet_login_ok* packet)
{
	m_loginPacket = packet;
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
	//err_quit("Connect()");

	WSAEVENT event = WSACreateEvent();
	memset(&m_overlapped, 0, sizeof(m_overlapped));

	m_overlapped.hEvent = event;
	cout << "over : " << m_overlapped.hEvent << endl;

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

void CNetMgr::Send_Move_Packet(unsigned const char& dir)
{
	cs_packet_move m_packet;
	m_packet.type = CS_MOVE;
	m_packet.size = sizeof(m_packet);
	m_packet.direction = dir;
	Send_Packet(&m_packet);
}

void CNetMgr::Send_Attack_Packet()
{
	cs_packet_attack m_packet;
	m_packet.type = CS_ATTACK;
	m_packet.size = sizeof(m_packet);
	Send_Packet(&m_packet);
}

void CNetMgr::testX(const float& _x)
{
	x = _x;
	cout << "test : " << x << endl;
}

int CNetMgr::recvn(SOCKET s, char* buf, int len, int flags)
{
	int received;
	char* ptr = buf;
	int left = 38;
	if (buf[1] == SC_PACKET_LOGIN_OK)
		left = sizeof(sc_packet_login_ok);
	//int left = len;

	while (left > 0) {
		received = recv(g_Socket, ptr, left, flags);
		if (received == SOCKET_ERROR)
			return SOCKET_ERROR;
		else if (received == 0)
			break;
		left -= received;
		ptr += received;
	}

	return (len - left);
}




void CNetMgr::Recevie_Data()
{
	EXOVER* dataBuf = new EXOVER{};
	DWORD	 recvByte = 0;
	DWORD	 flags = 0;

	dataBuf->over = m_overlapped;
	//cout << g_Socket << endl;
	//cout << "over : " << m_overlapped.hEvent << endl;

	//auto result = WSARecv(g_Socket, &(dataBuf->wsabuf), 1, &recvByte, &flags, &(dataBuf->over), NULL);
	char recvbuf[100] = "";
	int fg = 0;

	int ret = recv(g_Socket, recvbuf, sizeof(recvbuf), fg);
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

void CNetMgr::Recevie_ID_Data()
{

	EXOVER* dataBuf = new EXOVER{};
	DWORD	 recvByte = 0;
	DWORD	 flags = 0;

	dataBuf->over = m_overlapped;
	//cout << g_Socket << endl;
	//cout << "over : " << m_overlapped.hEvent << endl;

	//auto result = WSARecv(g_Socket, &(dataBuf->wsabuf), 1, &recvByte, &flags, &(dataBuf->over), NULL);
	char recvbuf[100] = "";
	int fg = 0;

	int ret = recv(g_Socket, recvbuf, sizeof(recvbuf), fg);
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
		if (recvbuf[1] == SC_PACKET_ID)
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
		
		Vector3 a(p->x, p->y, p->z);
		
		m_pObj->Transform()->SetLocalPos(Vector3((p->x, p->y, p->z)));


		g_Object.emplace(g_myid, m_pObj);

	//	// ==================
	//// Camera Object 생성
	//// ==================
	//// Main Camera
	//	CGameObject* pMainCam = new CGameObject;
	//	pMainCam->SetName(L"MainCam");
	//	pMainCam->AddComponent(new CTransform);
	//	pMainCam->AddComponent(new CCamera);
	//	pMainCam->AddComponent(new CToolCamScript);

	//	pMainCam->Transform()->SetLocalPos(Vector3(0.f, 600.f, -500.f));
	//	pMainCam->Transform()->SetLocalRot(Vector3(XM_PI / 6 /*0.f*/, 0.0f, 0.f));
	//	pMainCam->Camera()->SetProjType(PROJ_TYPE::PERSPECTIVE);
	//	pMainCam->Camera()->SetFar(100000.f);
	//	pMainCam->Camera()->SetLayerAllCheck();
	//	pMainCam->Camera()->SetLayerCheck(30, false);
	//	//vector<CToolCamScript*> camScript = (CToolCamScript*)(pMainCam->GetScripts())
	//	////camScript[0]->
	//	//pMainCam->GetScripts()[0]->SetPlayer();

	//	CToolCamScript* camScript = pMainCam->GetScript<CToolCamScript>();
	//	camScript->SetPlayer(pObject);

	//	CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"Default")->AddGameObject(pMainCam);




	}
	break;
	case SC_PACKET_ENTER:
	{
		sc_packet_enter* my_packet = reinterpret_cast<sc_packet_enter*>(ptr);
		int id = my_packet->id;
		Ptr<CMeshData> pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\PlayerMale.fbx");
		// ===================
		// Player 오브젝트 생성
		// ===================
		CGameObject* pObject = new CGameObject;
		if (id == g_myid)
		{
			g_Object.find(g_myid)->second->Transform()->SetLocalPos(Vector3(my_packet->x, my_packet->y, my_packet->z));
			cout << "enter id(" << g_myid << ") packet=================" << endl;
		}
		else
		{
			if (id < MAX_USER)
			{

				CGameObject* pObject = new CGameObject;


				pObject = pMeshData->Instantiate();
				pObject->SetName(L"PlayerMale");
				pObject->FrustumCheck(false);
				pObject->Transform()->SetLocalPos(Vector3(0.f, 150.f, 300.f));
				pObject->Transform()->SetLocalScale(Vector3(1.f, 1.f, 1.f));
				pObject->Transform()->SetLocalRot(Vector3(0.f, XM_PI, 0.f));


				pObject->AddComponent(new CPlayerScript);

				CSceneMgr::GetInst()->GetCurScene()->AddGameObject(L"Player", pObject, false);
				g_Object.emplace(id, pObject);
				cout << "packet enter in " << endl;
				cout << "id -> " << id << endl;
				cout << "g_Object size -> " << g_Object.size() << endl;
			}
		}
	}
	break;
	case SC_PACKET_MOVE:
	{
		sc_packet_move* packet = reinterpret_cast<sc_packet_move*>(ptr);
		int other_id = packet->id;
		if (other_id == g_myid)
		{
			cout << "my move===>" << other_id << ", " << g_myid << endl;
			Vector3 temp(0.f,0.f, packet->z + DT * 200.f);
			g_Object.find(other_id)->second->Transform()->SetLocalPos(temp);

			cout << g_Object.find(other_id)->second->Transform()->GetLocalPos().x << endl;
			cout << g_Object.find(other_id)->second->Transform()->GetLocalPos().y << endl;
			cout << g_Object.find(other_id)->second->Transform()->GetLocalPos().z << endl;
			cout << endl;

		}
		else
		{
			//추가
			cout << "my move===>" << other_id << ", " << g_myid << endl;
			if (0 != g_Object.count(other_id))
				g_Object.find(other_id)->second->Transform()->SetLocalPos(Vector3(packet->x + DT * 200.f, packet->y + DT * 200.f, packet->z + DT * 200.f));
		}
	}
	break;
	case SC_PACKET_LEAVE:
	{

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
		// 플레이어스크립트에서 addobj 하는 부분과 어떻게 연결시킬껀지 고민해보기
		// 아직 테스트용 코드
		// 이따구로하면 멀티스레드에서 무적권 꼬임
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

