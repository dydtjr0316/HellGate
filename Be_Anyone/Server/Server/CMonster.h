#pragma once
class CMonster : public CGameObject
{
public:
	CMonster() {};	// 멤버 변수 초기화 해야됨 .. 기획따라
	~CMonster() {};

public:
	// GET
	const int& GetID() { return m_id; }
	const int& GetMaxEXP() { return m_iMax_exp; }
	const int& GetEXP() { return m_iExp; }
	const int& GetAttackDamage() { return m_iAttack_Damage; }
	const int& GetHP() { return m_iHP; }
	const int& GetMaxHP() { return m_iMaxHP; }
	const int& GetLevel() { return m_ilevel; }
	const bool GetBisLevelUP() { return m_bisLevelUp; }

	// SET
	void SetID(const int& id) { m_id = id; }
	void SetMaxEXP(const int& mexp) { m_iMax_exp = mexp; }
	void SetEXP(const int& exp) { m_iExp = exp; }
	void SetAttackDamage(const int& attackdamage) { m_iAttack_Damage = attackdamage; }
	void SetHP(const int& hp) { m_iHP = hp; }
	void SetMaxHP(const int& maxhp) { m_iMaxHP = maxhp; }
	void SetLevel(const int& level) { m_ilevel = level; }

	// bool 변수를 true에서 fasle로 false에서 true로 바꿔주는 함수
	void ChangeBisLevelUP() { m_bisLevelUp ? m_bisLevelUp = false : m_bisLevelUp = true; }
	void ChangeBisDead() { m_bisDead ? m_bisDead = false : m_bisDead = true; }

protected:
	// 전부 값 수정
	// 필수 
	int		m_id;	// id
	int		m_iMax_exp = 100; // 최대 경험치
	int		m_iExp = 0; // 경험치


	int		m_iAttack_Damage = 5; // 공격력
	int		m_iHP = 100;  // 체력
	int		m_iMaxHP = 100;  // 체력
	int		m_ilevel = 1;	// 레벨
	bool	m_bisLevelUp = false;	// 레벨업


	// 변경 및 삭제
	//short		m_iFirstX;
	//short		m_iFirstY;
	//short	m_iHP_Regen = (m_iHP / 10); // 체젠
	//short	m_monster_exp = 2;
	//short	m_Monster_level = 0;
	//bool	m_isFighter = false;
public:

	// 최적화 할때 필요없는거 전부 삭제 하기 
	// server
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
	CURRENT_SECTOR m_iCSector;	// sector 수정

	// 스크립트 추가 구현
	//lua_State* L;



};

