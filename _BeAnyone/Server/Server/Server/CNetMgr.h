#pragma once



class CNetMgr
{
public:
	CNetMgr() {}
	~CNetMgr() {}

public:
	// 패킷
	void Send_Packet(const int& id, void* packet);
	//void Send_Chat_Packet(const int& toClient, const int& id, const char* mess, const size_t& size);
	void Send_LevelUP_Packet(const int& id);
	void Send_Attacked_Packet_Monster(const int& monster_id);
	void Send_Attacked_Packet_Client(const int& client_id);
	void Send_ID_Packet(const int& user_id);
	void Send_LoginOK_Packet(const int& id);
	void Send_Enter_Packet(  const int& user_id,   const int& other_id );
	void Send_Leave_Packet(  const int& user_id, const int& other_id);
	void Send_Move_Packet(const int& user_id, const int& mover_id, const char& dir);
	void Send_Move_Packet(  const int& user_id,   const int& mover_id);
	//
public:
	void Random_Move_NPC(const int& id);
	void Random_Move_Monster(const int& id);
	void Do_Attack(const int& user_id);
	void Do_Move(const int& user_id, const char& dir);//아래꺼 완성하고 잘돌아가면 없애도 되는 코드 : 용석
	void Do_Move(const int& user_id, const char& dir, Vector3& localVec, Vector3& dirVec);
	void Disconnect(const int& user_id);
	void Enter_Game(const int& user_id, char name[]);
	void Process_Packet(const int& user_id, char* buf);
	void Recv_Packet_Construct(const int& user_id, const int& io_byte);

public:
	bool is_near(  const int& p1,  const int& p2);

public:
	void Init_Client();
	void Init_Monster();
	void Init_NPC();

	bool IsClient(const int& id);
	bool IsMonster(const int& id);
	bool IsNpc(const int& id);

	void error_display(const char* msg, int err_no);


	void Add_Timer(const int& obj_id, const int& status, system_clock::time_point t);
public:
	void Worker_Thread();
	void Timer_Worker();

	void WakeUp_NPC(const int& id);
	void WakeUp_Monster(const int& id);
public:
	// 중개자

	void Add(CGameObject* pObj, const int& id);
	CGameObject* Find(const int& id);
	void Delete_Obj(const int& id);
	const size_t Count(const int& id);
	const size_t Size();

};

