#pragma once
#include "CGameObject.h"
class CMonster : public CGameObject
{
public:
	CMonster() {}	//��� ���� �ʱ�ȭ �ؾߵ� .. ��ȹ����
	~CMonster() {}

public:

private:
	// ���� �� ����
	// �ʼ� 
	int		m_id;	// id
	int		m_iAttack_Damage = 5; // ���ݷ�
	int		m_iHP = 100;  // ü��
	int		m_iMaxHP = 100;  // ü��
	int		m_ilevel = 1;	// ����
	
public:

	// ����ȭ �Ҷ� �ʿ���°� ���� ���� �ϱ� 
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
	_tSector m_iCSector;	// sector ����

	// ��ũ��Ʈ �߰� ����
	//lua_State* L;



};

