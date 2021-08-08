#include "pch.h"
#include "StaticUI.h"
#include "ResMgr.h"
#include "KeyMgr.h"
#include "CollisionMgr.h"
#include "RenderMgr.h"
#include "Button.h"

void CStaticUI::CreatePickingObj()
{
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

void CStaticUI::CreateButton(CCamera* _camObj)
{
	//	item UI, 상속되는 버튼들
	CGameObject* pObject = new CGameObject;
	Vector3 vScale = Vector3(400.f, 600.f, 1.f);
	pObject->SetName(L"Item UI Object");
	pObject->FrustumCheck(false);
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CMeshRender);
	pObject->AddComponent(new CStaticUI);
	pObject->StaticUI()->init(UI_TYPE::PRIVATE_ITEM_UI);
	pObject->StaticUI()->CreatePickingObj();
	// 투영행렬 statiUI 컴포넌트에 등록 (ORTHOGRAPHIC 카메라 정보 필요)
	pObject->StaticUI()->SetCameraProj(_camObj->Camera());
	// Transform 설정
	tResolution res = CRenderMgr::GetInst()->GetResolution();
	pObject->Transform()->SetLocalPos(Vector3(-(res.fWidth / 2.f) + (vScale.x / 2.f) + (3 * vScale.x)
		, (res.fHeight / 2.f) - (vScale.y / 2.f)
		, 1.f));
	pObject->Transform()->SetLocalScale(vScale);
	// MeshRender 설정
	pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	Ptr<CMaterial> pMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"TestMtrl");
	pObject->MeshRender()->SetMaterial(pMtrl->Clone());
	Ptr<CTexture> itemUI = pObject->StaticUI()->m_pFrame;
	pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, itemUI.GetPointer());
	// AddGameObject
	CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"UI")->AddGameObject(pObject);

	//	Static Ui에 상속된 버튼들 Scene에 Obj로 추가
	for (int i = 0; i < pObject->StaticUI()->m_vecButton.size(); ++i)
	{
		vScale = Vector3(80.f, 120.f, 1.f);
		Ptr<CTexture> itemUI = CResMgr::GetInst()->FindRes<CTexture>(L"ItemUiTex");
		CGameObject* pButtonObj = new CGameObject;
		pButtonObj->SetName(L"Button Object");
		pButtonObj->FrustumCheck(false);	// 절두체 컬링 사용하지 않음
		pButtonObj->AddComponent(new CTransform);
		pButtonObj->AddComponent(new CMeshRender);
		pButtonObj->AddComponent(new CCollider);
		pButtonObj->Collider()->SetColliderType(COLLIDER_TYPE::RECT);
		//	버튼 Script 설정
		pButtonObj->AddComponent(pObject->StaticUI()->m_vecButton[i]);
		pObject->StaticUI()->m_vecButton[i]->SetParent(pObject->StaticUI());
		// Transform 설정
		tResolution res = CRenderMgr::GetInst()->GetResolution();
		pButtonObj->Transform()->SetLocalPos(Vector3(-(res.fWidth / 2.f) + (vScale.x / 2.f) + (i * vScale.x) + 200.f
			, (res.fHeight / 2.f) - (vScale.y / 2.f)
			, 1.f));
		pButtonObj->Transform()->SetLocalScale(vScale);
		// MeshRender 설정
		pButtonObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
		Ptr<CMaterial> pMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"TestMtrl");
		pButtonObj->MeshRender()->SetMaterial(pMtrl->Clone());
		pButtonObj->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pObject->StaticUI()->m_vecButton[i]->GetImage().GetPointer());
		// AddGameObject
		CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"UI")->AddGameObject(pButtonObj);
	}
}

void CStaticUI::SetButton(ITEM_ID _id)
{
	for (int i = 0; i < m_vecButton.size(); ++i)
	{
		if (m_vecButton[i]->GetItemID() == ITEM_ID::EMPTY)
		{
			//*(m_vecButton[i]->GetObj()) = *_obj;
			m_vecButton[i]->SetItemID(_id);
			return;
		}
		else
			cout << "아이템 창 꽉 참" << endl;
	}
}

void CStaticUI::init(UI_TYPE _eType)
{
	m_bActive = false;
	m_eType = _eType;
	m_pFrame = CResMgr::GetInst()->Load<CTexture>(L"ButtonFrame", L"Texture\\Terrain\\Lava1.png");

	for (int i = 0; i < 16; ++i)
	{
		m_vecButton.push_back(new CButton);
	}
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
			/*for (int i = 0; i < m_vecButton.size(); ++i)
			{
				
				m_vecButton[i]->GetActive();
			}*/

		}
		else if (KEY_AWAY(KEY_TYPE::KEY_LBTN))	//	피킹 후 상태 이상이 없으면 다시 제자리로 돌아간다.
		{
			for (int i = 0; i < m_vecButton.size(); ++i)
			{
				if (m_vecButton[i]->GetState() == BT_STATE::CLICKED)
				{
					m_vecButton[i]->OnCollisionExit(m_pMousePoint->Collider());


				}


			}
		}
	}
}

void CStaticUI::finalupdate()
{
	GetObj()->SetUiRenderCheck(m_bActive);
}

CStaticUI::CStaticUI()
	: CComponent(COMPONENT_TYPE::UI)
{
}

CStaticUI::~CStaticUI()
{
}

void CStaticUI::SaveToScene(FILE* _pFile)
{
}

void CStaticUI::LoadFromScene(FILE* _pFile)
{
}