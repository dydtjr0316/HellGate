#pragma once

class CNetMgr
{
public:
	CNetMgr() {}
	~CNetMgr() {}
	

public:		// 菩哦 傈价何
	void Send_Packet(const uShort& id, void* packet);
	void Send_LevelUP_Packet(const uShort& id);
	void Send_Attacked_Packet_Monster(const uShort& attacker, const uShort& monster_id);
	void Send_Attacked_Packet_Client(const uShort& client_id);
	void Send_ID_Packet(const uShort& user_id);
	void Send_LoginOK_Packet(const uShort& id);
	void Send_Enter_Packet(  const uShort& user_id,   const uShort& other_id );
	void Send_Leave_Packet(  const uShort& user_id, const uShort& other_id, const bool& isAttack = false);
	void Send_Move_Packet(const uShort& user_id, const uShort& mover_id, const char& dir);
	void Send_Stop_Packet(const uShort& user_id, const uShort& mover_id, const bool& isMoving);

	//啊扼
	void Send_Attack_Animation_Packet(const uShort& user_id, const uShort& attackerid, const bool& isAttack);
public:		// 菩哦 荐脚何
	void Process_Packet(const uShort& user_id, char* buf);
	void Recv_Packet_Construct(const uShort& user_id, const int& io_byte);
public:		// 角力 按眉 诀单捞飘何
	//void Random_Move_NPC(const int& id);
	//void Random_Move_Monster(const int& id);
	void Do_Attack(const uShort& attacker, const uShort& victim);
	void Do_Move(const uShort& user_id, const char& dir, Vector3& localVec, const float& rotateY);
	void Do_Stop(const uShort& user_id, const bool& isMoving);
	
	void Kill_Monster(const uShort& monster_id);
	void Disconnect(const uShort& user_id);
	void Enter_Game(const uShort& user_id, char name[]);
public:		// 矫具贸府 累己何
	bool is_near(  const uShort& p1,  const uShort& p2);
public:		// 按眉 积己 关 魄喊何 
	void Init_Client();
	void Init_Monster();
	void Init_NPC();
	bool IsClient(const uShort& id);
	bool IsMonster(const uShort& id);
	bool IsNpc(const uShort& id);
public:
	void error_display(const char* msg, int err_no);
public:		// thread 包府何
	void Worker_Thread();
	void DeadReckoning_Thread();
	void Timer_Worker();
	void WakeUp_NPC(const uShort& id);
	void WakeUp_Monster(const uShort& id);
	void Add_Timer(const uShort& obj_id, const int& status, system_clock::time_point t);

public:		//  object 吝俺磊 包府何
	void Add(CGameObject* pObj, const uShort& id);
	CGameObject* Find(const uShort& id);
	void Delete_Obj(const uShort& id);
	const size_t Count(const uShort& id);
	const size_t Size();

public:
	void ReckonerAdd(const uShort& id);
	int ReckonerFind(const uShort& id);
	void Delete_Reckoner(const uShort& id);
	const size_t ReckonerCount(const uShort& id);
	const size_t ReckonerSize();

};

