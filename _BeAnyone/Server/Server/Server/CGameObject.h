#pragma once
class CSector;
class CGameObject
{
public:
	CGameObject();
	virtual ~CGameObject() {}
public: // 상속 ,, 함수
	// GET
	const uShort& GetID() { return m_id; }
	//const float& GetX() { return m_iX; }
	//const float& GetY() { return m_iY; }
	//const float& GetZ() { return m_iZ; }
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
	Vector3& GetLocalPosVector() { return m_v3LocalPosVector; }
	Vector3& GetDirVector() { return m_v3DirVector; }
	Vector3& GetRoatateVector() { return m_v3RotateVector; }
	float GetRotateY() { return m_v3RotateVector.y; }
	float GetSpeed() { return m_fspeed; }
	system_clock::time_point GetHalfRTT() { return m_halfRTT; }
	bool GetIsMoving() { return m_bisMoving; }
	void SetDeadReckoningPacket(cs_packet_move* p);
	cs_packet_move* GetDeadReckoningPacket() { return m_deadReckoning_Packet; }
	OBJECT_TYPE GetType() { return m_Type; }


	// set
	void SetID(const uShort& id) { m_id = id; }
	
	void SetPosX(const float& x) {		m_v3LocalPosVector.x = x; }
	void SetPosY(const float& y) {		m_v3LocalPosVector.y = y; }
	void SetPosZ(const float& z) {		m_v3LocalPosVector.z = z; }
	void SetPosV(const Vector3& pos) {	m_v3LocalPosVector = pos; }
	void SetPosV(const float& x, const float& y, const float& z);

	void SetDirX(const float& x) {		m_v3DirVector.x = x; }
	void SetDirY(const float& y) {		m_v3DirVector.y = y; }
	void SetDirZ(const float& z) {		m_v3DirVector.z = z; }
	void SetDirV(const Vector3& dir) {	m_v3DirVector = dir; }
	void SetDirV(const float& x, const float& y, const float& z);

	void SetRotateX(const float& x) { m_v3RotateVector.x = x; }
	void SetRotateY(const float& y) { m_v3RotateVector.y = y; }
	void SetRotateZ(const float& z) { m_v3RotateVector.z = z; }
	void SetRotateV(const Vector3& rotate) { m_v3RotateVector = rotate; }
	void SetRotateV(const float& x, const float& y, const float& z);

	void SetClientTime(const int& t) { m_iclinet_time = t; }
	void SetPrev_Size(const int& size) { m_prev_size = size; }
	void SetName(char* name) { strcpy_s(m_name, name); }
	void SetStatus(const OBJSTATUS& status) { m_status = status; }
	void SetSector(const int& x, const int& z) { m_tSector.x = x; m_tSector.z = z; }
	void SetSector(const _tSector& tsec) { m_tSector = tsec; }

	void SetSocket_Zero() { m_s = 0; }
	void SetSocket(const SOCKET& s) { m_s = s; }

	void SetSpeed(const float& speed) { m_fspeed = speed; }
	void SetHalfRTT(const system_clock::time_point hrtt) { m_halfRTT = hrtt; }
	void SetIsMoving(const bool& isMoving);
	void SetType(const OBJECT_TYPE& type) { m_Type = type; }
public:
	void Insert_Sector();
	void Change_Sector(const _tSector& old_sector);
	void Change_Sector();

	
protected:
	// int
	uShort		m_id;	// id
	//float		m_iX, m_iY, m_iZ; // 좌표
	int   m_prev_size;

	bool m_bisMoving;
	// CHAR
	char  m_name[MAX_ID_LEN + 1]{"",};
	char  m_packet_buf[MAX_PACKET_SIZE]{};
	int	  m_iclinet_time;
	float m_fspeed;

	// 구조체
	OBJSTATUS m_status;
	SOCKET	m_s;
	_tSector m_tSector;
	mutex m_lock;
	EXOVER m_Exover;

	Vector3 m_v3LocalPosVector;
	Vector3 m_v3DirVector;

	Vector3 m_v3RotateVector;

	system_clock::time_point m_halfRTT;

	cs_packet_move* m_deadReckoning_Packet;
	OBJECT_TYPE m_Type;
};

