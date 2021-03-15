#pragma once
class CRect;
class CFRect
{
public:
	CFRect();
	~CFRect() {}
	void initialize(const float& x, const float& y, const float& w, const float& h);
	void update();

public:	// Get
	float GetRight() const { return m_fRight; }
	float GetBottom() const { return m_fBottom; }
	float GetLeft() const { return m_fLeft; }
	float GetTop() const { return m_fTop; }

public:
	// ®G®G Ãæµ¹®G
	bool ColliderRect(const CRect& target)const;
	bool ColliderRect(const CFRect& target)const;
	bool CollidePoint(const int& x, const int& y) const;

private:
	float m_fx, m_fy;
	float m_fw, m_fh;

	float m_fRight, m_fBottom, m_fLeft, m_fTop, m_fCenter_Arr[2];
};

