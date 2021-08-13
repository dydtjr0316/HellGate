#pragma once
#include "../Client/ServerHeader.h"
#include "Mesh.h"
#include "Ptr.h"

class CScene;
class CGameObject;
class CMesh;
class CToolCamScript;

class CNetMgr
{
public:
	CNetMgr();
	~CNetMgr() {};
public:		// obj객체 설정부
	void SetObj(CGameObject* obj) { m_pObj = new CGameObject; m_pObj = obj; }
	void SetCamObj(CGameObject* obj) { m_pCamObj = new CGameObject; m_pCamObj = obj; }
	void SetAniData(Ptr<CMesh> _aniData) { m_aniData.push_back(_aniData); }

public:		// 네트워크 연결부
	void Connect();
public:		// 패킷 처리부
	void Recevie_Data();
	void ProcessPacket(char* ptr);
	void Process_Data(char* net_buf, size_t& io_byte);
	void Enter_Player(const int& id);
public:		// 패킷 전송부
	void Send_Packet(void* _packet);
	void Send_LogIN_Packet();
	void Send_Move_Packet(unsigned const char& dir, const Vector3& local);
	void Send_Move_Packet(unsigned const char& dir, const Vector3& local, const float& rotateY);
	void Send_Move_Packet(unsigned const char& dir, const Vector3& local, const Vector3& dirVec,const float& rotateY,
		const system_clock::time_point& startTime, const float& delta, const bool& isMoving);
	void Send_Stop_Packet( const bool& isMoving);
	void Send_Attack_Packet(const uShort& victim_id);
	void Send_MonsterDead_Packet(const uShort& monster_id);
	// item
	void Send_ItemCreate_Paket(const Vector3& itemPos, const vector<int>& itemid);
	void Send_ItemDelete_Paket(const Vector3& itemPos);

	// 가라
	void Send_Player_Animation_Packet(const uShort& user_id, const bool& isAttack);
	void Send_Monster_Animation_Packet(const uShort& monster_id, const MONSTER_ANI_TYPE& aniType);
public:		// animation
	void SetAnimation(int id, const Ani_TYPE& type);

public:
	void err_quit(const char* msg);
private:
	OVERLAPPED m_overlapped;
	CGameObject* m_pObj;
	CGameObject* m_pCamObj;
	vector<Ptr<CMesh>>		m_aniData;
};

