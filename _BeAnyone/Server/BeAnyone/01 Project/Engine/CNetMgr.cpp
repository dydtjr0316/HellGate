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
#include "SwordScript.h"
int cnt = 0;
bool tempbool = false;

OBJECT_TYPE CheckObjType(const uShort& id)
{
	if (id >= 0 && id < MAX_USER)return OBJECT_TYPE::CLIENT;
	else if (id >= START_MONSTER && id < END_MONSTER)return OBJECT_TYPE::MONSTER;
}

//const char ip[] = "192.168.0.11";
//const char ip[] = "192.168.0.7";
//const char ip[] = "192.168.0.13";
//const char ip[] = "221.151.160.142";
const char ip[] = "192.168.102.247";//근로
//const char KPUIP[] = "192.168.140.245";

CNetMgr g_netMgr;

int testpacket;
int ctnt = 0;
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
	DWORD flag;
	dataBuf.wsabuf.len = packet[0];
	dataBuf.wsabuf.buf = (char*)packet;
	dataBuf.over = m_overlapped;

	testpacket = dataBuf.wsabuf.len;


	//if (WSASend(g_Socket, &dataBuf.wsabuf, 1, (LPDWORD)&sent, 0, &dataBuf.over, NULL) == SOCKET_ERROR)
	if (WSASend(g_Socket, &dataBuf.wsabuf, 1, (LPDWORD)&sent, 0, &dataBuf.over, NULL) == SOCKET_ERROR)
	{
		if (WSAGetLastError() == WSA_IO_PENDING)
		{
			WSAWaitForMultipleEvents(1, &m_overlapped.hEvent, TRUE, WSA_INFINITE, FALSE);
			WSAGetOverlappedResult(g_Socket, &m_overlapped, (LPDWORD)&sent, FALSE, &flag);
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
	cout << "CS_LOGIN 보냄 -> "<<g_myid << endl;
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

void CNetMgr::Send_Monster_Animation_Packet(const uShort& monster_id, const MONSTER_ANI_TYPE& aniType)
{
	cs_pcaket_Monster_Animation p;
	p.type = CS_MONSTER_ANIMATION;
	p.size = sizeof(p);
	p.id = monster_id;
	p.aniType = aniType;
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
		cout << "SC_PACKET_LOGIN_OK"<< endl;
		sc_packet_login_ok* p = reinterpret_cast<sc_packet_login_ok*>(ptr);
		m_pObj->Transform()->SetLocalPos(Vector3(p->localVec));
		g_Object.emplace(g_myid, m_pObj);
		g_Object.find(g_myid)->second->SetID(g_myid);
		g_Object.find(g_myid)->second->GetScript<CPlayerScript>()->initDeadReckoner();
	}
	break;
	case SC_PACKET_ENTER:
	{
		cout << "SC_PACKET_ENTER" << endl;
		sc_packet_enter* my_packet = reinterpret_cast<sc_packet_enter*>(ptr);
		int id = my_packet->id;

		if (id == g_myid)
		{

		}
		else
		{
			if (CheckObjType(id) == OBJECT_TYPE::CLIENT)
			{
				if (0 == g_Object.count(id)/*&& (g_Object.find(id)!= g_Object.end())*/)
				{

					Ptr<CMeshData> pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\Player\\PlayerMale@nIdle1.fbx", FBX_TYPE::PLAYER);

					CGameObject* pObject = new CGameObject;
					g_Object.emplace(id, pObject);
					g_Object.find(id)->second->SetID(id);

					g_Object.find(id)->second = pMeshData->Instantiate();
					g_Object.find(id)->second->SetName(L"PlayerMale");
					g_Object.find(id)->second->FrustumCheck(false);
					g_Object.find(id)->second->Transform()->SetLocalPos(my_packet->localVec);
					g_Object.find(id)->second->Transform()->SetLocalScale(Vector3(1.f, 1.f, 1.f));
					g_Object.find(id)->second->Transform()->SetLocalRot(Vector3(0.f, my_packet->RotateY, 0.f));
					g_Object.find(id)->second->MeshRender()->SetDynamicShadow(true);
					g_Object.find(id)->second->AddComponent(new CPlayerScript);

					CSceneMgr::GetInst()->GetCurScene()->AddGameObject(L"Player", g_Object.find(id)->second, false);

					g_Object.find(id)->second->GetScript<CPlayerScript>()->SetTerrain(
						g_Object.find(g_myid)->second->GetScript<CPlayerScript>()->GetTerrain()
					);
					g_Object.find(id)->second->Transform()->SetLocalRot(my_packet->RotateY);

					// ----
					// 무기
					// ----

					pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\Player\\PlayerMale_Weapon_Sword.fbx", FBX_TYPE::PLAYER);

					CGameObject* pSword = nullptr;

					pSword = pMeshData->Instantiate();
					pSword->SetName(L"sword");
					pSword->FrustumCheck(false);
					pSword->Transform()->SetLocalScale(Vector3(1.f, 1.f, 1.f));//(1.0f, 1.0f, 1.0f));
					pSword->AddComponent(new CCollider);
					pSword->Collider()->SetColliderType(COLLIDER_TYPE::MESH, L"PlayerMale_Weapon_Sword");


					// Script 설정
					pSword->AddComponent(new CSwordScript);
					CSwordScript* SwordScript = pSword->GetScript<CSwordScript>();
					SwordScript->SetBoneFinalMat(g_Object.find(id)->second->Animator3D()->GetSwordFinalBoneMat());



					CSceneMgr::GetInst()->GetCurScene()->AddGameObject(L"Player", pSword, false);
					g_Object.find(id)->second->AddChild(pSword);

				}
			}
			else if (CheckObjType(id) == OBJECT_TYPE::MONSTER)
			{


				if (0 == g_Object.count(id))
				{
					if (id == 1000)
					{
						Ptr<CMeshData> pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\Monster\\monster3@walking.fbx", FBX_TYPE::MONSTER);
						Ptr<CMeshData> pIdleMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\Monster\\monster3@idle.fbx", FBX_TYPE::MONSTER);

						CGameObject* pObject = new CGameObject;
						g_Object.emplace(id, pObject);
						//
						g_Object.find(id)->second = pMeshData->Instantiate();
						g_Object.find(id)->second->SetName(L"FireMonster");
						g_Object.find(id)->second->FrustumCheck(false);
						g_Object.find(id)->second->Transform()->SetLocalPos(my_packet->localVec);

						g_Object.find(id)->second->Transform()->SetLocalScale(Vector3(1.f, 1.f, 1.f));//(1.0f, 1.0f, 1.0f));
						g_Object.find(id)->second->Transform()->SetLocalRot(Vector3(XM_PI / 2, 0.f, 0.f));
						g_Object.find(id)->second->AddComponent(new CCollider);
						// Collider 물어보기
						g_Object.find(id)->second->Collider()->SetColliderType(COLLIDER_TYPE::MESH, L"monster3_idle");
						g_Object.find(id)->second->Collider()->SetBoundingBox(BoundingBox(g_Object.find(id)->second->Transform()->GetLocalPos()
							, pIdleMeshData->GetMesh()->GetBoundingBoxExtents()));
						g_Object.find(id)->second->Collider()->SetBoundingSphere(BoundingSphere
						(g_Object.find(id)->second->Transform()->GetLocalPos(),
							pIdleMeshData->GetMesh()->GetBoundingSphereRadius()));

						// Script 설정
						g_Object.find(id)->second->AddComponent(new CMonsterScript);

						CSceneMgr::GetInst()->GetCurScene()->AddGameObject(L"Monster", g_Object.find(id)->second, false);

						CMonsterScript* monsterScript = g_Object.find(id)->second->GetScript<CMonsterScript>();
						monsterScript->SetMonsterType(MONSTER_TYPE::MONSTER1);
						//animation
						//idle
						monsterScript->SetAnimationData(pMeshData->GetMesh());
						//walk
						pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\Monster\\monster3@walking.fbx", FBX_TYPE::MONSTER);
						monsterScript->SetAnimationData(pMeshData->GetMesh());
						//dead
						pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\Monster\\monster3@die.fbx", FBX_TYPE::MONSTER);
						monsterScript->SetAnimationData(pMeshData->GetMesh());
						//attack
						pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\Monster\\monster3@attack2.fbx", FBX_TYPE::MONSTER);
						monsterScript->SetAnimationData(pMeshData->GetMesh());
						//damage
						pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\Monster\\monster3@damage2.fbx", FBX_TYPE::MONSTER);
						monsterScript->SetAnimationData(pMeshData->GetMesh());

						g_Object.find(id)->second->GetScript<CMonsterScript>()->SetTerrain(
							g_Object.find(g_myid)->second->GetScript<CPlayerScript>()->GetTerrain()
						);
						g_Object.find(id)->second->SetID(id);
						g_Object.find(id)->second->GetScript<CMonsterScript>()->SetID(id);
						g_Object.find(id)->second->GetScript<CMonsterScript>()->SetHP(my_packet->hp);

					}
					else
					{
						Ptr<CMeshData> pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\Monster\\TreantGuard@idle.fbx", FBX_TYPE::MONSTER);
						CGameObject* pObject = new CGameObject;
						g_Object.emplace(id, pObject);
						//
						g_Object.find(id)->second = pMeshData->Instantiate();
						g_Object.find(id)->second->SetName(L"GreenMonster");
						g_Object.find(id)->second->FrustumCheck(false);

						g_Object.find(id)->second->Transform()->SetLocalScale(Vector3(1.f, 1.f, 1.f));//(1.0f, 1.0f, 1.0f));
						{
							int z = (int)(my_packet->localVec.z / g_Object.find(id)->second->Transform()->GetLocalScale().z);
							float fHeight = g_Object.find(g_myid)->second->GetScript<CPlayerScript>()->GetTerrain()->GetHeight(my_packet->localVec.x, my_packet->localVec.z, ((z % 2) != 0)) * 2.f + 65.f;

							if (my_packet->localVec.y != fHeight)
								my_packet->localVec.y = fHeight;
						}
						g_Object.find(id)->second->Transform()->SetLocalPos(my_packet->localVec);

						g_Object.find(id)->second->Transform()->SetLocalRot(Vector3(0.f, 0.f, 0.f));
						g_Object.find(id)->second->AddComponent(new CCollider);
						// Collider 물어보기
						g_Object.find(id)->second->Collider()->SetColliderType(COLLIDER_TYPE::MESH, L"GreenMonster");
						g_Object.find(id)->second->Collider()->SetBoundingBox(BoundingBox(g_Object.find(id)->second->Transform()->GetLocalPos()
							, g_Object.find(id)->second->MeshRender()->GetMesh()->GetBoundingBoxExtents()));
						g_Object.find(id)->second->Collider()->SetBoundingSphere(BoundingSphere
						(g_Object.find(id)->second->Transform()->GetLocalPos(),
							g_Object.find(id)->second->MeshRender()->GetMesh()->GetBoundingSphereRadius()));

						// Script 설정
						g_Object.find(id)->second->AddComponent(new CMonsterScript);

						CSceneMgr::GetInst()->GetCurScene()->AddGameObject(L"Monster", g_Object.find(id)->second, false);

						CMonsterScript* monsterScript = g_Object.find(id)->second->GetScript<CMonsterScript>();
						monsterScript->SetMonsterType(MONSTER_TYPE::MONSTER2);
						//animation
						//idle
						monsterScript->SetAnimationData(pMeshData->GetMesh());
						//walk
						pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\Monster\\TreantGuard@Walk Forward With Root Motion.fbx", FBX_TYPE::MONSTER);
						monsterScript->SetAnimationData(pMeshData->GetMesh());
						//dead
						pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\Monster\\TreantGuard@die.fbx", FBX_TYPE::MONSTER);
						monsterScript->SetAnimationData(pMeshData->GetMesh());
						//attack
						pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\Monster\\TreantGuard@meleeattack01.fbx", FBX_TYPE::MONSTER);
						monsterScript->SetAnimationData(pMeshData->GetMesh());
						//damage
						pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\Monster\\TreantGuard@Damage.fbx", FBX_TYPE::MONSTER);
						monsterScript->SetAnimationData(pMeshData->GetMesh());

						g_Object.find(id)->second->GetScript<CMonsterScript>()->SetTerrain(
							g_Object.find(g_myid)->second->GetScript<CPlayerScript>()->GetTerrain()
						);
						g_Object.find(id)->second->SetID(id);
						g_Object.find(id)->second->GetScript<CMonsterScript>()->SetID(id);
						g_Object.find(id)->second->GetScript<CMonsterScript>()->SetHP(my_packet->hp);
					}

					//cout << my_packet->id<<"번 몬스터 -- " << my_packet->localVec.x << ", " << my_packet->localVec.z << endl;
					cout << id << "의 Monster Enter------  패킷 들어옴" << endl;

				}


			}
		}
	}
	break;
	case SC_PACKET_MOVE:
	{
		cout << "SC_PACKET_MOVE" << endl;
		sc_packet_move* packet = reinterpret_cast<sc_packet_move*>(ptr);
		int other_id = packet->id;

		if (other_id == g_myid)
		{
			g_Object.find(other_id)->second->Transform()->SetLocalPos(packet->localVec); //여기
		}
		else // 여기 브로드캐스팅하려면 다시수정
		{
			//추가
			if (0 != g_Object.count(other_id))
			{
				if (CheckObjType(other_id) == OBJECT_TYPE::CLIENT)
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
				else if (CheckObjType(other_id) == OBJECT_TYPE::MONSTER)
				{
				}
			}
		}
	}
	break;
	case SC_PACKET_MONSTER_MOVE:
	{
		cout << "SC_PACKET_MONSTER_MOVE" << endl;
		sc_packet_monster_automove* packet = reinterpret_cast<sc_packet_monster_automove*>(ptr);
		sc_packet_monster_automove* packet1;
		sc_packet_monster_automove* packet2;
		int monster_id = packet->id;
		if (g_Object.count(packet->id) == 0)break;
		if (g_Object.find(packet->id)->second == nullptr)break;

		if (packet->id == 1000)
		{
			packet1 = reinterpret_cast<sc_packet_monster_automove*>(ptr);
			g_Object.find(packet1->id)->second->GetScript<CMonsterScript>()->SetPacketMove(packet1);
			g_Object.find(packet1->id)->second->GetScript<CMonsterScript>()->SetDir((MONSTER_AUTOMOVE_DIR)packet1->eDir);

			g_Object.find(packet1->id)->second->Transform()->SetLocalPos(packet1->pos);
		}
		else
		{
			packet2 = reinterpret_cast<sc_packet_monster_automove*>(ptr);
			g_Object.find(packet2->id)->second->GetScript<CMonsterScript>()->SetPacketMove(packet2);
			g_Object.find(packet2->id)->second->GetScript<CMonsterScript>()->SetDir((MONSTER_AUTOMOVE_DIR)packet2->eDir);

			g_Object.find(packet2->id)->second->Transform()->SetLocalPos(packet2->pos);

		}
	}
	break;
	case SC_PACKET_STOP:
	{
		cout << "SC_PACKET_STOP" << endl;
		sc_packet_stop* packet = reinterpret_cast<sc_packet_stop*>(ptr);
		int other_id = packet->id;

		if (other_id == g_myid)
		{

		}
		else // 여기 브로드캐스팅하려면 다시수정
		{

			if (g_Object.count(other_id) == 0)break;
			if (g_Object.find(other_id)->second == nullptr)break;

			g_Object.find(g_myid)->second->GetScript<CPlayerScript>()->SetAnimation(other_id, Ani_TYPE::IDLE);

			g_Object.find(g_myid)->second->GetScript<CPlayerScript>()->SetOtherMovePacket__IsMoving(packet->isMoving);
		}
	}
	break;
	case SC_PACKET_LEAVE:
	{
		cout << "SC_PACKET_LEAVE" << endl;
		break;
		//cout << "leave packet" << endl;
		sc_packet_leave* my_packet = reinterpret_cast<sc_packet_leave*>(ptr);
		int other_id = my_packet->id;
		if (other_id == g_myid) {
		}
		else {
			if (0 != g_Object.count(other_id))
			{
				if (CheckObjType(other_id) == OBJECT_TYPE::CLIENT)
				{
					g_Object.find(other_id)->second->GetScript<CPlayerScript>()->DeleteObject(g_Object.find(other_id)->second);
					CEventMgr::GetInst()->update();
					//CEventMgr::GetInst()->update();
					g_Object.erase(other_id);
				}
				else if (CheckObjType(other_id) == OBJECT_TYPE::MONSTER)
				{
					
					if (!my_packet->isAttack)
					{
						g_Object.find(other_id)->second->GetScript<CMonsterScript>()->SetPacketMove(nullptr);
						g_Object.find(other_id)->second->GetScript<CMonsterScript>()->DeleteObject(g_Object.find(other_id)->second);
						CEventMgr::GetInst()->update();
						g_Object.erase(other_id);

					}
					else
					{
						g_Object.find(other_id)->second->GetScript<CMonsterScript>()->SetPacketMove(nullptr);
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
		cout << "SC_PACKET_ATTACK" << endl;
		sc_packet_attack* packet = reinterpret_cast<sc_packet_attack*>(ptr);
		int id = packet->id;
		if (id == g_myid)
		{

		}
		else
		{

			if (CheckObjType(id) == OBJECT_TYPE::MONSTER)
			{
				g_Object.find(packet->id)->second->GetScript<CMonsterScript>()->SetHP(packet->hp);
			}
		}
	}
	break;
	case SC_PACKET_ID:
	{
		cout << "SC_PACKET_ID" << endl;
		sc_packet_id* packet = reinterpret_cast<sc_packet_id*>(ptr);
		g_myid = packet->id;

		cout << "Send_ID_Packet My ID : " << g_myid << endl;
	}
	break;

	case SC_PACKET_ATTACKANI:
	{
		cout << "SC_PACKET_ATTACKANI" << endl;
		sc_packet_AttackAni* packet = reinterpret_cast<sc_packet_AttackAni*>(ptr);
		int id = packet->id;
		if (id == g_myid) {
		}
		else {
			if (0 != g_Object.count(id) && CheckObjType(id) == OBJECT_TYPE::CLIENT)
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
	case SC_PACKET_MONSTER_ANIMATION:
	{
		cout << "SC_PACKET_MONSTER_ANIMATION" << endl;
		sc_packet_Monster_Animation* packet = reinterpret_cast<sc_packet_Monster_Animation*>(ptr);
		int id = packet->id;
		CMonsterScript* monsterScr = g_Object.find(id)->second->GetScript<CMonsterScript>();

		if (g_Object.find(packet->id)->second != nullptr)
			g_Object.find(packet->id)->second->GetScript<CMonsterScript>()->SetAnimation(packet->aniType);

		
		//g_Object.find(packet->id)->second->GetScript<CMonsterScript>()->SetAnimation(packet->aniType);
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

