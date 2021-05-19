#include "stdafx.h"
#include "CPlayer.h"
#include "CNode.h"

void CPlayer::clean()
{
	if (m_pParent != nullptr)
	{
		delete m_pParent;		// 이코드 문제될지도 모름
		m_pParent = nullptr;
	}
}