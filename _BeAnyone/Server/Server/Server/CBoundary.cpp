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
	if (player == nullptr)return;
	switch (player->GetType())
	{
	case OBJECT_TYPE::CLIENT:
		this->m_fx = dynamic_cast<CClient*>(player)->GetLocalPosVector().x;
		this->m_fz = dynamic_cast<CClient*>(player)->GetLocalPosVector().z;
		break;
	case OBJECT_TYPE::MONSTER:
		this->m_fx = dynamic_cast<CMonster*>(player)->GetLocalPosVector().x;
		this->m_fz = dynamic_cast<CMonster*>(player)->GetLocalPosVector().z;
		break;
	case OBJECT_TYPE::NPC:
		this->m_fx = dynamic_cast<CNPC*>(player)->GetLocalPosVector().x;
		this->m_fz = dynamic_cast<CNPC*>(player)->GetLocalPosVector().z;
		break;
	default:
		break;
	}

	this->m_fw = searchsize;
	this->m_fh = searchsize;
	m_sID = player->GetID();
}

CBoundary::CBoundary(CClient* player, const float& searchsize)
{
	this->m_fx = player->GetLocalPosVector().x;
	this->m_fz = player->GetLocalPosVector().z;
	this->m_fw = searchsize;
	this->m_fh = searchsize;
	m_sID = player->GetID();
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
	if (this == nullptr)return false;
	return !(range.GetX() - range.GetW() > m_fx + m_fw ||
		range.GetX() + range.GetW() < m_fx - m_fw ||
		range.GetZ() - range.GetH() > m_fz + m_fh ||
		range.GetZ() + range.GetH() < m_fz - m_fh);
}
