#pragma once
#include "CGameObject.h"

class CClient : public CGameObject
{
public:
	CClient() {}
	~CClient() {}

public:
	bool Is_SameStatus(OBJSTATUS status);
public:
	// 시야처리

public:
	// Get
	const int& GetLevel() { return m_ilevel; }
	const int& GetHP() { return m_iHP; }
	const int& GetMaxHP() { return m_iMaxHP; }
	const int& GetAttackDamage() { return m_iAttack_Damage; }
	const int& GetFirstX() { return m_iFirstX; }
	const int& GetFirstY() { return m_iFirstY; }
	const int& GetEXP() { return m_iExp; }
	const int& GetMaxEXP() { return m_iMax_exp; }
	const bool& GetBisLevelUP() { return m_bisLevelUp; }
	unordered_set<int>& GetViewList() { return view_list; }

	// SET
	void SetLevel(const int& level) { m_ilevel = level; }
	void SetHP(const int& hp) { m_iHP = hp; }
	void SetMaxHP(const int& maxhp) { m_iMaxHP = maxhp; }
	void SetAttack_Damage(const int& damage) { m_iAttack_Damage = damage; }
	void SetFirstXY(const int& x, const int& y) { m_iFirstX = x; m_iFirstY = y; }
	void SetEXP(const int& exp) { m_iExp = exp; }
	void SetMaxEXP(const int& mexp) { m_iMax_exp = mexp; }
	void SetBisDead(const bool& isdead) { m_bisDead = isdead; }

	//view list
	void InsertViewList(const int& id) { view_list.emplace(id); }

private:
	// int
	int		m_ilevel = 1;	// 레벨
	int		m_iHP = 100;  // 체력
	int		m_iMaxHP = 100;  // 체력int		m_iFirstX = 0;
	int		m_iAttack_Damage = 5; // 공격력
	int		m_iFirstX = 0;
	int		m_iFirstY = 0;
	int		m_iExp = 0; // 경험치
	int		m_iMax_exp = 100; // 최대 경험치
							//bool
	bool	m_bisLevelUp = false;	// 레벨업
	bool m_bisDead = false;

	// char
	// server
	unordered_set<int> view_list;


};

