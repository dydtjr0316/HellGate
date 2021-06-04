#pragma once

class CNetMgr
{
public:
	CNetMgr() {}
	~CNetMgr() {}

public:		// ��Ŷ ���ۺ�
	void Send_Packet(const int& id, void* packet);
	void Send_LevelUP_Packet(const int& id);
	void Send_Attacked_Packet_Monster(const int& monster_id);
	void Send_Attacked_Packet_Client(const int& client_id);
	void Send_ID_Packet(const int& user_id);
	void Send_LoginOK_Packet(const int& id);
	void Send_Enter_Packet(  const int& user_id,   const int& other_id );
	void Send_Leave_Packet(  const int& user_id, const int& other_id);
	void Send_Move_Packet(const int& user_id, const int& mover_id, const char& dir);
	void Send_Stop_Packet(const unsigned short& user_id, const int& mover_id, const bool& isMoving);
public:		// ��Ŷ ���ź�
	void Process_Packet(const int& user_id, char* buf);
	void Recv_Packet_Construct(const int& user_id, const int& io_byte);
public:		// ���� ��ü ������Ʈ��
	//void Random_Move_NPC(const int& id);
	//void Random_Move_Monster(const int& id);
	void Do_Attack(const int& user_id);
	void Do_Move(const int& user_id, const char& dir, Vector3& localVec, const float& rotateY);
	void Do_Stop(const unsigned short& user_id, const bool& isMoving);
	void Disconnect(const int& user_id);
	void Enter_Game(const int& user_id, char name[]);
public:		// �þ�ó�� �ۼ���
	bool is_near(  const int& p1,  const int& p2);
public:		// ��ü ���� �� �Ǻ��� 
	void Init_Client();
	void Init_Monster();
	void Init_NPC();
	bool IsClient(const int& id);
	bool IsMonster(const int& id);
	bool IsNpc(const int& id);
public:
	void error_display(const char* msg, int err_no);
public:		// thread ������
	void Worker_Thread();
	void DeadReckoning_Thread();
	void Timer_Worker();
	void WakeUp_NPC(const int& id);
	void WakeUp_Monster(const int& id);
	void Add_Timer(const int& obj_id, const int& status, system_clock::time_point t);

public:		//  object �߰��� ������
	void Add(CGameObject* pObj, const int& id);
	CGameObject* Find(const int& id);
	void Delete_Obj(const int& id);
	const size_t Count(const int& id);
	const size_t Size();

public:
	void ReckonerAdd(const int& id);
	int ReckonerFind(const int& id);
	void Delete_Reckoner(const int& id);
	const size_t ReckonerCount(const int& id);
	const size_t ReckonerSize();

};

