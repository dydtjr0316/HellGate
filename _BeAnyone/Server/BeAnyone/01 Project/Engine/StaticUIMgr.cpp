#include "pch.h"
#include "StaticUIMgr.h"
#include "ResMgr.h"
#include "KeyMgr.h"
#include "Button.h"

void CStaticUIMgr::init()
{
	m_bActive = false;
	m_eType = UI_TYPE::PRIVATE_ITEM_UI;
	m_pFrame = CResMgr::GetInst()->Load<CTexture>(L"ButtonFrame", L"Texture\\Terrain\\Desert_Base.png");

	/*for (int i = 0; i < 16; ++i)
		m_vecButton.push_back(new CButton);*/
}

void CStaticUIMgr::update()
{
	if (KEY_TAB(KEY_TYPE::KEY_Q))
		m_bActive != m_bActive;

	if (!m_bActive)
		return;

	
}

void CStaticUIMgr::SaveToScene(FILE* _pFile)
{
}

void CStaticUIMgr::LoadFromScene(FILE* _pFile)
{
}

CStaticUIMgr::CStaticUIMgr()
	: CComponent(COMPONENT_TYPE::UI)
{
	init();
}

CStaticUIMgr::~CStaticUIMgr()
{
}