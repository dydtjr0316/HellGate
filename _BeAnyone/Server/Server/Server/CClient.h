#pragma once
#include "CGameObject.h"
#include "Template.h"

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
	//const float& GetFirstX() { return m_iFirstX; }// 삭제
	//const float& GetFirstY() { return m_iFirstY; }// 삭제
	const Vector3& GetFirstPosVector() { return m_v3FirstPos; }
	const int& GetEXP() { return m_iExp; }
	const int& GetMaxEXP() { return m_iMax_exp; }
	const bool& GetBisLevelUP() { return m_bisLevelUp; }
	unordered_set<uShort>& GetViewList() { return view_list; }
	const float& GetRefreshPacketCnt() { m_frpc; }

	// SET
	void SetLevel(const int& level) { m_ilevel = level; }
	void SetHP(const int& hp) { m_iHP = hp; }
	void SetMaxHP(const int& maxhp) { m_iMaxHP = maxhp; }
	void SetAttack_Damage(const int& damage) { m_iAttack_Damage = damage; }
	//void SetFirstXY(const float& x, const float& y) { m_iFirstX = x; m_iFirstY = y; }	// 삭제
	void SetFirstPos(const Vector3& posV) { m_v3FirstPos = posV; }
	void SetFirstPos(const float& x, const float& y, const float& z);
	void SetEXP(const int& exp) { m_iExp = exp; }
	void SetMaxEXP(const int& mexp) { m_iMax_exp = mexp; }
	void SetBisDead(const bool& isdead) { m_bisDead = isdead; }
	void SetRefreshPacketCnt_Zero() { m_frpc = 0.f; }
	void CountRefreshPacketCnt() { m_frpc += DeltaTime; }

	//view list
	void InsertViewList(const int& id) { view_list.emplace(id); }
	void SetViewList(const unordered_set<uShort>& view) { view_list = view; }

private:
	// int
	int		m_ilevel = 1;	// 레벨
	int		m_iHP = 100;  // 체력
	int		m_iMaxHP = 100;  // 체력int		m_iFirstX = 0;
	int		m_iAttack_Damage = 5; // 공격력
	/*float		m_iFirstX = 0;
	float		m_iFirstY = 0;*/		// 삭제
	int		m_iExp = 0; // 경험치
	int		m_iMax_exp = 100; // 최대 경험치
							//bool
	bool	m_bisLevelUp = false;	// 레벨업
	bool m_bisDead = false;
	float m_frpc = 0.f;// Refresh Packet Cnt

	// char
	// server
	unordered_set<uShort> view_list;
	Vector3 m_v3FirstPos;




};

