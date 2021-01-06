#pragma once


class CClient : public CGameObject
{
public:
	CClient() {};
	~CClient() {};
public:
	// GET
	int GetID() { return m_id; }
	short GetMaxEXP() { return m_iMax_exp; }
	short GetEXP() { return m_iExp; }
	short GetAttackDamage() { return m_iAttack_Damage; }
	short GetHP() { return m_iHP; }
	short GetMaxHP() { return m_iMaxHP; }
	short GetLevel() { return m_ilevel; }
	bool GetBisLevelUP() { return m_bisLevelUp; }

	// SET
	void SetID(const int &id) { m_id = id; }
	void SetMaxEXP(const short &mexp) { m_iMax_exp = mexp; }
	void SetEXP(const short & exp) { m_iExp = exp; }
	void SetAttackDamage(const short& attackdamage) { m_iAttack_Damage = attackdamage; }
	void SetHP(const short& hp) { m_iHP = hp; }
	void SetMaxHP(const short& maxhp) { m_iMaxHP = maxhp; }
	void SetLevel(const short& level) { m_ilevel = level; }

	// bool ������ true���� fasle�� false���� true�� �ٲ��ִ� �Լ�
	void ChangeBisLevelUP() { m_bisLevelUp ? m_bisLevelUp = false : m_bisLevelUp = true; }
	void ChangeBisDead() { m_bisDead ? m_bisDead = false : m_bisDead = true; }
	
protected:
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


	// ���� �� ����
	//short		m_iFirstX;
	//short		m_iFirstY;
	//short	m_iHP_Regen = (m_iHP / 10); // ü��
	//short	m_monster_exp = 2;
	//short	m_Monster_level = 0;
	//bool	m_isFighter = false;
public:
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
	CURRENT_SECTOR m_iCSector;	// sector ����

	// ��ũ��Ʈ �߰� ����
	//lua_State* L;
};

