#pragma once
class CSendMgr
{
public:		//player
	void Send_LevelUP_Packet(const uShort& id);
	void Send_Attacked_Packet_Monster(const uShort& attacker, const uShort& monster_id);
	void Send_Attacked_Packet_Client(const uShort& client_id);
	void Send_ID_Packet(const uShort& user_id);
	void Send_LoginOK_Packet(const uShort& id);
	void Send_Enter_Packet(const uShort& user_id, const uShort& other_id);
	void Send_Leave_Packet(const uShort& user_id, const uShort& other_id, const bool& isAttack = false);
	void Send_Move_Packet(const uShort& user_id, const uShort& mover_id, const char& dir);
	void Send_Stop_Packet(const uShort& user_id, const uShort& mover_id, const bool& isMoving);

	//°¡¶ó
	void Send_Attack_Animation_Packet(const uShort& user_id, const uShort& attackerid, const bool& isAttack);
	void Send_Monster_Animation_Packet(const uShort& monster_id, const uShort& user_id, const MONSTER_ANI_TYPE& aniType);
public:		// monster
	void Send_Monster_Move_Packet(const uShort& user_id, const uShort& mover_id, const char& dir);
	void Send_Npc_Move_Packet(const uShort& user_id, const uShort& mover_id, const char& dir);

public:
	void Send_Packet(const uShort& id, void* packet);
};

