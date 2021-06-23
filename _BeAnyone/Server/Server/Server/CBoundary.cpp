#include "stdafx.h"
#include "CBoundary.h"
#include "CGameObject.h"

CBoundary::CBoundary()
{
}

CBoundary::CBoundary(const float& x, const float& z, const float& w, const float& h)
{
	this->m_fx = x;
	this->m_fz = z;
	this->m_fw = w;
	this->m_fh = h;
}

CBoundary::CBoundary(CGameObject* player, const float& searchsize)
{
	this->m_fx = dynamic_cast<CClient*>(player)->GetLocalPosVector().x;
	this->m_fz = dynamic_cast<CClient*>(player)->GetLocalPosVector().z;
	this->m_fw = searchsize;
	this->m_fh = searchsize;
}

CBoundary::CBoundary(CClient* player, const float& searchsize)
{
	this->m_fx = player->GetLocalPosVector().x;
	this->m_fz = player->GetLocalPosVector().z;
	this->m_fw = searchsize;
	this->m_fh = searchsize;
}

bool CBoundary::contains(CGameObject* p)
{
	return (p->GetLocalPosVector().x >= m_fx - m_fw &&
		p->GetLocalPosVector().x <= m_fx + m_fw &&
		p->GetLocalPosVector().z >= m_fz - m_fh &&
		p->GetLocalPosVector().z <= m_fz + m_fh);
}

bool CBoundary::intersects(CBoundary& range)
{
	return !(range.GetX() - range.GetW() > m_fx + m_fw ||
		range.GetX() + range.GetW() < m_fx - m_fw ||
		range.GetZ() - range.GetH() > m_fz + m_fh ||
		range.GetZ() + range.GetH() < m_fz - m_fh);
}
