#include "stdafx.h"
#include "CRect.h"
#include "CFRect.h"

CRect::CRect()
{
	m_ix = m_iy = m_iw = m_ih = 0;
	m_iLeft = m_iRight = m_iTop = m_iBottom = 0;
	m_iCenter_Arr[0] = 0;
	m_iCenter_Arr[1] = 0;
}

CRect::CRect(const int& x, const int& y, const int& w, const int& h)
{
	initialize(x, y, w, h);
}

void CRect::initialize(const int& x, const int& y, const int& w, const int& h)
{
	m_ix = x;
	m_iy = y;
	m_iw = w;
	m_ih = h;

	m_iLeft = x;
	m_iRight = x + w;
	m_iTop = y;
	m_iBottom = y + h;

	m_iCenter_Arr[0] = x + (w / 2);
	m_iCenter_Arr[1] = y + (h / 2);
}

void CRect::update()
{
	m_iLeft = m_ix;
	m_iTop = m_iy;
	m_iRight = m_ix + m_iw;
	m_iBottom = m_iy + m_ih;

	m_iCenter_Arr[0] = m_iLeft + (m_iw / 2);
	m_iCenter_Arr[1] = m_iTop + (m_ih / 2);
}

void CRect::SetPos(const int& x, const int& y, const int& w, const int& h)
{
	m_ix = x;
	m_iy = y;
	m_iw = w;
	m_ih = h;
}

bool CRect::ColliderRect(const CRect& target) const
{
	return (m_iLeft < target.m_iRight &&
			m_iRight > target.m_iLeft &&
			m_iBottom > target.m_iTop &&
			m_iTop < target.m_iBottom);
}

bool CRect::ColliderRect(const CFRect& target) const
{
	return ((float)m_iLeft < target.GetRight() &&
			(float)m_iRight > target.GetLeft() &&
			(float)m_iBottom > target.GetTop() &&
			(float)m_iTop < target.GetBottom());
}

bool CRect::CollidePoint(const int& x, const int &y) const
{
	return (x <= m_iRight && x >= m_iLeft && y <= m_iBottom && y >= m_iTop);
}
