#pragma once
class CFRect;
class CRect
{
public:
	CRect();
	~CRect() {}
	CRect(const int& x, const int& y, const int& w, const int& h);
	void initialize(const int& x, const int& y, const int& w, const int& h);
	void update();

public:	// Get
	int	  GetX() { return m_ix; }
	int	  GetY() { return m_iy; }
	int	  GetW() { return m_iw; }
	int	  GetH() { return m_ih; }
	float GetRight() const { return m_iRight; }
	float GetBottom() const { return m_iBottom; }
	float GetLeft() const { return m_iLeft; }
	float GetTop() const { return m_iTop; }

public: // Set
	void SetPos(const int& x, const int& y, const int& w, const int& h);

public:
	// ®G®G Ãæµ¹®G
	bool ColliderRect(const CRect& target)const;
	bool ColliderRect(const CFRect& target)const;
	bool CollidePoint(const int& x, const int& y) const;

private:
	//tRect m_tdest;
	int m_ix, m_iy;
	int m_iw, m_ih;

	int m_iRight, m_iBottom, m_iLeft, m_iTop, m_iCenter_Arr[2];
};

