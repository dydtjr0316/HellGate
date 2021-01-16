#pragma once
#include "CGameObject.h"

class CClient : public CGameObject
{
public:
	CClient() {}
	~CClient() {}
public:
	bool Is_SameStatus(ENUM_STATUS status);
public:
	// �þ�ó��
	void Insert_Sector();
	void Change_Sector();
	vector<unordered_set<int>>& Search_Sector();

private:
	// ���� �� ����
	// �ʼ� 

	// int
	int		m_iMax_exp = 100; // �ִ� ����ġ
	int		m_iExp = 0; // ����ġ
	int		m_ilevel = 1;	// ����
	// ���� ����
	int   m_prev_size;
	int	  m_iclinet_time;
							//bool
	bool	m_bisLevelUp = false;	// ������

	// char
	char  m_packe_buf[MAX_PACKET_SIZE];

	// struct
	EXOVER  m_recv_over;
	_tSector m_tCector;	// sector ����
	
	// server

	mutex	m_cl;
	mutex   vl;
	unordered_set<int> view_list[OBJID::END];

	// ��ũ��Ʈ �߰� ����
	//lua_State* L;
public:
	// Get
	const int& GetMaxEXP() { return m_iMax_exp; }
	const int& GetEXP() { return m_iExp; }
	const int& GetLevel() { return m_ilevel; }
	const int& GetClientTime() { return m_iclinet_time; }
	const bool& GetBisLevelUP() { return m_bisLevelUp; }

	unordered_set<int>& GetViewList(const OBJID& eID) { return view_list[eID]; }
	

	mutex& GetClientLock() { return m_cl; }

	// SET
	void SetMaxEXP(const int& mexp) { m_iMax_exp = mexp; }
	void SetEXP(const int& exp) { m_iExp = exp; }
	void SetLevel(const int& level) { m_ilevel = level; }

	// bool ������ true���� fasle�� false���� true�� �ٲ��ִ� �Լ�
	void ChangeBisLevelUP() { m_bisLevelUp ? m_bisLevelUp = false : m_bisLevelUp = true; }

};

