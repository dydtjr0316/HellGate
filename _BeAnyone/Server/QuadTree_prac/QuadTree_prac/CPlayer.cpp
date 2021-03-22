#include "stdafx.h"
#include "CPlayer.h"
#include "CQuad.h"
#include "CRect.h"
int CPlayer::m_iCounter = 0;

CPlayer::CPlayer()
{
	srand((unsigned int)time(NULL));
	int x = rand() % WINDOW::WIDTH;
	int y = rand() % WINDOW::HEIGHT;
	m_pRect = new CRect(x, y, PLAYER::SIZE, PLAYER::SIZE);
	m_iID = m_iCounter;
}

CPlayer::CPlayer(const int& x, const int& y)
{
	m_pRect = new CRect(x, y, PLAYER::SIZE, PLAYER::SIZE);
	m_iID = m_iCounter;
}

void CPlayer::clean()
{
	if (m_pParent != nullptr)
	{
		delete m_pParent;		// 이코드 문제될지도 모름
		m_pParent = nullptr;
	}
}
