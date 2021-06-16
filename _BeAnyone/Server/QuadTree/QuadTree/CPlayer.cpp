#include "stdafx.h"
#include "CPlayer.h"

CPlayer::CPlayer()
{
	m_fx = 0.f;
	m_fz = 0.f;
	m_id = 0;
}

CPlayer::CPlayer(const uShort& id, const float& x, const float& z)
{
	this->m_id = id;
	this->m_fx = x;
	this->m_fz = z;
}

