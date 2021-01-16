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
	void Disconnect(const OBJID& eID,const int& id);
	void Enter_Game(const int& user_id, char name[]);

	bool IS_Near_Client(const OBJID& other_eID,const int& other_id, const OBJID& user_eID, const int& user_id);

	bool is_near(const OBJID& p1eID, const int& p1, const OBJID& p2eID, const int& p2);
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

