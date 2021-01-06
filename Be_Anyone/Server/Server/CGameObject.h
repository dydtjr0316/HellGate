#pragma once
class CGameObject
{
public: // 상속 ,, 함수
	// get
	const int& GetX() { return m_iX; }
	const int& GetY() { return m_iY; }
	const int& GetZ() { return m_iZ; }
	
	// set
	void SetX(const int& x) { m_iX = x; }
	void SetY(const int& y) { m_iY = y; }
	void SetZ(const int& z) { m_iZ = z; }
	void SetXY(const int& x, const int& y) { m_iX = x; m_iY = y; }

protected:
	int		m_iX, m_iY, m_iZ; // 좌표



};

