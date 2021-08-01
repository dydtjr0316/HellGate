#include "pch.h"
#include "StaticUI.h"
#include "ResMgr.h"
#include "KeyMgr.h"
#include "CollisionMgr.h"
#include "RenderMgr.h"
#include "Button.h"

void CStaticUI::init()
{
	m_bActive = false;
	m_eType = UI_TYPE::PRIVATE_ITEM_UI;
	m_pFrame = CResMgr::GetInst()->Load<CTexture>(L"ButtonFrame", L"Texture\\Terrain\\Lava1.png");

	for (int i = 0; i < 16; ++i)
		m_vecButton.push_back(new CButton);

	CGameObject* pObject = new CGameObject;
	pObject->SetName(L"MousePoint");
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CCollider);
	pObject->AddComponent(new CMeshRender);
	pObject->Collider()->SetColliderType(COLLIDER_TYPE::RECT);
	pObject->FrustumCheck(false);
	pObject->Transform()->SetLocalPos(Vector3(CKeyMgr::GetInst()->GetMousePos().x, CKeyMgr::GetInst()->GetMousePos().y, 1.f));
	pObject->Transform()->SetLocalScale(Vector3(1.f, 1.f, 1.f));
	pObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"RectMtrl"));
	CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"PUI")->AddGameObject(pObject);
	m_pMousePoint = pObject;

	//g_netMgr.SetObj(pObject);
}

void CStaticUI::update()
{
	GetObj()->SetUiRenderCheck(m_bActive);

	if (KEY_TAB(KEY_TYPE::KEY_I))
	{
		m_bActive = !m_bActive;
		for (int i = 0; i < m_vecButton.size(); ++i)
			m_vecButton[i]->SetActive(m_bActive);
	}

	if (!m_bActive)
		return;
	else
	{
		if (KEY_HOLD(KEY_TYPE::KEY_LBTN))
		{
			auto mPos = CKeyMgr::GetInst()->GetMousePos();
		
			m_pMousePoint->Transform()->SetLocalPos(Vector3(mPos.x, mPos.y, 1.f));
			cout << mPos.x << "\t" << mPos.y << endl;
		}
	}
}

void CStaticUI::finalupdate()
{
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