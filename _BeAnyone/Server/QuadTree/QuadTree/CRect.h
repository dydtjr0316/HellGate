#pragma once
class CRect
{
public:
	CRect();
	CRect(const Point& p, const float& len);
	CRect(const float& x, const float& z, const float& half_len);
	~CRect() {};
public:
	void update();
public:
	float GetRight() const { return m_fRight; }
	float GetLeft() const { return m_fLeft; }
	float GetTop() const { return m_fTop; }
	float GetBottom() const { return m_fBottom; }
	float GetLength() const { return m_flength; }
	float GetHalfLength() const { return (int)m_flength >> 1; }

	Point GetCenter() const { return m_Center; }
	float GetCenterX() const { return m_Center.x; }
	float GetCenterZ() const { return m_Center.z; }
public:
	bool ColliderRect(const CRect& target)const;
	bool ColliderRectPoint(const float& x, const float& z)const;


private:
	float m_flength;
	Point m_Center;

	float m_fRight, m_fBottom, m_fLeft, m_fTop;
};

