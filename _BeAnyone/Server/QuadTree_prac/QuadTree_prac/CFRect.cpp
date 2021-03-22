#include "stdafx.h"
#include "CFRect.h"
#include "CRect.h"

CFRect::CFRect()
{
	m_fx = m_fy = m_fw = m_fh = 0.f;
	m_fRight = m_fBottom = m_fLeft = m_fTop = 0.f;
	m_fCenter_Arr[0] = 0.f;
	m_fCenter_Arr[1] = 0.f;
}

void CFRect::initialize(const float& x, const float& y, const float& w, const float& h)
{
	// 아래 4줄 문제 될지도? 확인해보기 패킷넘어가며 이상해질수도
	m_fx = (int)x;
	m_fy = (int)y;
	m_fw = (int)w; 
	m_fh = (int)h;

	m_fLeft = x;
	m_fRight = x + w;
	m_fTop = y;
	m_fBottom = y + h;

	m_fCenter_Arr[0] = x + (w / 2.0f);	// 다시 보기 
	m_fCenter_Arr[1] = y + (y / 2.0f);

}

void CFRect::update()
{
	m_fLeft = m_fx;
	m_fTop = m_fy;
	m_fRight = m_fx + m_fw;
	m_fBottom = m_fy + m_fh;

	m_fCenter_Arr[0] = m_fLeft + (m_fw / 2);
	m_fCenter_Arr[1] = m_fTop + (m_fh / 2);
}

bool CFRect::ColliderRect(const CRect& target) const
{
	return (m_fLeft < target.GetRight() &&
			m_fRight > target.GetLeft() &&
			m_fBottom > target.GetTop() &&
			m_fTop < target.GetBottom());
}

bool CFRect::ColliderRect(const CFRect& target) const
{
	return ((float)m_fLeft < target.GetRight() &&
			(float)m_fRight > target.GetLeft() &&
			(float)m_fBottom > target.GetTop() &&
			(float)m_fTop < target.GetBottom());
}

bool CFRect::CollidePoint(const int& x, const int& y) const
{
	return (x <= m_fRight && x >= m_fLeft && y <= m_fBottom && y >= m_fTop);
}
