#include "stdafx.h"
#include "CPlayer.h"
#include "CNode.h"

void CPlayer::clean()
{
	if (m_pParent != nullptr)
	{
		delete m_pParent;		// ���ڵ� ���������� ��
		m_pParent = nullptr;
	}
}