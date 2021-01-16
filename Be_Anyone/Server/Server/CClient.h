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
	void ZeroMemory_recv_over() { ZeroMemory(&m_recv_over.over, sizeof(m_recv_over.over)); }

	void SetPrev_Size(const int& size) { m_prev_size = size; }
	EXOVER& GetExover() { return m_recv_over; }
	int Get_Prev_Size() { return m_prev_size; }
	char* Get_Packet_buf() { return m_packet_buf; }
	vector<unordered_set<int>>& Search_Sector();
	void SetFirstXY(const int& x, const int& y) { m_iFirstX = x; m_iFirstY = y; }

private:
	// ���� �� ����
	// �ʼ� 

	// int
	int		m_iFirstX = 0;
	int		m_iFirstY = 0;
	int		m_iMax_exp = 100; // �ִ� ����ġ
	int		m_iExp = 0; // ����ġ
	int		m_ilevel = 1;	// ����
	// ���� ����
	int   m_prev_size;
	int	  m_iclinet_time;
							//bool
	bool	m_bisLevelUp = false;	// ������

	// char
	char  m_packet_buf[MAX_PACKET_SIZE];

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

