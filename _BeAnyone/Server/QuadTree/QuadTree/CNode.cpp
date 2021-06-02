#include "stdafx.h"
#include "CNode.h"
#include "CRect.h"
#include "CPlayer.h"

CNode::CNode(const Point& p, const float& half_len)
{
	m_pRect = new CRect(p, half_len);
	m_pRect->update();
	m_pParent = nullptr;
}

CNode::CNode(const float& x, const float& z, const float& half_len)
{
	m_pRect = new CRect(x, z, half_len);
	m_pRect->update();
	m_pParent = nullptr;
}
