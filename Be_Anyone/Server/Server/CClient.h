#pragma once


class CClient : public CGameObject
{
public:
	CClient() {};
	~CClient() {};
public:
	// GET
	const int& GetID() { return m_id; }
	const int& GetMaxEXP() { return m_iMax_exp; }
	const int& GetEXP() { return m_iExp; }
	const int& GetAttackDamage() { return m_iAttack_Damage; }
	const int& GetHP() { return m_iHP; }
	const int& GetMaxHP() { return m_iMaxHP; }
	const int& GetLevel() { return m_ilevel; }
	const bool& GetBisLevelUP() { return m_bisLevelUp; }

	// SET
	void SetID(const int &id) { m_id = id; }
	void SetMaxEXP(const int &mexp) { m_iMax_exp = mexp; }
	void SetEXP(const int& exp) { m_iExp = exp; }
	void SetAttackDamage(const int& attackdamage) { m_iAttack_Damage = attackdamage; }
	void SetHP(const int& hp) { m_iHP = hp; }
	void SetMaxHP(const int& maxhp) { m_iMaxHP = maxhp; }
	void SetLevel(const int& level) { m_ilevel = level; }

	// bool ������ true���� fasle�� false���� true�� �ٲ��ִ� �Լ�
	void ChangeBisLevelUP() { m_bisLevelUp ? m_bisLevelUp = false : m_bisLevelUp = true; }
	void ChangeBisDead() { m_bisDead ? m_bisDead = false : m_bisDead = true; }

public:
	// �þ�ó��
	void Insert_Sector();
	void Change_Sector();
	vector<unordered_set<int>>& Search_Sector();

private:
	// ���� �� ����
	// �ʼ� 
	int		m_id;	// id
	int		m_iMax_exp = 100; // �ִ� ����ġ
	int		m_iExp = 0; // ����ġ


	int		m_iAttack_Damage = 5; // ���ݷ�
	int		m_iHP = 100;  // ü��
	int		m_iMaxHP = 100;  // ü��
	int		m_ilevel = 1;	// ����
	bool	m_bisLevelUp = false;	// ������
	// server
	SOCKET	m_s;
	mutex	m_cl;
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
	EXOVER  m_recv_over;
	PLAYER_STATUS m_status;
	_tSector m_tCector;	// sector ����

	// ��ũ��Ʈ �߰� ����
	//lua_State* L;
};

