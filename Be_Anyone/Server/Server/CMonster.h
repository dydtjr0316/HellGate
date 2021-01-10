#pragma once
#include "CGameObject.h"
class CMonster : public CGameObject
{
public:
	CMonster() {}	//멤버 변수 초기화 해야됨 .. 기획따라
	~CMonster() {}

public:

private:
	// 전부 값 수정
	// 필수 
	int		m_id;	// id
	int		m_iAttack_Damage = 5; // 공격력
	int		m_iHP = 100;  // 체력
	int		m_iMaxHP = 100;  // 체력
	int		m_ilevel = 1;	// 레벨
	
public:

	// 최적화 할때 필요없는거 전부 삭제 하기 
	// server
	mutex vl;
	unordered_set<int> view_list;

	// int
	int   m_prev_size;
	int	  m_iclinet_time;

	// bool 
	bool m_bisDead = false;

	// char
	char  m_packe_buf[MAX_PACKET_SIZE];
	char  m_name[MAX_ID_LEN + 1];

	// struct
	ENUM_STATUS m_status;
	_tSector m_iCSector;	// sector 수정

	// 스크립트 추가 구현
	//lua_State* L;



};

