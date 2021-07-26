#include "pch.h"
#include "Button.h"

CButton::CButton()
	: CGameObject()
{
	m_pItemImage = CResMgr::GetInst()->Load<CTexture>(L"ButtonFrame", L"Texture\\Terrain\\Desert_Base.png");
}
