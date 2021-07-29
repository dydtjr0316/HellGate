#include "pch.h"

#include <iostream>
#include <random>

#include "SceneMgr.h"

#include "Scene.h"
#include "Layer.h"
#include "GameObject.h"

#include "ResMgr.h"
#include "Shader.h"
#include "Mesh.h"
#include "Texture.h"
#include "Transform.h"
#include "MeshRender.h"
#include "Light.h"

#include "TimeMgr.h"
#include "KeyMgr.h"
#include "Camera.h"
#include "Terrain.h"

#include "CollisionMgr.h"
#include "EventMgr.h"
#include "RenderMgr.h"
#include "Device.h"
#include "Core.h"

#include "PlayerScript.h"
#include "ToolCamScript.h"
#include "GridScript.h"
#include "SwordScript.h"
#include "MonsterScript.h"
#include "NpcScript.h"
// UI
#include "StaticUIMgr.h"
#include "TemperUiScript.h"
#include "Button.h"

using namespace std;

default_random_engine dre;
uniform_real_distribution<float> uid(500, 11000);

CScene* CSceneMgr::GetCurScene()
{
	return m_pCurScene;
}

void CSceneMgr::ChangeScene(CScene* _pNextScene)
{
	SAFE_DELETE(m_pCurScene);
	m_pCurScene = _pNextScene;
}

CSceneMgr::CSceneMgr()
	: m_pCurScene(nullptr)
{
}

CSceneMgr::~CSceneMgr()
{
	SAFE_DELETE(m_pCurScene);
}

void CSceneMgr::CreateTargetUI()
{
#ifdef _DEBUG
	Vector3 vScale(150.f, 150.f, 1.f);
	
	Ptr<CTexture> arrTex[5] = { CResMgr::GetInst()->FindRes<CTexture>(L"DiffuseTargetTex")
		, CResMgr::GetInst()->FindRes<CTexture>(L"NormalTargetTex")
		, CResMgr::GetInst()->FindRes<CTexture>(L"PositionTargetTex")
		, CResMgr::GetInst()->FindRes<CTexture>(L"DiffuseLightTargetTex")
		, CResMgr::GetInst()->FindRes<CTexture>(L"SpecularLightTargetTex") };
	
	for (UINT i = 0; i < 5; ++i)
	{
		CGameObject* pObject = new CGameObject;
		pObject->SetName(L"UI Object");
		pObject->FrustumCheck(false);	// 절두체 컬링 사용하지 않음
		pObject->AddComponent(new CTransform);
		pObject->AddComponent(new CMeshRender);
	
		// Transform 설정
		tResolution res = CRenderMgr::GetInst()->GetResolution();
	
		pObject->Transform()->SetLocalPos(Vector3(-(res.fWidth / 2.f) + (vScale.x / 2.f) + (i * vScale.x)
			, (res.fHeight / 2.f) - (vScale.y / 2.f)
			, 1.f));
	
		pObject->Transform()->SetLocalScale(vScale);
	
		// MeshRender 설정
		pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
		Ptr<CMaterial> pMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"TestMtrl");
		pObject->MeshRender()->SetMaterial(pMtrl->Clone());
		pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, arrTex[i].GetPointer());
	
		// AddGameObject
		m_pCurScene->FindLayer(L"UI")->AddGameObject(pObject);
	}


#else

	Ptr<CTexture> UiTexture[2] = {
		CResMgr::GetInst()->FindRes<CTexture>(L"UiHug"),
		CResMgr::GetInst()->FindRes<CTexture>(L"UiTemper")
	};

	Vector3 vScale(350.f, 10.f, 1.f);

	for (int i = 0; i < 4; ++i) {

		CGameObject* pObject = new CGameObject;
		pObject->SetName(L"UI Object");
		pObject->FrustumCheck(false);	// 절두체 컬링 사용하지 않음
		pObject->AddComponent(new CTransform);
		pObject->AddComponent(new CMeshRender);

		tResolution res = CRenderMgr::GetInst()->GetResolution();

		if (i == 0 || i == 1) {
			if (i == 1) {
				vScale = Vector3(350.f, 20.f, 1.f);
			}
			pObject->Transform()->SetLocalPos(Vector3(-(res.fWidth / 2.f) + (vScale.x / 2.f) + 60.f
				, (res.fHeight / 2.f) - (vScale.y / 2.f) - (10.f * (i + 1) + (10.f * i))
				, 1.f));
		}
		else if (i == 2 || i == 3) {
			vScale = Vector3(360.f, 2.f, 1.f);
			pObject->Transform()->SetLocalPos(Vector3(-(res.fWidth / 2.f) + (vScale.x / 2.f) + 60.f
				, (res.fHeight / 2.f) - (vScale.y / 2.f) - (15.f * (i - 1) + (5.f * (i - 2)))
				, 1.f));
		}

		pObject->Transform()->SetLocalScale(vScale);

		// MeshRender 설정
		Ptr<CMesh> hp = new CMesh;
		hp = CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh");


		pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
		Ptr<CMaterial> pMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"TestMtrl");
		pObject->MeshRender()->SetMaterial(pMtrl->Clone());

		// AddGameObject
		m_pCurScene->FindLayer(L"UI")->AddGameObject(pObject);
	}

	vector<CGameObject*> vUiObject;

	vScale = Vector3(350.f, 10.f, 1.f);

	for (int i = 0; i < 4; ++i) {

		CGameObject* pUiObject = new CGameObject;
		pUiObject->SetName(L"UI Object");
		pUiObject->FrustumCheck(false);	// 절두체 컬링 사용하지 않음
		pUiObject->AddComponent(new CTransform);
		pUiObject->AddComponent(new CMeshRender);

		tResolution res = CRenderMgr::GetInst()->GetResolution();

		if (i == 0 || i == 1) {
			//if (i == 1) {
			//	vScale = Vector3(350.f, 20.f, 1.f);
			//}
			pUiObject->Transform()->SetLocalPos(Vector3(-(res.fWidth / 2.f) + (vScale.x / 2.f) + 600.f
				, (res.fHeight / 2.f) - (vScale.y / 2.f) - (10.f * (i + 1) + (10.f * i))
				, 1.f));
		}
		else if (i == 2 || i == 3) {
			vScale = Vector3(360.f, 2.f, 1.f);
			pUiObject->Transform()->SetLocalPos(Vector3(-(res.fWidth / 2.f) + (vScale.x / 2.f) + 600.f
				, (res.fHeight / 2.f) - (vScale.y / 2.f) - (15.f * (i - 1) + (5.f * (i - 2)))
				, 1.f));
		}

		pUiObject->Transform()->SetLocalScale(vScale);

		// MeshRender 설정
		Ptr<CMesh> hp = new CMesh;
		hp = CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh");


		pUiObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
		Ptr<CMaterial> pMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"TestMtrl");
		pUiObject->MeshRender()->SetMaterial(pMtrl->Clone());
		pUiObject->SetUiRenderCheck(false);

		vUiObject.push_back(pUiObject);

		// AddGameObject
		m_pCurScene->FindLayer(L"UI")->AddGameObject(pUiObject);
	}

	vScale = Vector3(40.f, 40.f, 1.f);

	for (int i = 0; i < 2; ++i) {

		CGameObject* pObject = new CGameObject;
		pObject->SetName(L"UI Object");
		pObject->FrustumCheck(false);	// 절두체 컬링 사용하지 않음
		pObject->AddComponent(new CTransform);
		pObject->AddComponent(new CMeshRender);
		pObject->AddComponent(new CTemperUiScript);

		tResolution res = CRenderMgr::GetInst()->GetResolution();


		pObject->Transform()->SetLocalPos(Vector3(-(res.fWidth / 2.f) + (vScale.x / 2.f) + 10.f + (410 * i)
			, (res.fHeight / 2.f) - (vScale.y / 2.f) - 10.f
			, 1.f));


		pObject->Transform()->SetLocalScale(vScale);

		// MeshRender 설정
		Ptr<CMesh> hp = new CMesh;
		hp = CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh");


		pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CircleMesh"));
		Ptr<CMaterial> pMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"TestMtrl");
		pObject->MeshRender()->SetMaterial(pMtrl->Clone());
		pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, UiTexture[i].GetPointer());


		CTemperUiScript* uiScript = pObject->GetScript<CTemperUiScript>();
		uiScript->SetObject(pObject);

		if (i == 0) {
			uiScript->SetTempBar(vUiObject[0]);
			uiScript->SetUnderTempBar(vUiObject[2]);
		}
		else if (i == 1) {
			uiScript->SetTempBar(vUiObject[1]);
			uiScript->SetUnderTempBar(vUiObject[3]);
		}
		// AddGameObject
		m_pCurScene->FindLayer(L"UI")->AddGameObject(pObject);
	}

#endif

	CGameObject* pObject = new CGameObject;
	vScale = Vector3(400.f, 600.f, 1.f);
	pObject->SetName(L"UI Object");
	pObject->FrustumCheck(false);
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CMeshRender);
	pObject->AddComponent(new CStaticUIMgr);
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
	Ptr<CTexture> itemUI = pObject->StaticUIMgr()->m_pFrame;
	pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, itemUI.GetPointer());
	// AddGameObject
	m_pCurScene->FindLayer(L"UI")->AddGameObject(pObject);

	//	Static Ui mgr에 상속된 버튼들 Scene에 Obj로 추가
	for (int i = 0; i < pObject->StaticUIMgr()->m_vecButton.size(); ++i)
	{
		vScale = Vector3(80.f, 120.f, 1.f);
		Ptr<CTexture> itemUI = CResMgr::GetInst()->FindRes<CTexture>(L"ItemUiTex");
		CGameObject* pButtonObj = new CGameObject;
		pButtonObj->SetName(L"zButton Object");
		pButtonObj->FrustumCheck(false);	// 절두체 컬링 사용하지 않음
		pButtonObj->AddComponent(new CTransform);
		pButtonObj->AddComponent(new CMeshRender);
		//	버튼 Script 설정
		pButtonObj->AddComponent(pObject->StaticUIMgr()->m_vecButton[i]);
		pObject->StaticUIMgr()->m_vecButton[i]->SetParent(pObject->StaticUIMgr());

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
		pButtonObj->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pObject->StaticUIMgr()->m_vecButton[i]->GetImage().GetPointer());
		// AddGameObject
		m_pCurScene->FindLayer(L"UI")->AddGameObject(pButtonObj);
	}
}

void CSceneMgr::CreateMap(CTerrain* _terrain)
{
	// 집1 left
	Ptr<CMeshData> pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\Desert\\House1.fbx", FBX_TYPE::DESERT_MAP);
	//pMeshData->Save(pMeshData->GetPath());

	CGameObject* pMapObject = nullptr;

	pMapObject = pMeshData->Instantiate();
	pMapObject->SetName(L"House1");
	pMapObject->FrustumCheck(false);

	int z = (int)(3000.f / 60.f);
	bool bReverseQuad = ((z % 2) != 0);
	float mapY = _terrain->GetHeight(1000.f, 3000, bReverseQuad);

	pMapObject->Transform()->SetLocalPos(Vector3(1000.f, mapY * 2 + 60.f, 3000.f));
	pMapObject->Transform()->SetLocalScale(Vector3(60.f,60.f, 60.f));//(1.0f, 1.0f, 1.0f));
	pMapObject->Transform()->SetLocalRot(Vector3(-XM_PI / 2, 0.f, 0.f));
	pMapObject->AddComponent(new CCollider);
	pMapObject->Collider()->SetColliderType(COLLIDER_TYPE::MESH, L"House1");
	pMapObject->Collider()->SetBoundingBox(BoundingBox(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingBoxExtents( pMapObject->Transform()->GetLocalScale() )));
	pMapObject->Collider()->SetBoundingSphere(BoundingSphere(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingSphereRadius() * 60.f ));
	m_pCurScene->AddGameObject(L"Map", pMapObject, false);

	// 집1 통나무

	pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\Desert\\Barrel.fbx", FBX_TYPE::DESERT_MAP);
	//pMeshData->Save(pMeshData->GetPath());

	pMapObject = new CGameObject;

	pMapObject = pMeshData->Instantiate();
	pMapObject->SetName(L"House1");
	pMapObject->FrustumCheck(false);

	z = (int)(2750.f / 60.f);
	bReverseQuad = ((z % 2) != 0);
	mapY = _terrain->GetHeight(1100.f, 2750.f, bReverseQuad);

	pMapObject->Transform()->SetLocalPos(Vector3(1100.f, mapY * 2 + 50.f, 2750.f));
	pMapObject->Transform()->SetLocalScale(Vector3(60.f, 60.f, 60.f));//(1.0f, 1.0f, 1.0f));
	pMapObject->Transform()->SetLocalRot(Vector3(-XM_PI / 2, 0.f, 0.f));
	pMapObject->AddComponent(new CCollider);
	pMapObject->Collider()->SetColliderType(COLLIDER_TYPE::MESH, L"Barrel");

	m_pCurScene->AddGameObject(L"Map", pMapObject, false);

	// 집2 left

	// 집 2
	pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\Desert\\WoodenHouse3.fbx", FBX_TYPE::DESERT_MAP);
	//pMeshData->Save(pMeshData->GetPath());

	pMapObject = new CGameObject;

	pMapObject = pMeshData->Instantiate();
	pMapObject->SetName(L"House1");
	pMapObject->FrustumCheck(false);

	z = (int)(3700.f / 60.f);
	bReverseQuad = ((z % 2) != 0);
	mapY = _terrain->GetHeight(1000.f, 3700.f, bReverseQuad);

	pMapObject->Transform()->SetLocalPos(Vector3(1000.f, mapY * 2 + 0.f, 3700.f));
	pMapObject->Transform()->SetLocalScale(Vector3(60.f, 60.f, 60.f));//(1.0f, 1.0f, 1.0f));
	pMapObject->Transform()->SetLocalRot(Vector3(-XM_PI / 2, 0.f, 0.f));
	pMapObject->AddComponent(new CCollider);
	pMapObject->Collider()->SetColliderType(COLLIDER_TYPE::MESH, L"WoodenHouse3");

	m_pCurScene->AddGameObject(L"Map", pMapObject, false);

	// 집3 left

	pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\Desert\\House2.fbx", FBX_TYPE::DESERT_MAP);
	//pMeshData->Save(pMeshData->GetPath());

	pMapObject = new CGameObject;

	pMapObject = pMeshData->Instantiate();
	pMapObject->SetName(L"House2");
	pMapObject->FrustumCheck(false);

	z = (int)(5500.f / 60.f);
	bReverseQuad = ((z % 2) != 0);
	mapY = _terrain->GetHeight(1000.f, 5500.f, bReverseQuad);

	pMapObject->Transform()->SetLocalPos(Vector3(1000.f, mapY * 2 + 60.f, 5500.f));
	pMapObject->Transform()->SetLocalScale(Vector3(60.f, 60.f, 50.f));//(1.0f, 1.0f, 1.0f));
	pMapObject->Transform()->SetLocalRot(Vector3(-XM_PI / 2, 0.f, 0.f));
	pMapObject->AddComponent(new CCollider);
	pMapObject->Collider()->SetColliderType(COLLIDER_TYPE::MESH, L"House2");

	m_pCurScene->AddGameObject(L"Map", pMapObject, false);

	// 집4 right

	pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\Desert\\WoodenHouse3.fbx", FBX_TYPE::DESERT_MAP);
	//pMeshData->Save(pMeshData->GetPath());

	pMapObject = new CGameObject;

	pMapObject = pMeshData->Instantiate();
	pMapObject->SetName(L"House2");
	pMapObject->FrustumCheck(false);

	z = (int)(5300.f / 60.f);
	bReverseQuad = ((z % 2) != 0);
	mapY = _terrain->GetHeight(2500.f, 5300.f, bReverseQuad);

	pMapObject->Transform()->SetLocalPos(Vector3(2500.f, mapY * 2 + 0.f, 5300.f));
	pMapObject->Transform()->SetLocalScale(Vector3(60.f, 60.f, 60.f));//(1.0f, 1.0f, 1.0f));
	pMapObject->Transform()->SetLocalRot(Vector3(-XM_PI / 2, 0.f, 0.f));
	pMapObject->AddComponent(new CCollider);
	pMapObject->Collider()->SetColliderType(COLLIDER_TYPE::MESH, L"WoodenHouse3");
	pMapObject->Collider()->SetBoundingBox(BoundingBox(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingBoxExtents() * pMapObject->Transform()->GetLocalScale()));
	pMapObject->Collider()->SetBoundingSphere(BoundingSphere(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingSphereRadius() * 60.f));
	m_pCurScene->AddGameObject(L"Map", pMapObject, false);

	// 집5 right

	pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\Desert\\House2.fbx", FBX_TYPE::DESERT_MAP);
	//pMeshData->Save(pMeshData->GetPath());

	pMapObject = new CGameObject;

	pMapObject = pMeshData->Instantiate();
	pMapObject->SetName(L"House2");
	pMapObject->FrustumCheck(false);

	z = (int)(4500.f / 60.f);
	bReverseQuad = ((z % 2) != 0);
	mapY = _terrain->GetHeight(2500.f, 4500.f, bReverseQuad);

	pMapObject->Transform()->SetLocalPos(Vector3(2500.f, mapY * 2 + 50.f, 4500.f));
	pMapObject->Transform()->SetLocalScale(Vector3(60.f, 60.f, 60.f));//(1.0f, 1.0f, 1.0f));
	pMapObject->Transform()->SetLocalRot(Vector3(-XM_PI / 2, XM_PI, 0.f));
	pMapObject->AddComponent(new CCollider);
	pMapObject->Collider()->SetColliderType(COLLIDER_TYPE::MESH, L"House2");

	m_pCurScene->AddGameObject(L"Map", pMapObject, false);

	// 화덕

	pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\Desert\\CookingFireplace.fbx", FBX_TYPE::DESERT_MAP);
	//pMeshData->Save(pMeshData->GetPath());

	pMapObject = new CGameObject;

	pMapObject = pMeshData->Instantiate();
	pMapObject->SetName(L"House1");
	pMapObject->FrustumCheck(false);

	z = (int)(3500.f / 60.f);
	bReverseQuad = ((z % 2) != 0);
	mapY = _terrain->GetHeight(2400.f, 3500.f, bReverseQuad);

	pMapObject->Transform()->SetLocalPos(Vector3(2400.f, mapY * 2 + 50.f, 3500.f));
	pMapObject->Transform()->SetLocalScale(Vector3(80.f, 100.f, 80.f));//(1.0f, 1.0f, 1.0f));
	pMapObject->Transform()->SetLocalRot(Vector3(-XM_PI / 2, 0.f, 0.f));
	pMapObject->AddComponent(new CCollider);
	pMapObject->Collider()->SetColliderType(COLLIDER_TYPE::MESH, L"CookingFireplace");
	pMapObject->Collider()->SetBoundingBox(BoundingBox(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingBoxExtents() * pMapObject->Transform()->GetLocalScale()));
	pMapObject->Collider()->SetBoundingSphere(BoundingSphere(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingSphereRadius() * 60.f));

	m_pCurScene->AddGameObject(L"Map", pMapObject, false);

	// 텐트

	pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\Desert\\Tent1.fbx", FBX_TYPE::DESERT_MAP);
	//pMeshData->Save(pMeshData->GetPath());

	pMapObject = new CGameObject;

	pMapObject = pMeshData->Instantiate();
	pMapObject->SetName(L"House1");
	pMapObject->FrustumCheck(false);

	z = (int)(3000.f / 60.f);
	bReverseQuad = ((z % 2) != 0);
	mapY = _terrain->GetHeight(2700.f, 3000.f, bReverseQuad);

	pMapObject->Transform()->SetLocalPos(Vector3(2700.f, mapY * 2 + 70.f, 3000.f));
	pMapObject->Transform()->SetLocalScale(Vector3(100.f, 100.f, 100.f));//(1.0f, 1.0f, 1.0f));
	pMapObject->Transform()->SetLocalRot(Vector3(-XM_PI / 2, 0.f, -XM_PI/25));
	pMapObject->AddComponent(new CCollider);
	pMapObject->Collider()->SetColliderType(COLLIDER_TYPE::MESH, L"Tent1");
	pMapObject->Collider()->SetBoundingBox(BoundingBox(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingBoxExtents() * pMapObject->Transform()->GetLocalScale()));
	pMapObject->Collider()->SetBoundingSphere(BoundingSphere(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingSphereRadius() * 60.f));

	m_pCurScene->AddGameObject(L"Map", pMapObject, false);


	// 돌 

	pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\Desert\\RockGrey2_high.fbx", FBX_TYPE::DESERT_MAP);
	//pMeshData->Save(pMeshData->GetPath());

	pMapObject = new CGameObject;

	pMapObject = pMeshData->Instantiate();
	pMapObject->SetName(L"RockHigh");
	pMapObject->FrustumCheck(false);

	z = (int)(3000.f / 60.f);
	bReverseQuad = ((z % 2) != 0);
	mapY = _terrain->GetHeight(500, 3000, bReverseQuad);

	pMapObject->Transform()->SetLocalPos(Vector3(500.f, mapY * 2 + 100.f, 3000.f));
	pMapObject->Transform()->SetLocalScale(Vector3(60.f, 100.f, 60.f));//(1.0f, 1.0f, 1.0f));
	pMapObject->Transform()->SetLocalRot(Vector3(0.f, 0.f, 0.f));
	pMapObject->AddComponent(new CCollider);
	pMapObject->Collider()->SetColliderType(COLLIDER_TYPE::MESH, L"RockGrey2_high");
	pMapObject->Collider()->SetBoundingBox(BoundingBox(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingBoxExtents()* pMapObject->Transform()->GetLocalScale()));
	pMapObject->Collider()->SetBoundingSphere(BoundingSphere(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingSphereRadius() * 70.f));

	m_pCurScene->AddGameObject(L"Map", pMapObject, false);

	// 나무 

	for (int i = 0; i < 20; ++i) {
		pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\Desert\\JoshuaTree1.fbx", FBX_TYPE::DESERT_MAP);
	//	pMeshData->Save(pMeshData->GetPath());

		pMapObject = new CGameObject;

		pMapObject = pMeshData->Instantiate();
		pMapObject->SetName(L"Tree" + i);
		pMapObject->FrustumCheck(false);

		float randomX = uid(dre);
		float randomZ = uid(dre);

		z = (int)(randomZ / 60.f);
		bReverseQuad = ((z % 2) != 0);
		mapY = _terrain->GetHeight(randomX, randomZ, bReverseQuad);

		pMapObject->Transform()->SetLocalPos(Vector3(randomX, mapY * 2 + 50.f, randomZ));
		pMapObject->Transform()->SetLocalScale(Vector3(100.f, 180.f, 100.f));//(1.0f, 1.0f, 1.0f));
		pMapObject->Transform()->SetLocalRot(Vector3(-XM_PI / 2, 0.f, 0.f));
		pMapObject->AddComponent(new CCollider);
		pMapObject->Collider()->SetColliderType(COLLIDER_TYPE::MESH, L"JoshuaTree1");
		pMapObject->Collider()->SetBoundingBox(BoundingBox(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingBoxExtents() * pMapObject->Transform()->GetLocalScale()));
		pMapObject->Collider()->SetBoundingSphere(BoundingSphere(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingSphereRadius() * 120.f));

		m_pCurScene->AddGameObject(L"Map", pMapObject, false);
	}

	for (int i = 0; i < 5; ++i) {
		pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\Desert\\TreeDead1.fbx", FBX_TYPE::DESERT_MAP);



		pMapObject = new CGameObject;

		pMapObject = pMeshData->Instantiate();
		pMapObject->SetName(L"Tree" + i);
		pMapObject->FrustumCheck(false);

		float randomX = uid(dre);
		float randomZ = uid(dre);

		z = (int)(randomZ / 60.f);
		bReverseQuad = ((z % 2) != 0);
		mapY = _terrain->GetHeight(randomX, randomZ, bReverseQuad);

		pMapObject->Transform()->SetLocalPos(Vector3(randomX, mapY * 2 + 50.f, randomZ));
		pMapObject->Transform()->SetLocalScale(Vector3(100.f, 180.f, 100.f));//(1.0f, 1.0f, 1.0f));
		pMapObject->Transform()->SetLocalRot(Vector3(-XM_PI / 2, 0.f, 0.f));
		pMapObject->AddComponent(new CCollider);
		pMapObject->Collider()->SetColliderType(COLLIDER_TYPE::MESH, L"TreeDead1");
		pMapObject->Collider()->SetBoundingBox(BoundingBox(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingBoxExtents() * pMapObject->Transform()->GetLocalScale()));
		pMapObject->Collider()->SetBoundingSphere(BoundingSphere(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingSphereRadius() * 120.f));

		m_pCurScene->AddGameObject(L"Map", pMapObject, false);
	}

	for (int i = 0; i < 3; ++i) {
		pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\Desert\\TreeDead2.fbx", FBX_TYPE::DESERT_MAP);


		pMapObject = new CGameObject;

		pMapObject = pMeshData->Instantiate();
		pMapObject->SetName(L"Tree" + i);
		pMapObject->FrustumCheck(false);

		float randomX = uid(dre);
		float randomZ = uid(dre);

		z = (int)(randomZ / 60.f);
		bReverseQuad = ((z % 2) != 0);
		mapY = _terrain->GetHeight(randomX, randomZ, bReverseQuad);

		pMapObject->Transform()->SetLocalPos(Vector3(randomX, mapY * 2 + 50.f, randomZ));
		pMapObject->Transform()->SetLocalScale(Vector3(100.f, 180.f, 100.f));//(1.0f, 1.0f, 1.0f));
		pMapObject->Transform()->SetLocalRot(Vector3(-XM_PI / 2, 0.f, 0.f));
		pMapObject->AddComponent(new CCollider);
		pMapObject->Collider()->SetColliderType(COLLIDER_TYPE::MESH, L"TreeDead2");
		pMapObject->Collider()->SetBoundingBox(BoundingBox(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingBoxExtents() * pMapObject->Transform()->GetLocalScale()));
		pMapObject->Collider()->SetBoundingSphere(BoundingSphere(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingSphereRadius() * 120.f));

		m_pCurScene->AddGameObject(L"Map", pMapObject, false);
	}

	// 선인장 

	for (int i = 0; i < 6; ++i) {
		pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\Desert\\CactusSmall.fbx", FBX_TYPE::DESERT_MAP);
	//	pMeshData->Save(pMeshData->GetPath());

		pMapObject = new CGameObject;

		pMapObject = pMeshData->Instantiate();
		pMapObject->SetName(L"cactus" + i);
		pMapObject->FrustumCheck(false);

		float randomX = uid(dre);
		float randomZ = uid(dre);

		z = (int)(randomZ / 60.f);
		bReverseQuad = ((z % 2) != 0);
		mapY = _terrain->GetHeight(randomX, randomZ, bReverseQuad);

		pMapObject->Transform()->SetLocalPos(Vector3(randomX, mapY * 2 + 50.f, randomZ));
		pMapObject->Transform()->SetLocalScale(Vector3(30.f, 80.f, 80.f));//(1.0f, 1.0f, 1.0f));
		pMapObject->Transform()->SetLocalRot(Vector3(-XM_PI / 2, 0.f, 0.f));
		pMapObject->AddComponent(new CCollider);
		pMapObject->Collider()->SetColliderType(COLLIDER_TYPE::MESH, L"CactusSmall");
		pMapObject->Collider()->SetBoundingBox(BoundingBox(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingBoxExtents()* pMapObject->Transform()->GetLocalScale()));
		pMapObject->Collider()->SetBoundingSphere(BoundingSphere(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingSphereRadius() * 60.f));

		m_pCurScene->AddGameObject(L"Map", pMapObject, false);
	}

	// Wall

	float wallX = 1400.f;
	for (int i = 0; i < 9; ++i) {
		pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\Desert\\Wall.fbx", FBX_TYPE::DESERT_MAP);

		pMapObject = new CGameObject;

		pMapObject = pMeshData->Instantiate();
		pMapObject->SetName(L"Wall");
		pMapObject->FrustumCheck(false);


		z = (int)(30.f / 60.f);
		bReverseQuad = ((z % 2) != 0);
		mapY = _terrain->GetHeight(30.f, 30.f, bReverseQuad);

		pMapObject->Transform()->SetLocalPos(Vector3(30.f, mapY * 2, (wallX* i) + wallX / 2));
		pMapObject->Transform()->SetLocalScale(Vector3(310.f, 100.f, 200.f));//(1.0f, 1.0f, 1.0f));
		pMapObject->Transform()->SetLocalRot(Vector3(-XM_PI / 2, -XM_PI / 2, 0.f));
		pMapObject->AddComponent(new CCollider);
		pMapObject->Collider()->SetColliderType(COLLIDER_TYPE::MESH, L"Wall");
		pMapObject->Collider()->SetBoundingBox(BoundingBox(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingBoxExtents() * pMapObject->Transform()->GetLocalScale()));
		pMapObject->Collider()->SetBoundingSphere(BoundingSphere(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingSphereRadius() * 60.f));

		m_pCurScene->AddGameObject(L"Map", pMapObject, false);
	}
}

void CSceneMgr::CreateNpc()
{
	// Npc_1
	Ptr<CMeshData> pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\NPC\\common_people_female_4@Idle.fbx", FBX_TYPE::NPC);
	//Ptr<CMeshData> pAniData = CResMgr::GetInst()->LoadFBX(L"FBX\\Animation\\itempack@spraying.fbx", FBX_TYPE::ANI);

	CGameObject* pNpcObject = nullptr;
	pNpcObject = pMeshData->Instantiate();
	pNpcObject->SetName(L"Npc_1");
	pNpcObject->FrustumCheck(false);
	pNpcObject->Transform()->SetLocalPos(Vector3(2300.f, 240.f, 4300.f));
	pNpcObject->Transform()->SetLocalScale(Vector3(1.5f, 1.5f, 1.5f));//(1.0f, 1.0f, 1.0f));
	pNpcObject->Transform()->SetLocalRot(Vector3(-XM_PI / 2, XM_PI / 2, 0.f));
	pNpcObject->AddComponent(new CCollider);
	pNpcObject->Collider()->SetColliderType(COLLIDER_TYPE::MESH, L"Npc_1");
	pNpcObject->Collider()->SetBoundingBox(BoundingBox(pNpcObject->Transform()->GetLocalPos(), pNpcObject->MeshRender()->GetMesh()->GetBoundingBoxExtents()));
	pNpcObject->Collider()->SetBoundingSphere(BoundingSphere(pNpcObject->Transform()->GetLocalPos(), pNpcObject->MeshRender()->GetMesh()->GetBoundingSphereRadius()));
	//pNpcObject->MeshRender()->SetDynamicShadow(true);
	pNpcObject->AddComponent(new CNpcScript);
	m_pCurScene->AddGameObject(L"Npc", pNpcObject, false);

	// Idle
	CNpcScript* pNpcScript = pNpcObject->GetScript<CNpcScript>();
	pNpcScript->SetAnimationData(pMeshData->GetMesh());

	// Talk
	Ptr<CMeshData> pMeshDataKey = CResMgr::GetInst()->LoadFBX(L"FBX\\NPC\\common_people_female_4@Talking.fbx", FBX_TYPE::NPC);
	pNpcScript->SetAnimationData(pMeshDataKey->GetMesh());
	
	//--------------------------
	// Npc_2
	pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\NPC\\common_people_male_1@Dance.fbx", FBX_TYPE::NPC);
	//Ptr<CMeshData> pAniData = CResMgr::GetInst()->LoadFBX(L"FBX\\Animation\\itempack@spraying.fbx", FBX_TYPE::ANI);

	pNpcObject = new CGameObject;
	pNpcObject = pMeshData->Instantiate();
	pNpcObject->SetName(L"Npc_2");
	pNpcObject->FrustumCheck(false);
	pNpcObject->Transform()->SetLocalPos(Vector3(1300.f, 240.f, 3700.f));
	pNpcObject->Transform()->SetLocalScale(Vector3(1.5f, 1.5f, 1.5f));//(1.0f, 1.0f, 1.0f));
	pNpcObject->Transform()->SetLocalRot(Vector3(-XM_PI / 2, -XM_PI / 2, 0.f));
	pNpcObject->AddComponent(new CCollider);
	pNpcObject->Collider()->SetColliderType(COLLIDER_TYPE::MESH, L"Npc_2");
	pNpcObject->Collider()->SetBoundingBox(BoundingBox(pNpcObject->Transform()->GetLocalPos(), pNpcObject->MeshRender()->GetMesh()->GetBoundingBoxExtents()));
	pNpcObject->Collider()->SetBoundingSphere(BoundingSphere(pNpcObject->Transform()->GetLocalPos(), pNpcObject->MeshRender()->GetMesh()->GetBoundingSphereRadius()));
	//pNpcObject->MeshRender()->SetDynamicShadow(true);
	pNpcObject->AddComponent(new CNpcScript);
	m_pCurScene->AddGameObject(L"Npc", pNpcObject, false);

	// Idle
	CNpcScript* pNpcScript2 = pNpcObject->GetScript<CNpcScript>();
	pNpcScript2->SetAnimationData(pMeshData->GetMesh());

	// Talk
	pNpcScript2->SetAnimationData(pMeshDataKey->GetMesh());

	//--------------------------
	// Npc_3(store)
	pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\NPC\\common_people_male_3@SittingIdle.fbx", FBX_TYPE::NPC);
	//Ptr<CMeshData> pAniData = CResMgr::GetInst()->LoadFBX(L"FBX\\Animation\\itempack@spraying.fbx", FBX_TYPE::ANI);

	pNpcObject = new CGameObject;
	pNpcObject = pMeshData->Instantiate();
	pNpcObject->SetName(L"Npc_3");
	pNpcObject->FrustumCheck(false);
	pNpcObject->Transform()->SetLocalPos(Vector3(1300.f, 240.f, 5000.f));
	pNpcObject->Transform()->SetLocalScale(Vector3(1.5f, 1.5f, 1.5f));//(1.0f, 1.0f, 1.0f));
	pNpcObject->Transform()->SetLocalRot(Vector3(-XM_PI / 2, -XM_PI / 2, 0.f));
	pNpcObject->AddComponent(new CCollider);
	pNpcObject->Collider()->SetColliderType(COLLIDER_TYPE::MESH, L"Npc_3");
	pNpcObject->Collider()->SetBoundingBox(BoundingBox(pNpcObject->Transform()->GetLocalPos(), pNpcObject->MeshRender()->GetMesh()->GetBoundingBoxExtents()));
	pNpcObject->Collider()->SetBoundingSphere(BoundingSphere(pNpcObject->Transform()->GetLocalPos(), pNpcObject->MeshRender()->GetMesh()->GetBoundingSphereRadius()));
	//pNpcObject->MeshRender()->SetDynamicShadow(true);
	pNpcObject->AddComponent(new CNpcScript);
	m_pCurScene->AddGameObject(L"Npc", pNpcObject, false); 

	// Idle
	CNpcScript* pNpcScript3 = pNpcObject->GetScript<CNpcScript>();
	pNpcScript3->SetAnimationData(pMeshData->GetMesh());

	// Talk
	pNpcScript2->SetAnimationData(pMeshDataKey->GetMesh());
}

void CSceneMgr::init()
{
	// =================
	// 필요한 리소스 로딩
	// =================
	// Texture 로드
	Ptr<CTexture> pTex = CResMgr::GetInst()->Load<CTexture>(L"TestTex", L"Texture\\Health.png");
	Ptr<CTexture> pExplosionTex = CResMgr::GetInst()->Load<CTexture>(L"Explosion", L"Texture\\Explosion\\Explosion80.png");
	Ptr<CTexture> pBlackTex = CResMgr::GetInst()->Load<CTexture>(L"Black", L"Texture\\asd.png");
	Ptr<CTexture> pSky01 = CResMgr::GetInst()->Load<CTexture>(L"Sky01", L"Texture\\Skybox\\Sky01.png");
	Ptr<CTexture> pSky02 = CResMgr::GetInst()->Load<CTexture>(L"Sky02", L"Texture\\Skybox\\Sky02.jpg");
	Ptr<CTexture> pitemUI = CResMgr::GetInst()->Load<CTexture>(L"ItemUiTex", L"Texture\\Skybox\\Sky02.jpg");
	
	// UI
	Ptr<CTexture> pUiHug = CResMgr::GetInst()->Load<CTexture>(L"UiHug", L"Texture\\hug.png");
	Ptr<CTexture> pUiTemper = CResMgr::GetInst()->Load<CTexture>(L"UiTemper", L"Texture\\temper.png");

	Ptr<CTexture> pColor = CResMgr::GetInst()->Load<CTexture>(L"Tile", L"Texture\\Tile\\TILE_03.tga");
	Ptr<CTexture> pNormal = CResMgr::GetInst()->Load<CTexture>(L"Tile_n", L"Texture\\Tile\\TILE_03_N.tga");

	Ptr<CTexture> pDiffuseTargetTex = CResMgr::GetInst()->FindRes<CTexture>(L"DiffuseTargetTex");
	Ptr<CTexture> pNormalTargetTex = CResMgr::GetInst()->FindRes<CTexture>(L"NormalTargetTex");
	Ptr<CTexture> pPositionTargetTex = CResMgr::GetInst()->FindRes<CTexture>(L"PositionTargetTex");

	// Conversation Box
	Ptr<CTexture> pUiBoard = CResMgr::GetInst()->Load<CTexture>(L"UiBoard", L"Texture\\UIboard.png");
	// item UI texture
<<<<<<< HEAD
	Ptr<CTexture> pArrowImage = CResMgr::GetInst()->Load<CTexture>(L"BOW_IMG", L"Texture\\UI\\Items\\Weapons\\No_bg\\01_Bow_nobg.png");
	Ptr<CTexture> pSwordImage = CResMgr::GetInst()->Load<CTexture>(L"SWORD_IMG", L"Texture\\UI\\Items\\Weapons\\No_bg\\02_Sword_nobg.png");
=======
	/*Ptr<CTexture> pArrowImage = CResMgr::GetInst()->Load<CTexture>(L"BOW_IMG", L"UI/Items/Weapons/No_bg/01_Bow_nobg.png");
	Ptr<CTexture> pSwordImage = CResMgr::GetInst()->Load<CTexture>(L"SWORD_IMG", L"UI/Items/Weapons/No_bg/02_Sword_nobg.png");*/

>>>>>>> 2a08855

	// UAV 용 Texture 생성
	Ptr<CTexture> pTestUAVTexture = CResMgr::GetInst()->CreateTexture(L"UAVTexture", 1024, 1024
		, DXGI_FORMAT_R8G8B8A8_UNORM, CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT), D3D12_HEAP_FLAG_NONE
		, D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS);

	Ptr<CMaterial> pPM = CResMgr::GetInst()->FindRes<CMaterial>(L"MergeLightMtrl");
	pPM->SetData(SHADER_PARAM::TEX_3, pSky01.GetPointer());

	pPM = CResMgr::GetInst()->FindRes<CMaterial>(L"PointLightMtrl");
	pPM->SetData(SHADER_PARAM::TEX_2, pSky01.GetPointer());



	// ===============
	// Test Scene 생성
	// ===============
	m_pCurScene = new CScene;
	m_pCurScene->SetName(L"Test Scene");

	// ===============
	// Layer 이름 지정
	// ===============
	m_pCurScene->GetLayer(0)->SetName(L"Default");
	m_pCurScene->GetLayer(1)->SetName(L"Player");
	m_pCurScene->GetLayer(2)->SetName(L"Monster");
	m_pCurScene->GetLayer(3)->SetName(L"Map");
	m_pCurScene->GetLayer(4)->SetName(L"Npc");
	m_pCurScene->GetLayer(5)->SetName(L"Bullet");

	m_pCurScene->GetLayer(30)->SetName(L"UI");

	CGameObject* pObject = nullptr;

//	CreateMap();
	CreateNpc();

	// =============
   // FBX 파일 로드
   // =============
	Ptr<CMeshData> pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\Player\\PlayerMale@nIdle1.fbx", FBX_TYPE::PLAYER);
	pMeshData->Save(pMeshData->GetPath());
	
	
	CGameObject* pPlayerObj = nullptr;
	pPlayerObj = pMeshData->Instantiate();
	pPlayerObj->SetName(L"PlayerMale");
	pPlayerObj->FrustumCheck(false);
	pPlayerObj->Transform()->SetLocalPos(Vector3(0.f, 140.f, 0.f));
	pPlayerObj->Transform()->SetLocalScale(Vector3(1.f, 1.f, 1.f));//(1.0f, 1.0f, 1.0f));
	pPlayerObj->Transform()->SetLocalRot(Vector3(0.f, XM_PI, 0.f));
	pPlayerObj->AddComponent(new CCollider);
	pPlayerObj->Collider()->SetColliderType(COLLIDER_TYPE::MESH, L"PlayerMale@nWalk_F");
	pPlayerObj->Collider()->SetBoundingBox(BoundingBox(pPlayerObj->Transform()->GetLocalPos(), pPlayerObj->MeshRender()->GetMesh()->GetBoundingBoxExtents()));
	pPlayerObj->Collider()->SetBoundingSphere(BoundingSphere(pPlayerObj->Transform()->GetLocalPos(), pPlayerObj->MeshRender()->GetMesh()->GetBoundingSphereRadius()));
	pPlayerObj->MeshRender()->SetDynamicShadow(true);

	// Script 설정
	pPlayerObj->AddComponent(new CPlayerScript);

	// Animaition Data 넘겨주기

	// Idle
	CPlayerScript* playerScript = pPlayerObj->GetScript<CPlayerScript>();
	playerScript->SetAnimationData(pMeshData->GetMesh());
	g_netMgr.SetAniData(pMeshData->GetMesh());

	// walk_F
	Ptr<CMeshData> pMeshDataKey = CResMgr::GetInst()->LoadFBX(L"FBX\\Player\\PlayerMale@nWalk_F.fbx", FBX_TYPE::PLAYER);
	playerScript->SetAnimationData(pMeshDataKey->GetMesh());
	g_netMgr.SetAniData(pMeshDataKey->GetMesh());

	// walk_d
	pMeshDataKey = CResMgr::GetInst()->LoadFBX(L"FBX\\Player\\PlayerMale@nWalk_B.fbx", FBX_TYPE::PLAYER);
	playerScript->SetAnimationData(pMeshDataKey->GetMesh());
	g_netMgr.SetAniData(pMeshDataKey->GetMesh());

	// run
	pMeshDataKey = CResMgr::GetInst()->LoadFBX(L"FBX\\Player\\PlayerMale@nRun_F.fbx", FBX_TYPE::PLAYER);
	playerScript->SetAnimationData(pMeshDataKey->GetMesh());
	g_netMgr.SetAniData(pMeshDataKey->GetMesh());

	// ATTACK
	pMeshDataKey = CResMgr::GetInst()->LoadFBX(L"FBX\\Player\\PlayerMale@Attack1.fbx", FBX_TYPE::PLAYER);
	playerScript->SetAnimationData(pMeshDataKey->GetMesh());

	// Damage
	pMeshDataKey = CResMgr::GetInst()->LoadFBX(L"FBX\\Player\\PlayerMale@Damage_Strong.fbx", FBX_TYPE::PLAYER);
	playerScript->SetAnimationData(pMeshDataKey->GetMesh());
	

	m_pCurScene->AddGameObject(L"Player", pPlayerObj, false);

	// =============
	// 무기 
	// =============
	pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\Player\\PlayerMale_Weapon_Sword.fbx", FBX_TYPE::PLAYER);
	//pMeshData->Save(pMeshData->GetPath());

	CGameObject* pSword = nullptr;
	pSword = pMeshData->Instantiate();
	pSword->SetName(L"sword");
	pSword->FrustumCheck(false);
	//pSword->Transform()->SetLocalPos(Vector3(100.f, 320.f, 150.f));
	pSword->Transform()->SetLocalScale(Vector3(1.f, 1.f, 1.f));//(1.0f, 1.0f, 1.0f));
	//pSword->Transform()->SetLocalRot(Vector3(0.f, XM_PI, 0.f));
	/*pSword->AddComponent(new CCollider);
	pSword->Collider()->SetColliderType(COLLIDER_TYPE::MESH, L"PlayerMale_Weapon_Sword");*/
	// Script 설정
	pSword->AddComponent(new CSwordScript);
	CSwordScript* SwordScript = pSword->GetScript<CSwordScript>();
	SwordScript->SetBoneFinalMat(pPlayerObj->Animator3D()->GetSwordFinalBoneMat());
	m_pCurScene->AddGameObject(L"Player", pSword, false);
	pPlayerObj->AddChild(pSword);

	//=============
	// monster 2
	//=============
	pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\Monster\\TreantGuard@idle.fbx", FBX_TYPE::MONSTER);
	CGameObject* pMonster = new CGameObject;
	pMonster = pMeshData->Instantiate();
	pMonster->SetName(L"StoneMonster");
	pMonster->FrustumCheck(false);
	pMonster->Transform()->SetLocalPos(Vector3(100.f, 50.f, 100.f));
	pMonster->Transform()->SetLocalScale(Vector3(1.f, 1.f, 1.f));//(1.0f, 1.0f, 1.0f));
	pMonster->Transform()->SetLocalRot(Vector3(0.f, 0.f, 0.f));
	pMonster->AddComponent(new CCollider);
	pMonster->Collider()->SetColliderType(COLLIDER_TYPE::MESH, L"monster3_idle");
	pMonster->Collider()->SetBoundingBox(BoundingBox(pMonster->Transform()->GetLocalPos(), pMonster->MeshRender()->GetMesh()->GetBoundingBoxExtents()));
	pMonster->Collider()->SetBoundingSphere(BoundingSphere(pMonster->Transform()->GetLocalPos(),pMonster->MeshRender()->GetMesh()->GetBoundingSphereRadius()));
	// Script 설정
	pMonster->AddComponent(new CMonsterScript);
	CSceneMgr::GetInst()->GetCurScene()->AddGameObject(L"Monster", pMonster, false);
	CMonsterScript* monsterScript = pMonster->GetScript<CMonsterScript>();
	monsterScript->SetMonsterType(MONSTER_TYPE::MONSTER2);
	//animation
	//idle
	monsterScript->SetAnimationData(pMeshData->GetMesh());
	//walk
	pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\Monster\\TreantGuard@Walk Forward With Root Motion.fbx", FBX_TYPE::MONSTER);
	monsterScript->SetAnimationData(pMeshData->GetMesh());
	//dead
	pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\Monster\\TreantGuard@die.fbx", FBX_TYPE::MONSTER);
	monsterScript->SetAnimationData(pMeshData->GetMesh());
	//attack
	pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\Monster\\TreantGuard@meleeattack01.fbx", FBX_TYPE::MONSTER);
	monsterScript->SetAnimationData(pMeshData->GetMesh());
	//damage
	pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\Monster\\TreantGuard@Damage.fbx", FBX_TYPE::MONSTER);
	monsterScript->SetAnimationData(pMeshData->GetMesh());
	
	// ==================
	// Camera Object 생성
	// ==================
	// Main Camera
	CGameObject* pMainCam = new CGameObject;
	pMainCam->SetName(L"MainCam");
	pMainCam->AddComponent(new CTransform);
	pMainCam->AddComponent(new CCamera);
	pMainCam->AddComponent(new CToolCamScript);

	pMainCam->Camera()->SetProjType(PROJ_TYPE::PERSPECTIVE);
	pMainCam->Camera()->SetFar(100000.f);
	pMainCam->Camera()->SetLayerAllCheck();
	pMainCam->Camera()->SetLayerCheck(30, false);
	//pMainCam->Transform()->SetLocalPos(Vector3(0.f, 600.f, -500.f));
	//pMainCam->Transform()->SetLocalRot(Vector3(XM_PI / 6 /*0.f*/, 0.0f, 0.f));
	//vector<CToolCamScript*> camScript = (CToolCamScript*)(pMainCam->GetScripts())
	////camScript[0]->
	//pMainCam->GetScripts()[0]->SetPlayer();
	CToolCamScript* camScript = pMainCam->GetScript<CToolCamScript>();
	camScript->SetPlayer(pPlayerObj);
	// pObject->SetCam(pMainCam);
	m_pCurScene->FindLayer(L"Default")->AddGameObject(pMainCam);


	// UI Camera
	CGameObject* pUICam = new CGameObject;
	pUICam->SetName(L"UiCam");
	pUICam->AddComponent(new CTransform);
	pUICam->AddComponent(new CCamera);
	pUICam->Camera()->SetProjType(PROJ_TYPE::ORTHOGRAPHIC);
	pUICam->Camera()->SetFar(100.f);
	pUICam->Camera()->SetLayerCheck(30, true);
	m_pCurScene->FindLayer(L"Default")->AddGameObject(pUICam);
	pUICam->Camera()->SetWidth(CRenderMgr::GetInst()->GetResolution().fWidth);
	pUICam->Camera()->SetHeight(CRenderMgr::GetInst()->GetResolution().fHeight);
	CreateTargetUI();


	// ====================
	// 3D Light Object 추가
	// ====================
	pObject = new CGameObject;
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CLight);
	pObject->Light()->SetLightPos(Vector3(0.f, 1000.f, 1000.f));
	pObject->Light()->SetLightType(LIGHT_TYPE::DIR);
	pObject->Light()->SetDiffuseColor(Vector3(1.f, 1.f, 1.f));
	pObject->Light()->SetSpecular(Vector3(0.3f, 0.3f, 0.3f));
	pObject->Light()->SetAmbient(Vector3(0.1f, 0.1f, 0.1f));
	pObject->Light()->SetLightDir(Vector3(1.f, -1.f, 1.f));
	pObject->Light()->SetLightRange(1000.f);
	pObject->Transform()->SetLocalPos(Vector3(-1000.f, 1000.f, -1000.f));
	m_pCurScene->FindLayer(L"Default")->AddGameObject(pObject);


	// monster
	//idle
	Ptr<CMeshData> pMonsterMadt = CResMgr::GetInst()->LoadFBX(L"FBX\\Monster\\monster3@idle.fbx", FBX_TYPE::MONSTER);
	//walk
	pMonsterMadt = CResMgr::GetInst()->LoadFBX(L"FBX\\Monster\\monster3@walking.fbx", FBX_TYPE::MONSTER);
	//dead
	pMonsterMadt = CResMgr::GetInst()->LoadFBX(L"FBX\\Monster\\monster3@die.fbx", FBX_TYPE::MONSTER);
	//attack
	pMonsterMadt = CResMgr::GetInst()->LoadFBX(L"FBX\\Monster\\monster3@attack2.fbx", FBX_TYPE::MONSTER);
	//damage
	pMonsterMadt = CResMgr::GetInst()->LoadFBX(L"FBX\\Monster\\monster3@damage.fbx", FBX_TYPE::MONSTER);


	// ====================
	// Skybox 오브젝트 생성
	// ====================
	pObject = new CGameObject;
	pObject->SetName(L"SkyBox");
	pObject->FrustumCheck(false);
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CMeshRender);
	// MeshRender 설정
	pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"SphereMesh"));
	pObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"SkyboxMtrl"));
	pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pSky01.GetPointer());
	// AddGameObject
	m_pCurScene->FindLayer(L"Default")->AddGameObject(pObject);


	// ====================
	// Grid 오브젝트 생성
	// ====================
	pObject = new CGameObject;
	pObject->SetName(L"Grid");
	pObject->FrustumCheck(false);
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CMeshRender);
	pObject->AddComponent(new CGridScript);
	// Transform 설정
	pObject->Transform()->SetLocalScale(Vector3(100000.f, 100000.f, 1.f));
	pObject->Transform()->SetLocalRot(Vector3(XM_PI / 2.f, 0.f, 0.f));
	// MeshRender 설정
	pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"GridMtrl"));
	// Script 설정	
	pObject->GetScript<CGridScript>()->SetToolCamera(pMainCam);
	pObject->GetScript<CGridScript>()->SetGridColor(Vector3(0.8f, 0.2f, 0.2f));
	// AddGameObject
	m_pCurScene->FindLayer(L"Default")->AddGameObject(pObject);


	// Terrain
	CGameObject* pTerrainObject = new CGameObject;
	pTerrainObject->SetName(L"Terrain");
	pTerrainObject->AddComponent(new CTransform);
	pTerrainObject->AddComponent(new CMeshRender);
	pTerrainObject->AddComponent(new CTerrain);
	pTerrainObject->FrustumCheck(false);
<<<<<<< HEAD
	pTerrainObject->Transform()->SetLocalPos(Vector3(0.f, 450.f, 0.f));
=======
	pTerrainObject->Transform()->SetLocalPos(Vector3(0.f, 350.f, 0.f));
>>>>>>> 2a08855
	pTerrainObject->Transform()->SetLocalScale(Vector3(300.f, 6000.f, 300.f));		//	하이트맵 텍스쳐의 UV좌표값 기준으로 정규화된 값을 스케일링 함
	pTerrainObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"TerrainMtrl"));
	pTerrainObject->Terrain()->init();
	m_pCurScene->FindLayer(L"Default")->AddGameObject(pTerrainObject);
	pPlayerObj->GetScript<CPlayerScript>()->SetTerrain(pTerrainObject->Terrain());
	g_netMgr.SetObj(pPlayerObj);
	
	CreateMap(pPlayerObj->GetScript<CPlayerScript>()->GetTerrain());


	// ====================
	// Compute Shader Test
	// ====================
	int i = 1;
	Ptr<CMaterial> pCSMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"CSTestMtrl");
	pCSMtrl->SetData(SHADER_PARAM::INT_0, &i);
	CDevice::GetInst()->SetUAVToRegister_CS(pTestUAVTexture.GetPointer(), UAV_REGISTER::u0);
	pCSMtrl->Dispatch(1, 1024, 1); // --> 컴퓨트 쉐이더 수행	


	CCollisionMgr::GetInst()->CheckCollisionLayer(L"Player", L"Monster");
	CCollisionMgr::GetInst()->CheckCollisionLayer(L"Player", L"Map");
	CCollisionMgr::GetInst()->CheckCollisionLayer(L"Bullet", L"Monster");
	CCollisionMgr::GetInst()->CheckCollisionLayer(L"Player", L"Npc");

	

	m_pCurScene->awake();
	m_pCurScene->start();
}

void CSceneMgr::update() 
{
	m_pCurScene->update();
	m_pCurScene->lateupdate();

	// rendermgr 카메라 초기화
	CRenderMgr::GetInst()->ClearCamera();

	m_pCurScene->finalupdate();

	// 충돌 처리
	CCollisionMgr::GetInst()->update();
}

void CSceneMgr::update_tool()
{
	// rendermgr 카메라 초기화
	CRenderMgr::GetInst()->ClearCamera();
	m_pCurScene->finalupdate();
}

void CSceneMgr::FindGameObjectByTag(const wstring& _strTag, vector<CGameObject*>& _vecFindObj)
{
	for (int i = 0; i < MAX_LAYER; ++i)
	{
		const vector<CGameObject*>& vecObject = m_pCurScene->GetLayer(i)->GetObjects();
		for (size_t j = 0; j < vecObject.size(); ++j)
		{
			if (_strTag == vecObject[j]->GetName())
			{
				_vecFindObj.push_back(vecObject[j]);
			}
		}
	}
}

bool Compare(CGameObject* _pLeft, CGameObject* _pRight)
{
	return (_pLeft->Transform()->GetWorldPos().z < _pRight->Transform()->GetWorldPos().z);
}
