#include "stdafx.h"
#include "CRect.h"

CRect::CRect()
{
	m_fRight = m_fBottom = m_fLeft = m_fTop = 0.f;
	m_Center.x = 0.f;
	m_Center.z = 0.f;
}

CRect::CRect(const Point& p, const float& half_len)
{
	m_Center = p;

	m_fRight = p.x + half_len;
	m_fLeft = p.x - half_len;
	m_fTop = p.z + half_len;
	m_fBottom = p.z - half_len;
	m_flength = half_len;
}

CRect::CRect(const float& x, const float& z, const float& half_len)
{
	m_Center = Point(x, z);

	m_fRight = m_Center.x + half_len;
	m_fLeft = m_Center.x - half_len;
	m_fTop = m_Center.z + half_len;
	m_fBottom = m_Center.z - half_len;
	m_flength = half_len;
}

void CRect::update()
{

}

bool CRect::ColliderRect(const CRect& target) const
{
	return (m_fLeft < target.GetRight() &&
		m_fRight > target.GetLeft() &&
		m_fBottom > target.GetTop() &&
		m_fTop < target.GetBottom());
}

bool CRect::ColliderRectPoint(const float& x, const float& z) const
{
	return (x <= m_fRight && x >= m_fLeft && z 
		>=m_fBottom && z <= m_fTop);
}
