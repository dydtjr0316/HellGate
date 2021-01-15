#pragma once
class CNetMgr
{
public:
	CNetMgr() {}
	~CNetMgr() {}

public:
	void Send_Packet(const int& id, void* packet);
	void Send_Chat_Packet(const int& toClient, const int& id, const char* mess, const size_t& size);
	void Send_LevelUP_Packet(const int& id);
	void Send_Attack_Packet(const int& id);
	void Send_LoginOK_Packet(const int& id);
	void Send_Enter_Packet(const int& user_id, const int& other_id);
	void Send_Leave_Packet(const int& user_id, const int& other_id);
	void Send_Move_Packet(const int& user_id, const int& mover_id);

public:
	void Random_Move_NPC(const int& id);
	void Do_Attack() {/*충돌체크 생각하고 구현*/};
	void Do_Move(const int& user_id, const int& dir);
	void Disconnect(const OBJID& eID,const int& id);
};

