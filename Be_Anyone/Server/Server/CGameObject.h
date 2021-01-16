#pragma once
class CGameObject
{
public:
	CGameObject() = default;
	virtual ~CGameObject() {}
public: // 상속 ,, 함수
	// GET
	const int& GetID() { return m_id; }
	const int& GetAttackDamage() { return m_iAttack_Damage; }
	const int& GetHP() { return m_iHP; }
	const int& GetMaxHP() { return m_iMaxHP; }
	const int& GetX() { return m_iX; }
	const int& GetY() { return m_iY; }
	const int& GetZ() { return m_iZ; }
	char* GetName() { return m_name; }
	const ENUM_STATUS& GetStatus() { return m_status; }
	const SOCKET& GetSocket() { return m_s; }
	mutex& GetLock() { return m_lock; }
	const int& GetTime() { return m_iclinet_time; }
	const _tSector& GetSector() { return m_tSector; }

	// set
	void SetAttackDamage(const int& attackdamage) { m_iAttack_Damage = attackdamage; }
	void SetID(const int& id) { m_id = id; }
	void SetHP(const int& hp) { m_iHP = hp; }
	void SetX(const int& x) { m_iX = x; }
	void SetY(const int& y) { m_iY = y; }
	void SetStatus(const ENUM_STATUS& status) { m_status = status; }
	void SetZ(const int& z) { m_iZ = z; }
	void SetPos(const int& x, const int& y, const int& z) { m_iX = x; m_iY = y; m_iZ = z; }
	void SetMaxHP(const int& maxhp) { m_iMaxHP = maxhp; }
	void SetName(char* name) { strcpy_s(m_name, name); }
	void SetSector(const int& x, const int& y) { m_tSector.x = x; m_tSector.y = y; }
	void SetTime(const int& t) { m_iclinet_time = t; }

	void ChangeBisDead() { m_bisDead ? m_bisDead = false : m_bisDead = true; }
	void SetSocket_Zero() { m_s = 0; }
	void SetSocket(const SOCKET& s) { m_s = s; }

public:
	void Insert_Sector();
	void Change_Sector();
protected:
	int		m_iX, m_iY, m_iZ; // 좌표
	char  m_name[MAX_ID_LEN + 1];

	int		m_id;	// id
	int		m_iAttack_Damage = 5; // 공격력
	int		m_iHP = 100;  // 체력
	int		m_iMaxHP = 100;  // 체력

		// bool 
	bool m_bisDead = false;
	ENUM_STATUS m_status;
	SOCKET	m_s;
	_tSector m_tSector;
	mutex m_lock;

	int m_iclinet_time;
};

