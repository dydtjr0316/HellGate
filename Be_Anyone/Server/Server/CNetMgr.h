#pragma once
typedef unordered_map<int, CGameObject*> OBJLIST;
typedef OBJLIST::iterator OBJITER;


class CNetMgr
{
public:
	CNetMgr() {}
	~CNetMgr() {}

public:
	// 패킷
	void Send_Packet(const OBJID& eID, const int& id, void* packet);
	//void Send_Chat_Packet(const int& toClient, const int& id, const char* mess, const size_t& size);
	void Send_LevelUP_Packet(const int& id);
	void Send_Attacked_Packet_Monster(const const int& id);
	void Send_Attacked_Packet_Client(const const int& id);
	void Send_LoginOK_Packet(const int& id);
	void Send_Enter_Packet(const OBJID& user_eID, const int& user_id, const OBJID& other_eID, const int& other_id );
	void Send_Leave_Packet(const OBJID& user_eID, const int& user_id, const int& other_id);
	void Send_Move_Packet(const OBJID& user_eID, const int& user_id, const OBJID& mover_eID, const int& mover_id);

public:
	void Random_Move_NPC(const int& id);
	void Random_Move_Monster(const int& id);
	void Do_Attack(const int& user_id);
	void Do_Move(const int& user_id, const int& dir);
	void Disconnect(const int& user_id);
	void Enter_Game(const int& user_id, char name[]);
	void Process_Packet(const int& user_id, char* buf);
	void Recv_Packet_Construct(const int& user_id, const int& io_byte);

	bool IS_Near_Client(const OBJID& other_eID,const int& other_id, const OBJID& user_eID, const int& user_id);
public:
	vector<unordered_multiset<int>> Search_Sector(const int& user_id);
	bool is_near(const OBJID& p1eID, const int& p1, const OBJID& p2eID, const int& p2);
	void Insert_Sector(const OBJID& eID, const int& id);

public:
	void Init_Client();
	void Init_Monster();
	void Init_NPC();
	void error_display(const char* msg, int err_no);


	void Add_Timer(const int& obj_id, const ENUMOP& status, system_clock::time_point t);
public:
	void Worker_Thread();
	void Timer_Worker();

	void WakeUp_NPC(const int& id);
	void WakeUp_Monster(const int& id);
public:
	// 중개자
	OBJLIST* GetObjlist(const OBJID& eID) { return &(m_ObjLst[eID]); }
	void Add(CGameObject* pObj, const OBJID& eID, const int& id);
	CGameObject* Find(const OBJID& eID, const int& id);
	void Delete_lst(const OBJID& eID);
	void Delete_Obj(const OBJID& eID, const int& id);
	int Count(const OBJID& eID, const int& id);
	int Size(const OBJID& eID);


private:
	OBJLIST m_ObjLst[OBJID::END];
};

