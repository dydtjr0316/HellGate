#pragma once
class CGameObject
{
public:
	CGameObject() = default;
	virtual ~CGameObject() {}
public: // 상속 ,, 함수
	// GET
	const int& GetID() { return m_id; }
	const int& GetX() { return m_iX; }
	const int& GetY() { return m_iY; }
	const int& GetZ() { return m_iZ; }
	const int& GetClientTime() { return m_iclinet_time; }
	int Get_Prev_Size() { return m_prev_size; }
	//
	char* GetName() { return m_name; }
	//
	const OBJSTATUS& GetStatus() { return m_status; }
	const SOCKET& GetSocket() { return m_s; }
	mutex& GetLock() { return m_lock; }
	const _tSector GetSector() { return m_tSector; }
	EXOVER& GetExover() { return m_Exover; }
	void ZeroMemory_recv_over() { ZeroMemory(&m_Exover.over, sizeof(m_Exover.over)); }
	char* Get_Packet_buf() { return m_packet_buf; }

	// set
	void SetID(const int& id) { m_id = id; }
	void SetX(const int& x) { m_iX = x; }
	void SetY(const int& y) { m_iY = y; }
	void SetZ(const int& z) { m_iZ = z; }
	void SetPos(const int& x, const int& y, const int& z) { m_iX = x; m_iY = y; m_iZ = z; }
	void SetClientTime(const int& t) { m_iclinet_time = t; }
	void SetPrev_Size(const int& size) { m_prev_size = size; }
	//
	void SetName(char* name) { strcpy_s(m_name, name); }
	//
	void SetStatus(const OBJSTATUS& status) { m_status = status; }
	void SetSector(const int& x, const int& y) { m_tSector.x = x; m_tSector.y = y; }
	void SetSector(const _tSector& tsec) { m_tSector = tsec; }

	void SetSocket_Zero() { m_s = 0; }
	void SetSocket(const SOCKET& s) { m_s = s; }

public:
	void Insert_Sector();
	void Change_Sector(const _tSector& old_sector);
	vector<unordered_set<int>> Search_Sector();
	
protected:
	// int
	int		m_id;	// id
	int		m_iX, m_iY, m_iZ; // 좌표
	int   m_prev_size;

	// CHAR
	char  m_name[MAX_ID_LEN + 1];
	char  m_packet_buf[MAX_PACKET_SIZE];
	int	  m_iclinet_time;

	// 구조체
	OBJSTATUS m_status;
	SOCKET	m_s;
	_tSector m_tSector;
	mutex m_lock;
	EXOVER m_Exover;

};

