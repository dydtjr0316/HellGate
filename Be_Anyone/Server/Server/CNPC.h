#pragma once
class CNPC : public CGameObject
{
public:
	CNPC() {}
	~CNPC() {}
public:
	//get
	const int& GetX() { return m_iX; }
	const int& GetY() { return m_iY; }
	const int& GetZ() { return m_iZ; }
	const int& GetID() { return m_id; }

	//set
	void SetID(const int& id) { m_id = id; }
	void SetX(const int& x) { m_iX = x; }
	void SetY(const int& y) { m_iY = y; }
	void SetZ(const int& z) { m_iZ = z; }
	void SetStatus(const ENUM_STATUS& status) { m_status = status; }

	void SetName(const char* name) {strcpy_s(m_name, name);}

private:
	int		m_iX, m_iY, m_iZ; // ÁÂÇ¥
	int		m_id;
	char  m_name[MAX_ID_LEN + 1];
	ENUM_STATUS m_status;

};

