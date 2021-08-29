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
	void Send_Rotate_Packet(const uShort& moverid, const uShort& userid, const Vector3& rotate);

	//°¡¶ó
	void Send_Attack_Animation_Packet(const uShort& user_id, const uShort& attackerid, const bool& isAttack, const Ani_TYPE& type);
	void Send_Monster_Animation_Packet(const uShort& monster_id, const uShort& user_id, const MONSTER_ANI_TYPE& aniType, const uShort& id = -1);
public:		// monster
	void Send_Monster_Move_Packet(const uShort& user_id, const uShort& mover_id, const char& dir);
	void Send_Boss_State_Packet(const uShort& user_id, const uShort& boss, const MONSTER_STATE& state, const BOSS_ATTACK& attackpattern = BOSS_ATTACK::END);
	void Send_Boss_Turn_Packet(const uShort& user_id, const uShort& boss, const Vector3 rotate);
	

public:
	void Send_ItemCreate_Packet(const uShort& user_id, const Vector3& pos);
	void Send_ItemDelete_Packet(const uShort& user_id, const Vector3& pos);

	void Send_Attack_Effect(const uShort& user_id, const Vector3& effectPos);

public:
	void Send_Packet(const uShort& id, void* packet);
};

