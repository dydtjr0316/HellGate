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
	const bool& GetIsMoving() { return m_bisMoving; }
	const MONSTER_AUTOMOVE_DIR& GetDir() { return m_dir; }

	// SET
	void SetHP(const int& hp) { m_iHP = hp; }
	void SetMaxHP(const int& maxhp) { m_iMaxHP = maxhp; }
	void SetAttack_Damage(const int& damage) { m_iAttack_Damage = damage; }
	void SetFirstXY(const int& x, const int& y) { m_iFirstX = x; m_iFirstY = y; }
	void SetBisDead(const bool& isdead) { m_bisDead = isdead; }
	void SetBisMoving(const bool* isMove) { m_bisMoving = isMove; }
	void SetDir(const MONSTER_AUTOMOVE_DIR& dir) { m_dir = dir; }


private:
	// 전부 값 수정
	// 필수 
	int		m_id = 0;	// id
	int		m_iAttack_Damage = 5; // 공격력
	int		m_iHP = 100;  // 체력
	int		m_iMaxHP = 100;  // 체력
	int		m_iFirstX = 0;
	int		m_iFirstY = 0;
	//int   m_prev_size = 0;
	bool	m_bisMoving = false;
	MONSTER_AUTOMOVE_DIR		m_dir = MONSTER_AUTOMOVE_DIR::IDLE;

	// bool 
	bool m_bisDead = false;

	char  m_name[MAX_ID_LEN + 1]{};
};

