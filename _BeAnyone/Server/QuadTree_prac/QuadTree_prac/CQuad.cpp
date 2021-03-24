#include "stdafx.h"
#include "CQuad.h"
#include "CRect.h"
#include "CPlayer.h"

CQuad::CQuad(const int& x, const int& y, const int& w, const int& h)
{
	m_pRect = new CRect;
	m_pRect->SetPos(x, y, w, h);
	m_pRect->update();
	m_pParent = nullptr;
}
