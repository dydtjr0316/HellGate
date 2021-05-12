#pragma once

class CNetMgr
{
public:
	CNetMgr() {}
	~CNetMgr() {}

public:		// 菩哦 傈价何
	void Send_Packet(const int& id, void* packet);
	void Send_LevelUP_Packet(const int& id);
	void Send_Attacked_Packet_Monster(const int& monster_id);
	void Send_Attacked_Packet_Client(const int& client_id);
	void Send_ID_Packet(const int& user_id);
	void Send_LoginOK_Packet(const int& id);
	void Send_Enter_Packet(  const int& user_id,   const int& other_id );
	void Send_Leave_Packet(  const int& user_id, const int& other_id);
	void Send_Move_Packet(const int& user_id, const int& mover_id, const char& dir);
	void Send_Roate_Packet(const int& user_id, const int& mover_id, const char& dir);
public:		// 菩哦 荐脚何
	void Process_Packet(const int& user_id, char* buf);
	void Recv_Packet_Construct(const int& user_id, const int& io_byte);
public:		// 角力 按眉 诀单捞飘何
	//void Random_Move_NPC(const int& id);
	//void Random_Move_Monster(const int& id);
	void Do_Attack(const int& user_id);
	void Do_Move(const int& user_id, const char& dir, Vector3& localVec);
	void Do_Rotate(const int& user_id, const char& dir, float& rotateY);
	void Disconnect(const int& user_id);
	void Enter_Game(const int& user_id, char name[]);
public:		// 矫具贸府 累己何
	bool is_near(  const int& p1,  const int& p2);
public:		// 按眉 积己 关 魄喊何 
	void Init_Client();
	void Init_Monster();
	void Init_NPC();
	bool IsClient(const int& id);
	bool IsMonster(const int& id);
	bool IsNpc(const int& id);
public:
	void error_display(const char* msg, int err_no);
public:		// thread 包府何
	void Worker_Thread();
	void Timer_Worker();
	void WakeUp_NPC(const int& id);
	void WakeUp_Monster(const int& id);
	void Add_Timer(const int& obj_id, const int& status, system_clock::time_point t);

public:		// 吝俺磊 包府何
	void Add(CGameObject* pObj, const int& id);
	CGameObject* Find(const int& id);
	void Delete_Obj(const int& id);
	const size_t Count(const int& id);
	const size_t Size();
};

