#pragma once
#include "CGameObject.h"
class CMonster : public CGameObject
{
public:
	CMonster() {}	//멤버 변수 초기화 해야됨 .. 기획따라
	~CMonster() {}

public:
	// Get
	const int& GetHP() { return m_iHP; }
	const int& GetMaxHP() { return m_iMaxHP; }
	const int& GetAttackDamage() { return m_iAttack_Damage; }
	const int& GetFirstX() { return m_iFirstX; }
	const int& GetFirstY() { return m_iFirstY; }

	const MONSTER_TYPE& GetType() { return m_Type; }
	int Get_Prev_Size() { return m_prev_size; }
	char* Get_Packet_buf() { return m_packet_buf; }
	unordered_set<int>& GetViewList() { return view_list; }

	// SET

	void SetHP(const int& hp) { m_iHP = hp; }
	void SetMaxHP(const int& maxhp) { m_iMaxHP = maxhp; }
	void SetAttack_Damage(const int& damage) { m_iAttack_Damage = damage; }
	void SetFirstXY(const int& x, const int& y) { m_iFirstX = x; m_iFirstY = y; }
	void SetType(const MONSTER_TYPE& t) { m_Type = t; }
	void SetPrev_Size(const int& size) { m_prev_size = size; }
	void SetBisDead(const bool& isdead) { m_bisDead = isdead; }

	//view list
	void InsertViewList(const int& id) { view_list.emplace(id); }
private:
	// 전부 값 수정
	// 필수 
	int		m_id = 0;	// id
	int		m_iAttack_Damage = 5; // 공격력
	int		m_iHP = 100;  // 체력
	int		m_iMaxHP = 100;  // 체력
	int		m_iFirstX = 0;
	int		m_iFirstY = 0;
	int   m_prev_size = 0;

	unordered_set<int> view_list;

	// bool 
	bool m_bisDead = false;

	// char
	char  m_packet_buf[MAX_PACKET_SIZE]{};
	char  m_name[MAX_ID_LEN + 1]{};

	// struct
	MONSTER_TYPE m_Type = MONSTER_TYPE::DEFAULT;
};

