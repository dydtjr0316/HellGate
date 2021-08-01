#include "pch.h"
#include "StaticUI.h"
#include "ResMgr.h"
#include "KeyMgr.h"
#include "CollisionMgr.h"
#include "Button.h"

void CStaticUI::init()
{
	m_bActive = false;
	m_eType = UI_TYPE::PRIVATE_ITEM_UI;
	m_pFrame = CResMgr::GetInst()->Load<CTexture>(L"ButtonFrame", L"Texture\\Terrain\\Lava1.png");

	for (int i = 0; i < 16; ++i)
		m_vecButton.push_back(new CButton);
}

void CStaticUI::update()
{
	GetObj()->SetUiRenderCheck(m_bActive);
}

void CStaticUI::finalupdate()
{

	if (KEY_TAB(KEY_TYPE::KEY_Q))
	{
		m_bActive = !m_bActive;
		for (int i = 0; i < m_vecButton.size(); ++i)
			m_vecButton[i]->SetActive(m_bActive);
	}

	if (!m_bActive)
	{
		return;
	}
	else
		CCollisionMgr::GetInst()->CheckCollisionLayer(L"UI", L"UI");


	GetObj()->SetUiRenderCheck(m_bActive);
}

void CStaticUI::SaveToScene(FILE* _pFile)
{
}

void CStaticUI::LoadFromScene(FILE* _pFile)
{
}

CStaticUI::CStaticUI()
	: CComponent(COMPONENT_TYPE::UI)
{
	init();
}

CStaticUI::~CStaticUI()
{
}