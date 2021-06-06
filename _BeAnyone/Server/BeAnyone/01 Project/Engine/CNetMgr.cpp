#include "pch.h"
#include "CNetMgr.h"
#include "GameObject.h"
#include "Scene.h"

#include "Layer.h"
#include "Camera.h"

#include "Transform.h"
#include "MeshRender.h"
#include "Collider.h"     

#include "PlayerScript.h"
#include "ToolCamScript.h"
#include "MonsterScript.h"
OBJ_TYPE CheckObjType(const uShort& id)
{
	if (id >= 0 && id < MAX_USER)return OBJ_TYPE::PLAYER;
	else if (id >= START_MONSTER && id < END_MONSTER)return OBJ_TYPE::MONSTER;
}

//const char ip[] = "192.168.0.11";
const char ip[] = "192.168.0.3";
//const char ip[] = "192.168.140.59";
const char office[] = "192.168.102.43";
const char KPUIP[] = "192.168.20.138";

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
	cs_packet_move p;
	p.type = CS_MOVE;
	p.size = sizeof(p);
	p.dir = dir;
	p.localVec = local;

	Send_Packet(&p);
}

void CNetMgr::Send_Move_Packet(unsigned const char& dir, const Vector3& local, const float& rotateY)
{
	cs_packet_move p;
	p.type = CS_MOVE;
	p.size = sizeof(p);
	p.dir = dir;
	p.localVec = local;
	p.rotateY = rotateY;

	Send_Packet(&p);

}

void CNetMgr::Send_Move_Packet(unsigned const char& dir, const Vector3& local, 
	const Vector3& dirVec, const float& rotateY, const system_clock::time_point& startTime,
	const float& delta, const bool& isMoving)
{
	cs_packet_move p;
	p.type = CS_MOVE;
	p.size = sizeof(p);
	p.dir = dir;
	p.localVec = local;
	p.DirVec = dirVec;
	p.rotateY = rotateY;
	p.Start = startTime;
	p.speed = g_Object.find(g_myid)->second->GetScript<CPlayerScript>()->GetSpeed();
	p.deltaTime = delta;
	p.isMoving = isMoving;


	Send_Packet(&p);
}

void CNetMgr::Send_Stop_Packet(const bool& isMoving)
{
	cs_packet_stop p;
	p.type = CS_STOP;
	p.size = sizeof(p);
	p.isMoving = isMoving;
	Send_Packet(&p);
}


void CNetMgr::Send_Attack_Packet(const uShort& victim_id)
{
	cs_packet_attack p;
	p.type = CS_ATTACK;
	p.size = sizeof(p);
	p.id = victim_id;
	p.hp = g_Object.find(victim_id)->second->GetScript<CMonsterScript>()->GetHP();
	Send_Packet(&p);
}

void CNetMgr::Send_MonsterDead_Packet(const uShort& monster_id)
{
	cs_packet_MonsterDead p;
	p.type = CS_MONSTER_DEAD;
	p.size = sizeof(p);
	p.id = monster_id;
	Send_Packet(&p);
}

void CNetMgr::Send_Player_Animation_Packet(const uShort& user_id, const bool& isAttack)
{
	cs_packet_AttackAni p;
	p.type = CS_ATTACK_ANIMATION;
	p.size = sizeof(p);
	p.id = user_id;
	p.isAttack = isAttack;
	Send_Packet(&p);
}

void CNetMgr::SetAnimation(int id, const Ani_TYPE& type)
{
	//cout << "------Setani -> " << (int)type << endl;
	
	g_Object.find(id)->second->Animator3D()->SetBones(m_aniData[(int)type]->GetBones());
	g_Object.find(id)->second->Animator3D()->SetAnimClip(m_aniData[(int)type]->GetAnimClip());
	g_Object.find(id)->second->MeshRender()->SetMesh(m_aniData[(int)type]);
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

		// 여기 패킷아이디로 바꾸자
		g_Object.emplace(g_myid, m_pObj);
		g_Object.find(g_myid)->second->GetScript<CPlayerScript>()->initDeadReckoner();
	}
	break;
	case SC_PACKET_ENTER:
	{
		sc_packet_enter* my_packet = reinterpret_cast<sc_packet_enter*>(ptr);
		int id = my_packet->id;
		cout << "enter 함" << endl;
		if (id == g_myid)
		{
			//g_Object.find(g_myid)->second->Transform()->SetLocalPos(my_packet->localVec);
		}
		else
		{
			if (CheckObjType(id) == OBJ_TYPE::PLAYER)
			{
				if (0 == g_Object.count(id))
				{
					Ptr<CMeshData> pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\Player\\PlayerMale@nIdle1.fbx", FBX_TYPE::PLAYER);

					CGameObject* pObject = new CGameObject;
					g_Object.emplace(id, pObject);

					g_Object.find(id)->second = pMeshData->Instantiate();
					g_Object.find(id)->second->SetName(L"PlayerMale");
					g_Object.find(id)->second->FrustumCheck(false);
					g_Object.find(id)->second->Transform()->SetLocalPos(my_packet->localVec);
					g_Object.find(id)->second->Transform()->SetLocalScale(Vector3(1.f, 1.f, 1.f));
					g_Object.find(id)->second->Transform()->SetLocalRot(Vector3(0.f, my_packet->RotateY, 0.f));
					g_Object.find(id)->second->AddComponent(new CPlayerScript);

					CSceneMgr::GetInst()->GetCurScene()->AddGameObject(L"Player", g_Object.find(id)->second, false);

					g_Object.find(id)->second->GetScript<CPlayerScript>()->SetTerrain(
						g_Object.find(g_myid)->second->GetScript<CPlayerScript>()->GetTerrain()
					);
					g_Object.find(id)->second->Transform()->SetLocalRot(my_packet->RotateY);

				}
			}
			else if (CheckObjType(id) == OBJ_TYPE::MONSTER)
			{
				if (0 == g_Object.count(id))
				{
					Ptr<CMeshData> pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\Monster\\monster3_walking.fbx", FBX_TYPE::MONSTER);
					CGameObject* pObject = new CGameObject;
					g_Object.emplace(id, pObject);

					//pMeshData->Save(pMeshData->GetPath());
					//
					g_Object.find(id)->second = pMeshData->Instantiate();
					g_Object.find(id)->second->SetName(L"FireMonster");
					g_Object.find(id)->second->FrustumCheck(false);
					g_Object.find(id)->second->Transform()->SetLocalPos(my_packet->localVec);

					g_Object.find(id)->second->Transform()->SetLocalScale(Vector3(1.f, 1.f, 1.f));//(1.0f, 1.0f, 1.0f));
					g_Object.find(id)->second->Transform()->SetLocalRot(Vector3(XM_PI / 2, 0.f, 0.f));
					g_Object.find(id)->second->AddComponent(new CCollider);
					g_Object.find(id)->second->Collider()->SetColliderType(COLLIDER_TYPE::MESH, L"monster3_walking");
					g_Object.find(id)->second->Collider()->SetBoundingBox(BoundingBox(g_Object.find(id)->second->Transform()->GetLocalPos()
						, g_Object.find(id)->second->MeshRender()->GetMesh()->GetBoundingBoxExtents()));
					g_Object.find(id)->second->Collider()->SetBoundingSphere(BoundingSphere
					(g_Object.find(id)->second->Transform()->GetLocalPos(),
						g_Object.find(id)->second->MeshRender()->GetMesh()->GetBoundingSphereRadius()));

					// Script 설정
					g_Object.find(id)->second->AddComponent(new CMonsterScript);

					CSceneMgr::GetInst()->GetCurScene()->AddGameObject(L"Monster", g_Object.find(id)->second, false);

					//animation
			   //walk
					CMonsterScript* monsterScript = g_Object.find(id)->second->GetScript<CMonsterScript>();
					monsterScript->SetAnimationData(pMeshData->GetMesh());
					//dead
					Ptr<CMeshData> pMeshDataKey = CResMgr::GetInst()->LoadFBX(L"FBX\\Monster\\monster3_die.fbx", FBX_TYPE::MONSTER);
					monsterScript->SetAnimationData(pMeshDataKey->GetMesh());

					g_Object.find(id)->second->GetScript<CMonsterScript>()->SetTerrain(
						g_Object.find(g_myid)->second->GetScript<CPlayerScript>()->GetTerrain()
					);

					g_Object.find(id)->second->GetScript<CMonsterScript>()->SetID(id);
					g_Object.find(id)->second->GetScript<CMonsterScript>()->SetHP(my_packet->hp);
					cout << "---------------------" << endl;
					cout << "ID : " << id << "    packet HP : " << my_packet->hp << endl;
				}
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
			//ObjTrans->SetLocalPos(packet->localVec);
		}
		else // 여기 브로드캐스팅하려면 다시수정
		{
			//추가
			if (0 != g_Object.count(other_id))
			{
				system_clock::time_point end = system_clock::now();
				nanoseconds rtt = duration_cast<nanoseconds>(end - packet->Start);
				g_Object.find(other_id)->second->GetScript<CPlayerScript>()->SetBisFrist(true);
				if (packet->isMoving)
					g_Object.find(g_myid)->second->GetScript<CPlayerScript>()->SetAnimation(other_id, Ani_TYPE::WALK_F);
				else
					g_Object.find(g_myid)->second->GetScript<CPlayerScript>()->SetAnimation(other_id, Ani_TYPE::IDLE);

				g_Object.find(g_myid)->second->GetScript<CPlayerScript>()->SetOtherMovePacket(packet, rtt.count() * 0.00000001);
				g_Object.find(other_id)->second->GetScript<CPlayerScript>()->Set_InterpolationCnt_Zero();
			}
		}
	}
	break;
	case SC_PACKET_STOP:
	{
		sc_packet_stop* packet = reinterpret_cast<sc_packet_stop*>(ptr);
		int other_id = packet->id;

		if (other_id == g_myid)
		{
			//ObjTrans->SetLocalPos(packet->localVec);
		}
		else // 여기 브로드캐스팅하려면 다시수정
		{
			g_Object.find(g_myid)->second->GetScript<CPlayerScript>()->SetAnimation(other_id, Ani_TYPE::IDLE);

			g_Object.find(g_myid)->second->GetScript<CPlayerScript>()->SetOtherMovePacket__IsMoving(packet->isMoving);
		}
	}
	break;
	case SC_PACKET_LEAVE:
	{
		
		sc_packet_leave* my_packet = reinterpret_cast<sc_packet_leave*>(ptr);
		int other_id = my_packet->id;
		if (other_id == g_myid) {
		}
		else {
			if (0 != g_Object.count(other_id))
			{
				if (CheckObjType(other_id) == OBJ_TYPE::PLAYER)
				{
					g_Object.find(other_id)->second->GetScript<CPlayerScript>()->DeleteObject(g_Object.find(other_id)->second);
					CEventMgr::GetInst()->update();
					CEventMgr::GetInst()->update();
					g_Object.erase(other_id);
				}
				else if (CheckObjType(other_id) == OBJ_TYPE::MONSTER)
				{
					if (!my_packet->isAttack)
					{
						g_Object.find(other_id)->second->GetScript<CMonsterScript>()->DeleteObject(g_Object.find(other_id)->second);
						CEventMgr::GetInst()->update();
						CEventMgr::GetInst()->update();
						g_Object.erase(other_id);

					}
					else
					{

						g_Object.find(other_id)->second->GetScript<CMonsterScript>()->SetBisAttack(my_packet->isAttack);
					}
				}

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
		sc_packet_attack* packet = reinterpret_cast<sc_packet_attack*>(ptr);
		int id = packet->id;
		if (id == g_myid)
		{

		}
		else
		{

			if (CheckObjType(id) == OBJ_TYPE::MONSTER)
			{
				g_Object.find(packet->id)->second->GetScript<CMonsterScript>()->SetHP(packet->hp);


			}
		}
	}
	break;
	case SC_PACKET_ID:
	{
		sc_packet_id* packet = reinterpret_cast<sc_packet_id*>(ptr);
		g_myid = packet->id;

		cout << "Send_ID_Packet My ID : " << g_myid << endl;
	}
	break;

	case SC_PACKET_ATTACKANI:
	{
		sc_packet_AttackAni* packet = reinterpret_cast<sc_packet_AttackAni*>(ptr);
		int id = packet->id;
		if (id == g_myid) {
		}
		else {
			if (0 != g_Object.count(id)&&CheckObjType(id) == OBJ_TYPE::PLAYER)
			{
				if (packet->isAttack)
				{
					g_Object.find(g_myid)->second->GetScript<CPlayerScript>()->SetAnimation(id, Ani_TYPE::ATTACK);

				}
				else
				{
					g_Object.find(g_myid)->second->GetScript<CPlayerScript>()->SetAnimation(id, Ani_TYPE::IDLE);
				}
			}
		}
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

