#include "pch.h"
#include "CNetMgr.h"
#include "ItemMgr.h"
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
#include "ParticleScript.h"
#include "ParticleSystem.h"
#include "Quest.h"
#include "StaticUI.h"
int cnt = 0;
int stopcnt = 0;
bool tempbool = false;
OBJECT_TYPE CheckObjType(const uShort& id)
{
	if (id >= 0 && id < MAX_USER)return OBJECT_TYPE::CLIENT;
	else if (id >= START_MONSTER && id < END_MONSTER)return OBJECT_TYPE::MONSTER;
	else if (id == BOSS_ID)return OBJECT_TYPE::BOSS;
}

//const char ip[] = "192.168.0.11";
//const char ip[] = "192.168.0.13";
//const char ip[] = "192.168.0.13";
const char ip[] = "221.151.160.142";
const char office[] = "192.168.102.43";
const char KPUIP[] = "192.168.140.245";


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
	//cout << "nick name : ";
	cin >> tempName;
	sprintf_s(l_packet.name, tempName);
	strcpy_s(name, l_packet.name);
	//cout << "CS_LOGIN 보냄 -> "<<g_myid << endl;
	////cout << sizeof(l_packet) << endl << endl;

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
	cout << "-------------------------------" << endl;
	cout << "-------------------------------" << endl;
	cout << "Send_Move_Packet" << endl;
	cout << local.x << " , " << local.z << endl;
	cout << "-------------------------------" << endl;
	cout << "-------------------------------" << endl;


	Send_Packet(&p);
}

void CNetMgr::Send_Rotate_Packet(const uShort& id, const Vector3& rotate)
{
	cs_packet_rotate p;
	p.type = CS_ROTATE;
	p.size = sizeof(p);
	p.id = id;
	p.rotate = rotate;
	Send_Packet(&p);
}

void CNetMgr::Send_Stop_Packet(const bool& isMoving, const uShort& id)
{
	cs_packet_stop p;
	p.type = CS_STOP;
	p.size = sizeof(p);
	p.id = id;
	p.isMoving = isMoving;
	cout << p.id << "가 스톱패킷 서버로 보냄" << endl;
	cout << "*-*-*-*-*-*-*-*-*-*-*-*-*-*" << endl;
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

void CNetMgr::Send_ItemCreate_Paket(const Vector3& itemPos)
{
	cs_packet_ItemCreate_Packet p;
	p.type = CS_ITEMCREATE;
	p.size = sizeof(p);
	
	p.vPos = itemPos;
	Send_Packet(&p);
}

void CNetMgr::Send_ItemDelete_Paket(const Vector3& itemPos)
{
	cs_packet_ItemDelete_Packet p;
	p.type = CS_ITEMDELETE;
	p.size = sizeof(p);
	p.vPos = itemPos;
	Send_Packet(&p);
}

void CNetMgr::Send_Attack_Effect(const uShort& monster_id, const Vector3& pos, const bool isStart)
{
	cs_packet_Attack_Effect p;
	p.type = CS_ATTACKEFFECT; 
	p.size = sizeof(p);
	p.id = monster_id;
	p.pos = pos;
	p.isStart = isStart;
	Send_Packet(&p);
}


void CNetMgr::Send_Player_Animation_Packet(const uShort& user_id, const bool& isact, const Ani_TYPE& ani)
{
	cs_packet_Animation p;
	p.type = CS_ATTACK_ANIMATION;
	p.size = sizeof(p);
	p.id = user_id;
	p.anitype = (char)ani;
	p.isact = isact;
	Send_Packet(&p);
}

void CNetMgr::Send_Monster_Animation_Packet(const uShort& monster_id, const MONSTER_ANI_TYPE& aniType)
{
	cs_pcaket_Monster_Animation p;
	p.type = CS_MONSTER_ANIMATION;
	p.size = sizeof(p);
	p.id = monster_id;
	p.aniType = aniType;
	if (aniType == MONSTER_ANI_TYPE::IDLE)
	{
		p.isMoving = true;
	}
	else
	{
		p.isMoving = false;
	}
	Send_Packet(&p);

}

void CNetMgr::Send_MonsterDir_Packet(const uShort& monser_id, const Vector3& dir)
{
    cs_pcaket_MonsterDir p;
	p.type = CS_MONSTERDIR;
	p.size = sizeof(p);
	p.id = monser_id;
	p.dir = dir;
	Send_Packet(&p);
}

void CNetMgr::Send_Boss_State_Packet(const uShort& boss, const MONSTER_STATE& state, const BOSS_ATTACK& attackpattern)
{
	cs_pcaket_Boss_State p;
	p.type = CS_BOSS_STATE;
	p.size = sizeof(p);
	p.id = boss;
	p.aniState = state;
	p.attackstate = attackpattern;

	Send_Packet(&p);
}

void CNetMgr::Send_Boss_Turn(const uShort& boss, const Vector3& rotate)
{
	cs_pcaket_Boss_Turn p;
	p.type = CS_BOSS_TURN;
	p.size = sizeof(p);
	p.id = boss;
	p.rotate = rotate;

	Send_Packet(&p);
}

void CNetMgr::SetAnimation(int id, const Ani_TYPE& type)
{
	////cout << "------Setani -> " << (int)type << endl;
	
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
		if (recvbuf[1] == SC_PACKET_STOP)
		{
			int i = 0;
		}
		Process_Data(recvbuf, retbytesize);
	}

}



void CNetMgr::ProcessPacket(char* ptr)
{
	switch (ptr[1])
	{
	case SC_PACKET_LOGIN_OK:
	{
		////cout << "SC_PACKET_LOGIN_OK"<< endl;
		sc_packet_login_ok* p = reinterpret_cast<sc_packet_login_ok*>(ptr);

		m_pObj->Transform()->SetLocalPos(Vector3(p->localVec));

		g_Object.emplace(g_myid, m_pObj);
		g_Object.find(g_myid)->second->SetID(g_myid);
		g_Object.find(g_myid)->second->GetScript<CPlayerScript>()->initDeadReckoner();
	}
	break;
	case SC_PACKET_ENTER:
	{
		////cout << "SC_PACKET_ENTER" << endl;
		sc_packet_enter* my_packet = reinterpret_cast<sc_packet_enter*>(ptr);
		uShort id = my_packet->id;
		////cout << "enter packet" << endl;

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
					g_Object.find(id)->second = pMeshData->Instantiate();

					g_Object.find(id)->second->SetID(id);
					g_Object.find(id)->second->SetName(L"PlayerMale");
					g_Object.find(id)->second->FrustumCheck(false);
					g_Object.find(id)->second->Transform()->SetLocalPos(my_packet->localVec);
					g_Object.find(id)->second->Transform()->SetLocalScale(Vector3(1.f, 1.f, 1.f));
					g_Object.find(id)->second->Transform()->SetLocalRot(Vector3(0.f, my_packet->RotateY, 0.f));
					g_Object.find(id)->second->MeshRender()->SetDynamicShadow(true);
					g_Object.find(id)->second->AddComponent(new CPlayerScript);
					g_Object.find(id)->second->AddComponent(new CQuest);

					CSceneMgr::GetInst()->GetCurScene()->AddGameObject(L"Player", g_Object.find(id)->second, false);

					g_Object.find(id)->second->GetScript<CPlayerScript>()->Init();
					g_Object.find(id)->second->GetScript<CPlayerScript>()->SetTerrain(
						g_Object.find(g_myid)->second->GetScript<CPlayerScript>()->GetTerrain()
					);
					g_Object.find(id)->second->Transform()->SetLocalRot(my_packet->RotateY);
					g_Object.find(id)->second->GetScript<CPlayerScript>()->initDeadReckoner();

					for (auto& data : m_aniData)
					{
						g_Object.find(id)->second->GetScript<CPlayerScript>()->SetAnimationData(data);
					}
					// static ui cam 설정
					CGameObject* cam;
					vector<CGameObject*> objects;

					objects = CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"Default")->GetParentObj();

					vector<CGameObject*>::iterator iter = objects.begin();

					for (; iter != objects.end();) {
						if ((*iter)->GetName() == L"UiCam")
							cam = *iter;
						++iter;
					}
					g_Object.find(id)->second->GetScript<CPlayerScript>()->GetUIObj()->StaticUI()->SetCameraProj(cam->Camera());
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
					if (id % 2 == 0)
					{
						Ptr<CMeshData> pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\Monster\\monster3@walking.fbx", FBX_TYPE::MONSTER);
						Ptr<CMeshData> pIdleMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\Monster\\monster3@idle.fbx", FBX_TYPE::MONSTER);

						CGameObject* pObject = new CGameObject;
						g_Object.emplace(id, pObject);
						//
						g_Object.find(id)->second = pMeshData->Instantiate();
						g_Object.find(id)->second->SetID(id);
						
						g_Object.find(id)->second->SetName(L"FireMonster");
						g_Object.find(id)->second->MeshRender()->SetDynamicShadow(true);
						g_Object.find(id)->second->FrustumCheck(false);
						{
							int z = (int)(my_packet->localVec.z / g_Object.find(id)->second->Transform()->GetLocalScale().z);

							float fHeight = g_Object.find(g_myid)->second->GetScript<CPlayerScript>()->GetTerrain()->GetHeight(my_packet->localVec.x, my_packet->localVec.z, ((z % 2) != 0)) * 2.f;

							if (my_packet->localVec.y != fHeight)
								my_packet->localVec.y = fHeight;
						}
						g_Object.find(id)->second->Transform()->SetLocalPos(my_packet->localVec);
						cout << "패킷 받을때 Position    " << my_packet->localVec.x << ", " << my_packet->localVec.z << endl;

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
						g_Object.find(id)->second->GetScript<CMonsterScript>()->SetID(id);
						g_Object.find(id)->second->GetScript<CMonsterScript>()->SetHP(my_packet->hp);
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
						

						//g_netMgr.Send_MonsterDir_Packet(id, Vector3(0.f,0.f,1.f)/*g_Object.find(id)->second->Transform()->GetWorldDir(DIR_TYPE::UP)*/);
						//g_Object.find(id)->second->GetScript<CMonsterScript>()->SetisDirChange(true);


					}
					else
					{
						Ptr<CMeshData> pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\Monster\\TreantGuard@idle.fbx", FBX_TYPE::MONSTER);
						CGameObject* pObject = new CGameObject;
						g_Object.emplace(id, pObject);
						//
						g_Object.find(id)->second = pMeshData->Instantiate();
						g_Object.find(id)->second->SetID(id);
		
						g_Object.find(id)->second->SetName(L"GreenMonster");
						g_Object.find(id)->second->FrustumCheck(false);
						g_Object.find(id)->second->MeshRender()->SetDynamicShadow(true);
						g_Object.find(id)->second->Transform()->SetLocalScale(Vector3(1.f, 1.f, 1.f));//(1.0f, 1.0f, 1.0f));
						{
							int z = (int)(my_packet->localVec.z / g_Object.find(id)->second->Transform()->GetLocalScale().z);

							float fHeight = g_Object.find(g_myid)->second->GetScript<CPlayerScript>()->GetTerrain()->GetHeight(my_packet->localVec.x, my_packet->localVec.z, ((z % 2) != 0)) * 2.f;

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
						g_Object.find(id)->second->GetScript<CMonsterScript>()->SetID(id);
						g_Object.find(id)->second->GetScript<CMonsterScript>()->SetHP(my_packet->hp);
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
						//g_netMgr.Send_MonsterDir_Packet(id, Vector3(0.f, 0.f, -1.f)/*g_Object.find(id)->second->Transform()->GetWorldDir(DIR_TYPE::UP)*/);

					}
				}
			}
			else if (CheckObjType(id) == OBJECT_TYPE::BOSS)
			{
				if (0 == g_Object.count(id))
				{
					Ptr<CMeshData> pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\Monster\\Polygonal Alien Serpent@Idle.fbx", FBX_TYPE::MONSTER);
					CGameObject* pMonster = new CGameObject;
					pMonster = pMeshData->Instantiate();
					g_Object.emplace(id, pMonster);

				

					g_Object.find(id)->second->SetID(id);

					pMonster->SetName(L"BossMonster");
					pMonster->MeshRender()->SetDynamicShadow(true);
					pMonster->FrustumCheck(false);
					pMonster->Transform()->SetLocalScale(Vector3(2.f, 2.f, 2.f));//(1.0f, 1.0f, 1.0f));
					pMonster->Transform()->SetLocalRot(Vector3(0.f, 0.f, 0.f));
					{
						int z = (int)(my_packet->localVec.z / g_Object.find(id)->second->Transform()->GetLocalScale().z);

						float fHeight = g_Object.find(g_myid)->second->GetScript<CPlayerScript>()->GetTerrain()->GetHeight(my_packet->localVec.x, my_packet->localVec.z, ((z % 2) != 0)) * 2.f;

						if (my_packet->localVec.y != fHeight)
							my_packet->localVec.y = fHeight;
					}
					pMonster->Transform()->SetLocalPos(my_packet->localVec);
					pMonster->AddComponent(new CCollider);
					pMonster->Collider()->SetColliderType(COLLIDER_TYPE::MESH, L"BossMonster");
					pMonster->Collider()->SetBoundingBox(BoundingBox(pMonster->Transform()->GetLocalPos(), pMonster->MeshRender()->GetMesh()->GetBoundingBoxExtents()));
					pMonster->Collider()->SetBoundingSphere(BoundingSphere(pMonster->Transform()->GetLocalPos(), pMonster->MeshRender()->GetMesh()->GetBoundingSphereRadius()));

					// Script 설정
					pMonster->AddComponent(new CMonsterScript);
					g_Object.find(id)->second->GetScript<CMonsterScript>()->SetID(id);
					g_Object.find(id)->second->GetScript<CMonsterScript>()->SetHP(my_packet->hp);
					CSceneMgr::GetInst()->GetCurScene()->AddGameObject(L"Monster", pMonster, false);
					CMonsterScript* monsterScript = pMonster->GetScript<CMonsterScript>();
					monsterScript->SetMonsterType(MONSTER_TYPE::BOSS_MONSTER);
					monsterScript->SetTerrain(g_Object.find(g_myid)->second->GetScript<CPlayerScript>()->GetTerrain());
					monsterScript->SetMobTYpe(MOB_TYPE::BOSS);
					monsterScript->Init();
					//animation
					//idle
					monsterScript->SetAnimationData(pMeshData->GetMesh());
					//walk
					pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\Monster\\Polygonal Alien Serpent@Slither Forward Fast WO Root.fbx", FBX_TYPE::MONSTER);
					monsterScript->SetAnimationData(pMeshData->GetMesh());
					//dead
					pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\Monster\\Polygonal Alien Serpent@Die.fbx", FBX_TYPE::MONSTER);
					monsterScript->SetAnimationData(pMeshData->GetMesh());
					//attack
					pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\Monster\\Polygonal Alien Serpent@Bite Attack.fbx", FBX_TYPE::MONSTER);
					monsterScript->SetAnimationData(pMeshData->GetMesh());
					//damage
					pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\Monster\\Polygonal Alien Serpent@Defend.fbx", FBX_TYPE::MONSTER);
					monsterScript->SetAnimationData(pMeshData->GetMesh());
					//roar
					pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\Monster\\Polygonal Alien Serpent@Roar.fbx", FBX_TYPE::MONSTER);
					monsterScript->SetAnimationData(pMeshData->GetMesh());
					//left
					pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\Monster\\Polygonal Alien Serpent@Claw Attack Left.fbx", FBX_TYPE::MONSTER);
					monsterScript->SetAnimationData(pMeshData->GetMesh());
					//right
					pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\Monster\\Polygonal Alien Serpent@Claw Attack Right.fbx", FBX_TYPE::MONSTER);
					monsterScript->SetAnimationData(pMeshData->GetMesh());

					g_Object.find(id)->second->GetScript<CMonsterScript>()->SetTerrain(
						g_Object.find(g_myid)->second->GetScript<CPlayerScript>()->GetTerrain()
					);
					g_Object.find(id)->second->SetID(id);
					g_Object.find(id)->second->GetScript<CMonsterScript>()->SetID(id);
					g_Object.find(id)->second->GetScript<CMonsterScript>()->SetHP(my_packet->hp);
				}
			}
		}

	}
	break;
	case SC_PACKET_MOVE:
	{
		////cout << "SC_PACKET_MOVE" << endl;
		sc_packet_move* packet = reinterpret_cast<sc_packet_move*>(ptr);
		int other_id = packet->id;

		if (other_id == g_myid)
		{

		//	//cout << "SC_PACKET_MOVE  자신에게 보내는 패킷 횟수"<<cnt++ << endl;
			//cout << "여기니?" << endl;
			g_Object.find(other_id)->second->Transform()->SetLocalPos(packet->localVec);
		}
		else // 여기 브로드캐스팅하려면 다시수정
		{
			//추가
			if (0 != g_Object.count(other_id))
			{
				if (CheckObjType(other_id) == OBJECT_TYPE::CLIENT)
				{
					//cout << " Move 들어옴    \t"<<(int)packet->dir << endl;
					system_clock::time_point end = system_clock::now();
					nanoseconds rtt = duration_cast<nanoseconds>(end - packet->Start);
					g_Object.find(other_id)->second->GetScript<CPlayerScript>()->SetBisFrist(true);
					g_Object.find(other_id)->second->GetScript<CPlayerScript>()->SetOtherMovePacket__IsMoving(true);

					if (packet->isMoving)
						g_Object.find(other_id)->second->GetScript<CPlayerScript>()->SetAnimation(Ani_TYPE::WALK_F);
					else
						g_Object.find(other_id)->second->GetScript<CPlayerScript>()->SetAnimation(Ani_TYPE::IDLE);
					/*cout << "\t\t\t\t세팅하는 dirvec" << packet->dirVec.x << " - " << packet->dirVec.z << endl;
					cout << "패킷받을때 패킷   : \t" << packet->localVec.x << ", " << packet->localVec.z << endl;
					cout << "패킷받을때 클라상태   : \t" << g_Object.find(other_id)->second->Transform()->GetLocalPos().x << ", " << 
						g_Object.find(other_id)->second->Transform()->GetLocalPos().z << endl;
					cout << "(float)rtt.count : " << (float)rtt.count() * 0.000000001 << endl;*/
					g_Object.find(other_id)->second->GetScript<CPlayerScript>()->SetOtherMovePacket(packet, (float)rtt.count()*0.000000001);
					g_Object.find(other_id)->second->GetScript<CPlayerScript>()->Set_InterpolationCnt_Zero();
					g_Object.find(other_id)->second->GetScript<CPlayerScript>()->SetisBezeir(false);
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
		////cout << "SC_PACKET_MONSTER_MOVE" << endl;
		sc_packet_monster_automove* packet = reinterpret_cast<sc_packet_monster_automove*>(ptr);
		

		int monster_id = packet->id;
		if (g_Object.count(packet->id) == 0)break;
		if (g_Object.find(packet->id)->second == nullptr)break;

		if (CheckObjType(monster_id) == OBJECT_TYPE::MONSTER) {
			g_Object.find(packet->id)->second->GetScript<CMonsterScript>()->SetPacketMove(packet);
			g_Object.find(packet->id)->second->GetScript<CMonsterScript>()->SetisDirChange(true);
			g_Object.find(packet->id)->second->GetScript<CMonsterScript>()->SetisMoving(true);
			g_Object.find(packet->id)->second->GetScript<CMonsterScript>()->SetDir((MONSTER_AUTOMOVE_DIR)packet->eDir);
			g_Object.find(packet->id)->second->Transform()->SetLocalPos(packet->pos);

		}
		if (CheckObjType(monster_id) == OBJECT_TYPE::BOSS) {

			g_Object.find(packet->id)->second->GetScript<CMonsterScript>()->SetIsPacketWorldDir(true);
			g_Object.find(packet->id)->second->GetScript<CMonsterScript>()->SetPacketWorldDir(packet->worldDir);

			cout << "Boss가 받는패킷 : " << packet->worldDir.x << ", " << packet->worldDir.z << endl;
			cout << "----------------------" << endl;
			g_Object.find(packet->id)->second->GetScript<CMonsterScript>()->SetPacketMove(packet);
			g_Object.find(packet->id)->second->GetScript<CMonsterScript>()->SetisDirChange(true);
			g_Object.find(packet->id)->second->GetScript<CMonsterScript>()->SetisMoving(true);
			g_Object.find(packet->id)->second->GetScript<CMonsterScript>()->SetDir((MONSTER_AUTOMOVE_DIR)packet->eDir);
			g_Object.find(packet->id)->second->Transform()->SetLocalPos(packet->pos);

		}
		

	}
	break;
	case SC_ROTATE:
	{
		sc_packet_rotate* packet = reinterpret_cast<sc_packet_rotate*>(ptr);
		if (packet->id != g_myid && g_Object.count(packet->id) != 0)
			g_Object.find(packet->id)->second->Transform()->SetLocalRot(packet->rotate);
	}
	break;

	case SC_PACKET_STOP:
	{
		////cout << "SC_PACKET_STOP" << endl;
		sc_packet_stop* packet = reinterpret_cast<sc_packet_stop*>(ptr);
		int other_id = packet->id;
		
		if (other_id == g_myid)
		{

			if (g_Object.count(other_id) == 0)break;
			if (g_Object.find(other_id)->second == nullptr)break;

			g_Object.find(g_myid)->second->GetScript<CPlayerScript>()->SetAnimation(Ani_TYPE::IDLE);

			g_Object.find(g_myid)->second->GetScript<CPlayerScript>()->SetPacketMoving(packet->isMoving);
			//g_Object.find(g_myid)->second->GetScript<CPlayerScript>()->SetOtherMovePacket__IsMoving(packet->isMoving);
			g_Object.find(g_myid)->second->GetScript<CPlayerScript>()->SetisBezeir(true);
		}
		else // 여기 브로드캐스팅하려면 다시수정
		{

			if (g_Object.count(other_id) == 0)break;
			if (g_Object.find(other_id)->second == nullptr)break;

			/*cout << "----------------------" << endl;
			stopcnt++;
			cout << other_id<<"번 플레이어의 Stop Packet 받음\t"<< stopcnt << endl;
			cout << "----------------------" << endl;*/

			g_Object.find(other_id)->second->GetScript<CPlayerScript>()->SetAnimation(Ani_TYPE::IDLE);
			/*//cout << "------------------------" << endl;
			//cout << "Moving False setting\t\t"<<other_id << endl;
			if (packet->isMoving)//cout << "true" << endl;
			else //cout << "false" << endl;
			//cout << "------------------------" << endl;*/
			g_Object.find(other_id)->second->GetScript<CPlayerScript>()->SetPacketMoving(false);
			//g_Object.find(other_id)->second->GetScript<CPlayerScript>()->SetOtherMovePacket__IsMoving(false);
			g_Object.find(other_id)->second->GetScript<CPlayerScript>()->SetisBezeir(true);
		}
	}
	break;
	case SC_PACKET_LEAVE:
	{
		////cout << "SC_PACKET_LEAVE" << endl;
		////cout << "leave packet" << endl;
		sc_packet_leave* my_packet = reinterpret_cast<sc_packet_leave*>(ptr);
		int other_id = my_packet->id;
		if (other_id == g_myid) {
		}
		else {
			if (0 != g_Object.count(other_id))
			{
				if (CheckObjType(other_id) == OBJECT_TYPE::CLIENT)
				{
					g_Object.find(other_id)->second->GetScript<CPlayerScript>()->SetPacketMoving(false);
					g_Object.find(other_id)->second->GetScript<CPlayerScript>()->DeleteObject(g_Object.find(other_id)->second);
					CEventMgr::GetInst()->update();
					g_Object.erase(other_id);
				}
				else if (CheckObjType(other_id) == OBJECT_TYPE::MONSTER)
				{
					if (!my_packet->isAttack)
					{
						g_Object.find(other_id)->second->GetScript<CMonsterScript>()->SetPacketMove(nullptr);
						g_Object.find(other_id)->second->GetScript<CMonsterScript>()->DeleteObject(g_Object.find(other_id)->second);
						//g_Object.find(other_id)->second->GetScript<CPlayerScript>()->SetPacketMoving(false);
						CEventMgr::GetInst()->update();
						g_Object.erase(other_id);

					}
					else
					{
						g_Object.find(other_id)->second->GetScript<CMonsterScript>()->SetPacketMove(nullptr);
						g_Object.find(other_id)->second->GetScript<CMonsterScript>()->SetBisAttack(my_packet->isAttack);
						//g_Object.find(other_id)->second->GetScript<CPlayerScript>()->SetPacketMoving(false);
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
		////cout << "SC_PACKET_ATTACK" << endl;
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
		////cout << "SC_PACKET_ID" << endl;
		sc_packet_id* packet = reinterpret_cast<sc_packet_id*>(ptr);
		g_myid = packet->id;

	}
	break;

	case SC_PACKET_ATTACKANI:
	{
		////cout << "SC_PACKET_ATTACKANI" << endl;
		sc_packet_AttackAni* packet = reinterpret_cast<sc_packet_AttackAni*>(ptr);
		int id = packet->id;
		if (id == g_myid) {
		}
		else {
			if (0 != g_Object.count(id) && CheckObjType(id) == OBJECT_TYPE::CLIENT)
			{
				if (packet->isAttack)
				{
					
					switch ((Ani_TYPE)packet->anitype)
					{
					case Ani_TYPE::ATTACK:
						g_Object.find(g_myid)->second->GetScript<CPlayerScript>()->SetAnimation(Ani_TYPE::ATTACK);
						g_SoundList.find(Sound_Type::HIT);
						break;
					case Ani_TYPE::DAMAGE:
						g_Object.find(g_myid)->second->GetScript<CPlayerScript>()->SetAnimation(Ani_TYPE::DAMAGE);
						break;
					case Ani_TYPE::PICK_UP:
						g_Object.find(g_myid)->second->GetScript<CPlayerScript>()->SetAnimation(Ani_TYPE::PICK_UP);
						g_SoundList.find(Sound_Type::GET_COIN);
						break;
					default:
						break;
					}
				}
				else
				{
					g_Object.find(g_myid)->second->GetScript<CPlayerScript>()->SetAnimation(Ani_TYPE::IDLE);
				}
			}
		}
	}
	break;
	case SC_PACKET_MONSTER_ANIMATION:
	{
		////cout << "SC_PACKET_MONSTER_ANIMATION" << endl;
		sc_packet_Monster_Animation* packet = reinterpret_cast<sc_packet_Monster_Animation*>(ptr);
		int id = packet->id;
		if (g_Object.count(id) == 0)break; 
		if (g_Object.find(packet->id)->second == nullptr)break;
		CMonsterScript* monsterScr = g_Object.find(id)->second->GetScript<CMonsterScript>();

		
			g_Object.find(packet->id)->second->GetScript<CMonsterScript>()->SetAnimation(packet->aniType);

		if (MONSTER_ANI_TYPE::IDLE != packet->aniType)
			g_Object.find(packet->id)->second->GetScript<CMonsterScript>()->SetisMoving(false);
		else
			g_Object.find(packet->id)->second->GetScript<CMonsterScript>()->SetisMoving(true);

		if (packet->id == BOSS_ID)
		{
			if (MONSTER_ANI_TYPE::ATTACK == packet->aniType)
			{
				//g_Object.find(packet->id)->second->GetScript<CMonsterScript>()->SetIsPunch(true);

			}
			if (packet->aniType == MONSTER_ANI_TYPE::ATTACK_LEFT)
			{

			}
			if (packet->aniType == MONSTER_ANI_TYPE::ATTACK_RIGHT)
			{

			}
			if (packet->aniType == MONSTER_ANI_TYPE::ROAR)
			{
				// isroar
				//g_Object.find(packet->id)->second->GetScript<CMonsterScript>()->SetIsPunch(true);

			}

		}
		else
		{
			if (MONSTER_ANI_TYPE::ATTACK == packet->aniType)
			{
				g_Object.find(packet->id)->second->GetScript<CMonsterScript>()->SetIsPunch(true);
				////cout << "\t\t\t 강제로 넣은 userid == " << packet->otherid << endl;
				//g_Object.find(packet->id)->second->GetScript<CMonsterScript>()->SetPlayer(g_Object.find(packet->otherid)->second);

				//if (g_Object.find(packet->id)->second->GetName() == L"GreenMonster")
				//	g_Object.find(packet->id)->second->GetScript<CMonsterScript>()->TurnToPlayer(MOB_TYPE::GREEN);
				//else
				//	g_Object.find(packet->id)->second->GetScript<CMonsterScript>()->TurnToPlayer(MOB_TYPE::YELLOW);
			}

		}

		// 플레이어에게 공격

		

		if (MONSTER_ANI_TYPE::DEAD == packet->aniType)
		{
			monsterScr->SetPacketDead(true);
		}
	}
	break;
	case SC_ITEMCREATE:
	{
		
		// 아이탬 생성부 - 효림
		sc_packet_ItemCreate_Packet* packet = reinterpret_cast<sc_packet_ItemCreate_Packet*>(ptr);
		packet->vid;// vector<char> monster id
		packet->vPos;//vecotr3 몬스터 position

		CGameObject* pItem;
		for (int i = 0; i < 3; ++i) {
			pItem = new CGameObject;
			pItem->SetName(L"Item1");
			pItem->FrustumCheck(true);
			pItem->AddComponent(new CTransform);
			pItem->AddComponent(new CMeshRender);
			//pItem->AddComponent(new CDummyItemScript);
			pItem->AddComponent(new CCollider);
			pItem->Transform()->SetLocalPos(Vector3(0.f, 0.f, 0.f));
			pItem->Transform()->SetLocalScale(Vector3(200.f, 200.f, 200.f));
			pItem->Transform()->SetLocalRot(Vector3(0.f, 0.f, 0.f));
			pItem->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh"));
			pItem->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"TestMtrl"));
			pItem->Collider()->SetColliderType(COLLIDER_TYPE::MESH, L"Branch");
			pItem->Collider()->SetBoundingBox(BoundingBox(pItem->Transform()->GetLocalPos(), pItem->MeshRender()->GetMesh()->GetBoundingBoxExtents()));
			pItem->Collider()->SetBoundingSphere(BoundingSphere(pItem->Transform()->GetLocalPos(), 30.f));

			CItemMgr::GetInst()->SetItemObj(pItem);		
			CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"Item")->AddGameObject(pItem);
		}
		CItemMgr::GetInst()->SetItemPos(packet->vPos);
		CItemMgr::GetInst()->SetIsMake(true);
		CItemMgr::GetInst()->SetReservePaket(true);
	}
	break;
	case SC_ITEMDELETE:
	{
		// 아이템 삭제부 - 효림
		sc_packet_ItemDelete_Packet* packet = reinterpret_cast<sc_packet_ItemDelete_Packet*>(ptr);
		packet->vPos;// vector3 item position
		////cout << "다시 받을 때 item pos: " << packet->vPos.x << "\t" << packet->vPos.y << "\t" << packet->vPos.z << endl;
		
		CItemMgr::GetInst()->DeleteItemObj(packet->vPos);
	}
	break;

	case SC_ATTACKEFFECT:
	{
		sc_packet_AttackEfefct_Packet* packet = reinterpret_cast<sc_packet_AttackEfefct_Packet*>(ptr);//effect  생성 위치 
		// ====================
			// Particle Object 생성
			// 떄린놈 이름이 FireMonster 일 때 이 파티클을 생성한다.
			// ====================
		CGameObject* pObject = new CGameObject;
		pObject->SetName(L"Particle");
		pObject->AddComponent(new CTransform);
		pObject->AddComponent(new CParticleSystem);
		pObject->Particlesystem()->SetFrequency(2.f);
		pObject->Particlesystem()->SetType(false);
		pObject->Particlesystem()->SetMaxParticle(10);
		pObject->AddComponent(new CParticleScript);
		pObject->GetScript<CParticleScript>()->SetLifeTime(pObject->Particlesystem()->GetMaxLifeTime());
		pObject->FrustumCheck(false);
		pObject->Transform()->SetLocalPos(packet->vPos);

		CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"Default")->AddGameObject(pObject);

	}
	break;
	case SC_PACKET_BOSS_STATE:
	{
		sc_packet_boss_state* packet = reinterpret_cast<sc_packet_boss_state*>(ptr);//effect  생성 위치 
		g_Object.find(packet->id)->second->GetScript<CMonsterScript>()->SetBossState(packet->aniState);
		if (packet->attackstate != BOSS_ATTACK::END)
		{
			g_Object.find(packet->id)->second->GetScript<CMonsterScript>()->SetAttackPattern(packet->attackstate);
		}
	}
	break;
	case SC_PACKET_BOSS_TURN:
	{
		sc_packet_boss_turn* packet = reinterpret_cast<sc_packet_boss_turn*>(ptr);//effect  생성 위치 

		g_Object.find(packet->id)->second->Transform()->SetLocalRot(packet->rotate);
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
			if (packet_buffer[1] == SC_PACKET_STOP)
			{
				cout << "Stop packet size : " << (int)packet_buffer[0] << endl;
			}
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

