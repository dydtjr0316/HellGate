#include "stdafx.h"
#include "CRectangle.h"
#include "CPlayer.h"

CRectangle::CRectangle()
{
}

CRectangle::CRectangle(const float& x, const float& z, const float& w, const float& h)
{
	this->m_fx = x;
	this->m_fz = z;
	this->m_fw = w;
	this->m_fh = h;
}

CRectangle::CRectangle(CPlayer* player, const float& w, const float& h)
{
	this->m_fx = player->GetX();
	this->m_fz = player->GetZ();
	this->m_fw = w;
	this->m_fh = h;
}

bool CRectangle::contains(CPlayer* p)
{
	return (p->GetX() >= m_fx - m_fw &&
		p->GetX() <= m_fx + m_fw &&
		p->GetZ() >= m_fz - m_fh &&
		p->GetZ() <= m_fz + m_fh);
}

bool CRectangle::intersects(CRectangle& range)
{
	return !(range.GetX() - range.GetW() > m_fx + m_fw ||
		range.GetX() + range.GetW() < m_fx - m_fw ||
		range.GetZ() - range.GetH() > m_fz + m_fh ||
		range.GetZ() + range.GetH() < m_fz - m_fh);
}
