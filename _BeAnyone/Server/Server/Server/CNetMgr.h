#pragma once
#include "CSendMgr.h"
#include "CMediatorMgr.h"
class CMediatorMgr;
class CGameObject;
class CMonster;
class CClient;
class CSendMgr;
class CNetMgr
{
public:
	CNetMgr() { m_pSendMgr = new CSendMgr; m_pMediator = new CMediatorMgr; }
	~CNetMgr() {}

public:		// ��Ŷ ���ź�
	void Process_Packet(const uShort& user_id, char* buf);
	void Recv_Packet_Construct(const uShort& user_id, const int& io_byte);
public:		// ���� ��ü ������Ʈ��
	void Random_Move_NPC(const int& Monster_id);
	void Random_Move_Monster(const int& Monster_id);
	void Do_Attack(const uShort& attacker, const uShort& victim);
	void Do_Move(const uShort& user_id, const char& dir, Vector3& localVec, const float& rotateY);
	void Do_Stop(const uShort& user_id, const bool& isMoving);
	void Kill_Monster(const uShort& monster_id);
	void Disconnect(const uShort& user_id);
	void Enter_Game(const uShort& user_id, char name[]);

public:		// ��ü ���� �� �Ǻ��� 


public:		// thread ������
	void Worker_Thread();
	void DeadReckoning_Thread();
	void Timer_Worker();
	void WakeUp_NPC(const uShort& id);
	void WakeUp_Monster(const uShort& id);
	void Add_Timer(const uShort& obj_id, const int& status, system_clock::time_point t);

public:		// ��Ŷ ���ۺ�
	CSendMgr* GetSendMgr() { return m_pSendMgr; }
	CMediatorMgr* GetMediatorMgr() { return m_pMediator; }

private:
	CSendMgr* m_pSendMgr;
	CMediatorMgr* m_pMediator;
public:
	void error_display(const char* msg, int err_no);
};

