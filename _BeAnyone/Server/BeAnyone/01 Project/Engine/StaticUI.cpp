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
		if (KEY_HOLD(KEY_TYPE::KEY_LBTN))		//	피킹 후 드래그
		{
			Vector3 vec = CKeyMgr::GetInst()->GetTransformedPoint(m_pCameraProj->GetProjMat());
			m_pMousePoint->Transform()->SetLocalPos(Vector3(vec.x, vec.y, 1.f));
		}
		else if (KEY_TAB(KEY_TYPE::KEY_LBTN))	//	키를 눌렸을 때 충돌검사를 하고 충돌 됐으면 BT_ACTIVE 상태값을 충돌된 버튼만 수정한다/
		{	
			for (int i = 0; i < m_vecButton.size(); ++i)
			{
				m_vecButton[i]->GetActive();
			}

		}
		else if (KEY_AWAY(KEY_TYPE::KEY_LBTN))	//	피킹 후 상태 이상이 없으면 다시 제자리로 돌아간다.
		{
			for (int i = 0; i < m_vecButton.size(); ++i)
			{
				if(m_vecButton[i]->GetState() == BT_STATE::CLICKED)
					m_vecButton[i]->OnCollisionExit(m_pMousePoint->Collider());
			}
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