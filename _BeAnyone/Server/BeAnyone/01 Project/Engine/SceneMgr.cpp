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
#include "Button.h"

#include "TimeMgr.h"
#include "KeyMgr.h"
#include "Camera.h"
#include "Terrain.h"

#include "CollisionMgr.h"
#include "EventMgr.h"
#include "RenderMgr.h"
#include "ItemMgr.h"
#include "Device.h"
#include "Core.h"

#include "PlayerScript.h"
#include "ToolCamScript.h"
#include "GridScript.h"
#include "SwordScript.h"
#include "MonsterScript.h"
#include "NpcScript.h"
#include "TreeScript.h"

// UI
#include "TemperUiScript.h"
#include "StaticUI.h"

using namespace std;

default_random_engine dre;
uniform_real_distribution<float> uid(9000, 76000);

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

void CSceneMgr::CreateTargetUI(CGameObject* _camObj)
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

	Vector3 vScale;

#endif
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

	pMapObject->Transform()->SetLocalPos(Vector3(1000.f, mapY * 2 , 3000.f));
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

	pMapObject->Transform()->SetLocalPos(Vector3(1000.f, mapY * 2 , 5500.f));
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
	//왼쪽
	float wallX = 1400.f;
	for (int i = 0; i < 55; ++i) {
		pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\Desert\\Wall.fbx", FBX_TYPE::DESERT_MAP);

		pMapObject = new CGameObject;

		pMapObject = pMeshData->Instantiate();
		pMapObject->SetName(L"Wall");
		pMapObject->FrustumCheck(false);


		z = (int)(30.f / 60.f);
		bReverseQuad = ((z % 2) != 0);
		mapY = _terrain->GetHeight(30.f, (wallX * i) + wallX / 2, true);

		pMapObject->Transform()->SetLocalPos(Vector3(250.f, mapY * 2, (wallX* i) + wallX / 2));
		pMapObject->Transform()->SetLocalScale(Vector3(310.f, 200.f, 1000.f));//(1.0f, 1.0f, 1.0f));
		pMapObject->Transform()->SetLocalRot(Vector3(-XM_PI / 2, -XM_PI / 2, 0.f));
		/*pMapObject->AddComponent(new CCollider);
		pMapObject->Collider()->SetColliderType(COLLIDER_TYPE::MESH, L"Wall");
		pMapObject->Collider()->SetBoundingBox(BoundingBox(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingBoxExtents() * pMapObject->Transform()->GetLocalScale()));
		pMapObject->Collider()->SetBoundingSphere(BoundingSphere(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingSphereRadius() * 60.f));*/

		m_pCurScene->AddGameObject(L"Map", pMapObject, false);
	}

	//오른쪽
	for (int i = 0; i < 55; ++i) {
		pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\Desert\\Wall.fbx", FBX_TYPE::DESERT_MAP);

		pMapObject = new CGameObject;

		pMapObject = pMeshData->Instantiate();
		pMapObject->SetName(L"Wall");
		pMapObject->FrustumCheck(false);


		z = (int)(30.f / 60.f);
		bReverseQuad = ((z % 2) != 0);
		mapY = _terrain->GetHeight(30.f, (wallX * i) + wallX / 2, true);

		pMapObject->Transform()->SetLocalPos(Vector3(76500.f, mapY * 2, (wallX * i) + wallX / 2));
		pMapObject->Transform()->SetLocalScale(Vector3(310.f, 200.f, 1000.f));//(1.0f, 1.0f, 1.0f));
		pMapObject->Transform()->SetLocalRot(Vector3(-XM_PI / 2, XM_PI / 2, 0.f));
		/*pMapObject->AddComponent(new CCollider);
		pMapObject->Collider()->SetColliderType(COLLIDER_TYPE::MESH, L"Wall");
		pMapObject->Collider()->SetBoundingBox(BoundingBox(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingBoxExtents() * pMapObject->Transform()->GetLocalScale()));
		pMapObject->Collider()->SetBoundingSphere(BoundingSphere(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingSphereRadius() * 60.f));*/

		m_pCurScene->AddGameObject(L"Map", pMapObject, false);
	}

	// 위
	for (int i = 0; i < 55; ++i) {
		pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\Desert\\Wall.fbx", FBX_TYPE::DESERT_MAP);

		pMapObject = new CGameObject;

		pMapObject = pMeshData->Instantiate();
		pMapObject->SetName(L"Wall");
		pMapObject->FrustumCheck(false);


		z = (int)(30.f / 60.f);
		bReverseQuad = ((z % 2) != 0);
		mapY = _terrain->GetHeight((wallX * i) + wallX / 2, 76300.f, true);

		pMapObject->Transform()->SetLocalPos(Vector3((wallX* i) + wallX / 2, mapY * 2, 76300.f));
		pMapObject->Transform()->SetLocalScale(Vector3(310.f, 200.f, 1000.f));//(1.0f, 1.0f, 1.0f));
		pMapObject->Transform()->SetLocalRot(Vector3(-XM_PI / 2, 0.f, 0.f));
		/*pMapObject->AddComponent(new CCollider);
		pMapObject->Collider()->SetColliderType(COLLIDER_TYPE::MESH, L"Wall");
		pMapObject->Collider()->SetBoundingBox(BoundingBox(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingBoxExtents() * pMapObject->Transform()->GetLocalScale()));
		pMapObject->Collider()->SetBoundingSphere(BoundingSphere(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingSphereRadius() * 60.f));*/

		m_pCurScene->AddGameObject(L"Map", pMapObject, false);
	}

	// 아래
	for (int i = 0; i < 55; ++i) {
		pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\Desert\\Wall.fbx", FBX_TYPE::DESERT_MAP);

		pMapObject = new CGameObject;

		pMapObject = pMeshData->Instantiate();
		pMapObject->SetName(L"Wall");
		pMapObject->FrustumCheck(false);


		z = (int)(30.f / 60.f);
		bReverseQuad = ((z % 2) != 0);
		mapY = _terrain->GetHeight((wallX * i) + wallX / 2, 300.f, true);

		pMapObject->Transform()->SetLocalPos(Vector3((wallX * i) + wallX / 2, mapY * 2, 300.f));
		pMapObject->Transform()->SetLocalScale(Vector3(310.f, 200.f, 1000.f));//(1.0f, 1.0f, 1.0f));
		pMapObject->Transform()->SetLocalRot(Vector3(-XM_PI / 2, XM_PI, 0.f));
		/*pMapObject->AddComponent(new CCollider);
		pMapObject->Collider()->SetColliderType(COLLIDER_TYPE::MESH, L"Wall");
		pMapObject->Collider()->SetBoundingBox(BoundingBox(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingBoxExtents() * pMapObject->Transform()->GetLocalScale()));
		pMapObject->Collider()->SetBoundingSphere(BoundingSphere(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingSphereRadius() * 60.f));*/

		m_pCurScene->AddGameObject(L"Map", pMapObject, false);
	}
}

void CSceneMgr::CreateNewMap(CTerrain* _terrain)
{
	// Tree
	Ptr<CMeshData> pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\DesertMap\\Tree_01.fbx", FBX_TYPE::NEW_DESERT_MAP);
	//pMeshData->Save(pMeshData->GetPath());
	CGameObject* pMapObject = nullptr;
	pMapObject = pMeshData->Instantiate();
	pMapObject->SetName(L"Tree");
	pMapObject->FrustumCheck(false);
	int z = (int)(3000.f / 60.f);
	bool bReverseQuad = ((z % 2) != 0);
	float mapY = _terrain->GetHeight(1000.f, 3000, bReverseQuad);
	pMapObject->Transform()->SetLocalPos(Vector3(1000.f, mapY * 2 , 3000.f));
	pMapObject->Transform()->SetLocalScale(Vector3(60.f, 60.f, 60.f));//(1.0f, 1.0f, 1.0f));
	pMapObject->Transform()->SetLocalRot(Vector3(-XM_PI / 2, 0.f, 0.f));
	pMapObject->AddComponent(new CCollider);
	pMapObject->Collider()->SetColliderType(COLLIDER_TYPE::MESH, L"Tree");
	pMapObject->Collider()->SetBoundingBox(BoundingBox(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingBoxExtents(pMapObject->Transform()->GetLocalScale())));
	pMapObject->Collider()->SetBoundingSphere(BoundingSphere(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingSphereRadius() * 60.f));
	m_pCurScene->AddGameObject(L"Map", pMapObject, false);

	// 나무
	{
		// 동그란 나무
		for (int i = 0; i < 150; ++i) {
			pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\DesertMap\\Tree_01.fbx", FBX_TYPE::NEW_DESERT_MAP);
			//	pMeshData->Save(pMeshData->GetPath());

			pMapObject = new CGameObject;

			pMapObject = pMeshData->Instantiate();
			pMapObject->SetName(L"Tree" + i);
			pMapObject->FrustumCheck(true);
			pMapObject->AddComponent(new CTreeScript);

			float randomX = uid(dre);
			float randomZ = uid(dre);

			z = (int)(randomZ / 60.f);
			bReverseQuad = ((z % 2) != 0);
			mapY = _terrain->GetHeight(randomX, randomZ, bReverseQuad);

			pMapObject->Transform()->SetLocalPos(Vector3(randomX, mapY * 2, randomZ));
			pMapObject->Transform()->SetLocalScale(Vector3(200.f, 130.f, 160.f));//(1.0f, 1.0f, 1.0f));
			pMapObject->Transform()->SetLocalRot(Vector3(-XM_PI / 2, 0.f, 0.f));
			pMapObject->AddComponent(new CCollider);
			pMapObject->Collider()->SetColliderType(COLLIDER_TYPE::MESH, L"JoshuaTree1");
			pMapObject->Collider()->SetBoundingBox(BoundingBox(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingBoxExtents() * pMapObject->Transform()->GetLocalScale()));
			pMapObject->Collider()->SetBoundingSphere(BoundingSphere(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingSphereRadius() * (120.f / 3.f)));

			m_pCurScene->AddGameObject(L"Map", pMapObject, false);
			// 나무 충돌 스크립트
			CTreeScript* pTreeScript = pMapObject->GetScript<CTreeScript>();
			//-------item load---------
			// 나뭇가지
			pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\Item\\Branch.fbx", FBX_TYPE::ITEM);
			pTreeScript->SetItemData(pMeshData);
			// 사과
			pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\Item\\Apple.fbx", FBX_TYPE::ITEM);
			pTreeScript->SetItemData(pMeshData);
			//-----------
			pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\DesertMap\\Stump_01.fbx", FBX_TYPE::NEW_DESERT_MAP);
			pTreeScript->SetStumpData(pMeshData);
			pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\DesertMap\\Stump_04.fbx", FBX_TYPE::NEW_DESERT_MAP);
			pTreeScript->SetStumpData(pMeshData);
			pTreeScript->SetTerrain(_terrain);	// Terrain
			//-----------------------------------		
		
		}

		// 죽은 나무
		for (int i = 0; i < 100; ++i) {
			pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\DesertMap\\Tree_05.fbx", FBX_TYPE::NEW_DESERT_MAP);
			//	pMeshData->Save(pMeshData->GetPath());

			pMapObject = new CGameObject;

			pMapObject = pMeshData->Instantiate();
			pMapObject->SetName(L"Tree" + i);
			pMapObject->FrustumCheck(true);
			pMapObject->AddComponent(new CTreeScript);

			float randomX = uid(dre);
			float randomZ = uid(dre);

			z = (int)(randomZ / 60.f);
			bReverseQuad = ((z % 2) != 0);
			mapY = _terrain->GetHeight(randomX, randomZ, bReverseQuad);

			pMapObject->Transform()->SetLocalPos(Vector3(randomX, mapY * 2, randomZ));
			pMapObject->Transform()->SetLocalScale(Vector3(200.f, 130.f, 160.f));//(1.0f, 1.0f, 1.0f));
			pMapObject->Transform()->SetLocalRot(Vector3(-XM_PI / 2, 0.f, 0.f));
			pMapObject->AddComponent(new CCollider);
			pMapObject->Collider()->SetColliderType(COLLIDER_TYPE::MESH, L"JoshuaTree1");
			pMapObject->Collider()->SetBoundingBox(BoundingBox(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingBoxExtents() * pMapObject->Transform()->GetLocalScale()));
			pMapObject->Collider()->SetBoundingSphere(BoundingSphere(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingSphereRadius() * (120.f / 3.f)));

			m_pCurScene->AddGameObject(L"Map", pMapObject, false);
			CTreeScript* pTreeScript = pMapObject->GetScript<CTreeScript>();
			//-------item load---------
			// 나뭇가지
			pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\Item\\Branch.fbx", FBX_TYPE::ITEM);
			pTreeScript->SetItemData(pMeshData);
			// 사과
			pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\Item\\Apple.fbx", FBX_TYPE::ITEM);
			pTreeScript->SetItemData(pMeshData);
			//-----------
			pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\DesertMap\\Stump_01.fbx", FBX_TYPE::NEW_DESERT_MAP);
			pTreeScript->SetStumpData(pMeshData);
			pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\DesertMap\\Stump_04.fbx", FBX_TYPE::NEW_DESERT_MAP);
			pTreeScript->SetStumpData(pMeshData);
			pTreeScript->SetTerrain(_terrain);	// Terrain
		}

		// 뾰족한 나무
		for (int i = 0; i < 100; ++i) {
			pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\DesertMap\\Tree_07.fbx", FBX_TYPE::NEW_DESERT_MAP);
			//	pMeshData->Save(pMeshData->GetPath());

			pMapObject = new CGameObject;

			pMapObject = pMeshData->Instantiate();
			pMapObject->SetName(L"Tree" + i);
			pMapObject->FrustumCheck(true);
			pMapObject->AddComponent(new CTreeScript);

			float randomX = uid(dre);
			float randomZ = uid(dre);

			z = (int)(randomZ / 60.f);
			bReverseQuad = ((z % 2) != 0);
			mapY = _terrain->GetHeight(randomX, randomZ, bReverseQuad);

			pMapObject->Transform()->SetLocalPos(Vector3(randomX, mapY * 2, randomZ));
			pMapObject->Transform()->SetLocalScale(Vector3(200.f, 130.f, 160.f));//(1.0f, 1.0f, 1.0f));
			pMapObject->Transform()->SetLocalRot(Vector3(-XM_PI / 2, 0.f, 0.f));
			pMapObject->AddComponent(new CCollider);
			pMapObject->Collider()->SetColliderType(COLLIDER_TYPE::MESH, L"JoshuaTree1");
			pMapObject->Collider()->SetBoundingBox(BoundingBox(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingBoxExtents() * pMapObject->Transform()->GetLocalScale()));
			pMapObject->Collider()->SetBoundingSphere(BoundingSphere(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingSphereRadius() * (120.f / 3.f)));

			m_pCurScene->AddGameObject(L"Map", pMapObject, false);
			CTreeScript* pTreeScript = pMapObject->GetScript<CTreeScript>();
			//-------item load---------
			// 나뭇가지
			pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\Item\\Branch.fbx", FBX_TYPE::ITEM);
			pTreeScript->SetItemData(pMeshData);
			// 사과
			pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\Item\\Apple.fbx", FBX_TYPE::ITEM);
			pTreeScript->SetItemData(pMeshData);
			//-----------
			pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\DesertMap\\Stump_01.fbx", FBX_TYPE::NEW_DESERT_MAP);
			pTreeScript->SetStumpData(pMeshData);
			pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\DesertMap\\Stump_04.fbx", FBX_TYPE::NEW_DESERT_MAP);
			pTreeScript->SetStumpData(pMeshData);
			pTreeScript->SetTerrain(_terrain);	// Terrain
		}
	}
	// 집1
	{
		pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\DesertMap\\House_03.fbx", FBX_TYPE::NEW_DESERT_MAP);
		//pMeshData->Save(pMeshData->GetPath());
		pMapObject = new CGameObject;
		pMapObject = pMeshData->Instantiate();
		pMapObject->SetName(L"House1");
		pMapObject->FrustumCheck(false);

		z = (int)(3000.f / 60.f);
		bReverseQuad = ((z % 2) != 0);
		mapY = _terrain->GetHeight(1000.f, 3000, bReverseQuad);

		pMapObject->Transform()->SetLocalPos(Vector3(1000.f, mapY * 2, 3000.f));
		pMapObject->Transform()->SetLocalScale(Vector3(130.f, 130.f, 130.f));//(1.0f, 1.0f, 1.0f));
		pMapObject->Transform()->SetLocalRot(Vector3(-XM_PI / 2, -XM_PI / 2, 0.f));
		pMapObject->AddComponent(new CCollider);
		pMapObject->Collider()->SetColliderType(COLLIDER_TYPE::MESH, L"House1");
		pMapObject->Collider()->SetBoundingBox(BoundingBox(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingBoxExtents(pMapObject->Transform()->GetLocalScale())));
		pMapObject->Collider()->SetBoundingSphere(BoundingSphere(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingSphereRadius() * 60.f));
		m_pCurScene->AddGameObject(L"Map", pMapObject, false);

		// 집1 tile
		pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\DesertMap\\Tile8.fbx", FBX_TYPE::NEW_DESERT_MAP);
		//pMeshData->Save(pMeshData->GetPath());
		pMapObject = new CGameObject;
		pMapObject = pMeshData->Instantiate();
		pMapObject->SetName(L"Tile");
		pMapObject->FrustumCheck(false);

		z = (int)(3000.f / 60.f);
		bReverseQuad = ((z % 2) != 0);
		mapY = _terrain->GetHeight(1600.f, 3000.f, bReverseQuad);

		pMapObject->Transform()->SetLocalPos(Vector3(1600.f, mapY * 2, 3000.f));
		pMapObject->Transform()->SetLocalScale(Vector3(150.f, 200.f, 100.f));//(1.0f, 1.0f, 1.0f));
		pMapObject->Transform()->SetLocalRot(Vector3(-XM_PI / 2, XM_PI / 4, 0.f));
		pMapObject->AddComponent(new CCollider);
		pMapObject->Collider()->SetColliderType(COLLIDER_TYPE::MESH, L"Tile5");
		pMapObject->Collider()->SetBoundingBox(BoundingBox(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingBoxExtents(pMapObject->Transform()->GetLocalScale())));
		pMapObject->Collider()->SetBoundingSphere(BoundingSphere(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingSphereRadius() * 60.f));
		m_pCurScene->AddGameObject(L"Map", pMapObject, false);

		// 집2 fence2
		pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\DesertMap\\Fence_01.fbx", FBX_TYPE::NEW_DESERT_MAP);
		//pMeshData->Save(pMeshData->GetPath());
		pMapObject = new CGameObject;
		pMapObject = pMeshData->Instantiate();
		pMapObject->SetName(L"Fence");
		pMapObject->FrustumCheck(false);

		z = (int)(3000.f / 60.f);
		bReverseQuad = ((z % 2) != 0);
		mapY = _terrain->GetHeight(1100.f, 3300.f, bReverseQuad);

		pMapObject->Transform()->SetLocalPos(Vector3(1100.f, mapY * 2, 3400.f));
		pMapObject->Transform()->SetLocalScale(Vector3(100.f, 150.f, 100.f));//(1.0f, 1.0f, 1.0f));
		pMapObject->Transform()->SetLocalRot(Vector3(-XM_PI / 2, XM_PI / 2, 0.f));
		pMapObject->AddComponent(new CCollider);
		pMapObject->Collider()->SetColliderType(COLLIDER_TYPE::MESH, L"Fence");
		pMapObject->Collider()->SetBoundingBox(BoundingBox(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingBoxExtents(pMapObject->Transform()->GetLocalScale())));
		pMapObject->Collider()->SetBoundingSphere(BoundingSphere(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingSphereRadius() * 60.f));
		m_pCurScene->AddGameObject(L"Map", pMapObject, false);

		for (int i = 0; i < 3; ++i) {
			// 집2 fence2
			pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\DesertMap\\Fence_02.fbx", FBX_TYPE::NEW_DESERT_MAP);
			//pMeshData->Save(pMeshData->GetPath());
			pMapObject = new CGameObject;
			pMapObject = pMeshData->Instantiate();
			pMapObject->SetName(L"Fence");
			pMapObject->FrustumCheck(false);

			z = (int)(3000.f / 60.f);
			bReverseQuad = ((z % 2) != 0);
			mapY = _terrain->GetHeight(1300.f, 3300.f, bReverseQuad);

			pMapObject->Transform()->SetLocalPos(Vector3(1300.f + (210.f * i), mapY * 2, 3400.f));
			pMapObject->Transform()->SetLocalScale(Vector3(100.f, 150.f, 100.f));//(1.0f, 1.0f, 1.0f));
			pMapObject->Transform()->SetLocalRot(Vector3(-XM_PI / 2, XM_PI, 0.f));
			pMapObject->AddComponent(new CCollider);
			pMapObject->Collider()->SetColliderType(COLLIDER_TYPE::MESH, L"Fence");
			pMapObject->Collider()->SetBoundingBox(BoundingBox(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingBoxExtents(pMapObject->Transform()->GetLocalScale())));
			pMapObject->Collider()->SetBoundingSphere(BoundingSphere(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingSphereRadius() * 60.f));
			m_pCurScene->AddGameObject(L"Map", pMapObject, false);
		}

		for (int i = 0; i < 3; ++i) {
			// 집2 fence2
			pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\DesertMap\\Fence_02.fbx", FBX_TYPE::NEW_DESERT_MAP);
			//pMeshData->Save(pMeshData->GetPath());
			pMapObject = new CGameObject;
			pMapObject = pMeshData->Instantiate();
			pMapObject->SetName(L"Fence");
			pMapObject->FrustumCheck(false);

			z = (int)(3000.f / 60.f);
			bReverseQuad = ((z % 2) != 0);
			mapY = _terrain->GetHeight(1720.f, 2300.f, bReverseQuad);

			pMapObject->Transform()->SetLocalPos(Vector3(1720.f, mapY * 2, 2300.f + (210 * i)));
			pMapObject->Transform()->SetLocalScale(Vector3(100.f, 150.f, 100.f));//(1.0f, 1.0f, 1.0f));
			pMapObject->Transform()->SetLocalRot(Vector3(-XM_PI / 2, XM_PI / 2, 0.f));
			pMapObject->AddComponent(new CCollider);
			pMapObject->Collider()->SetColliderType(COLLIDER_TYPE::MESH, L"Fence");
			pMapObject->Collider()->SetBoundingBox(BoundingBox(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingBoxExtents(pMapObject->Transform()->GetLocalScale())));
			pMapObject->Collider()->SetBoundingSphere(BoundingSphere(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingSphereRadius() * 60.f));
			m_pCurScene->AddGameObject(L"Map", pMapObject, false);
		}
	}{}
	// 집2
	{
		pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\DesertMap\\House_07.fbx", FBX_TYPE::NEW_DESERT_MAP);
		//pMeshData->Save(pMeshData->GetPath());
		pMapObject = new CGameObject;
		pMapObject = pMeshData->Instantiate();
		pMapObject->SetName(L"House2");
		pMapObject->FrustumCheck(false);

		z = (int)(3000.f / 60.f);
		bReverseQuad = ((z % 2) != 0);
		mapY = _terrain->GetHeight(1000.f, 1630.f, bReverseQuad);

		pMapObject->Transform()->SetLocalPos(Vector3(1000.f, mapY * 2, 1630.f));
		pMapObject->Transform()->SetLocalScale(Vector3(130.f, 130.f, 130.f));//(1.0f, 1.0f, 1.0f));
		pMapObject->Transform()->SetLocalRot(Vector3(-XM_PI / 2, -XM_PI / 2, 0.f));
		pMapObject->AddComponent(new CCollider);
		pMapObject->Collider()->SetColliderType(COLLIDER_TYPE::MESH, L"House2");
		pMapObject->Collider()->SetBoundingBox(BoundingBox(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingBoxExtents(pMapObject->Transform()->GetLocalScale())));
		pMapObject->Collider()->SetBoundingSphere(BoundingSphere(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingSphereRadius() * 60.f));
		m_pCurScene->AddGameObject(L"Map", pMapObject, false);

		// 집2 tile
		pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\DesertMap\\Tile8.fbx", FBX_TYPE::NEW_DESERT_MAP);
		//pMeshData->Save(pMeshData->GetPath());
		pMapObject = new CGameObject;
		pMapObject = pMeshData->Instantiate();
		pMapObject->SetName(L"Tile");
		pMapObject->FrustumCheck(false);

		z = (int)(3000.f / 60.f);
		bReverseQuad = ((z % 2) != 0);
		mapY = _terrain->GetHeight(1600.f, 1700.f, bReverseQuad);

		pMapObject->Transform()->SetLocalPos(Vector3(1600.f, mapY * 2, 1700.f));
		pMapObject->Transform()->SetLocalScale(Vector3(150.f, 250.f, 100.f));//(1.0f, 1.0f, 1.0f));
		pMapObject->Transform()->SetLocalRot(Vector3(-XM_PI / 2, XM_PI / 4, 0.f));
		pMapObject->AddComponent(new CCollider);
		pMapObject->Collider()->SetColliderType(COLLIDER_TYPE::MESH, L"Tile5");
		pMapObject->Collider()->SetBoundingBox(BoundingBox(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingBoxExtents(pMapObject->Transform()->GetLocalScale())));
		pMapObject->Collider()->SetBoundingSphere(BoundingSphere(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingSphereRadius() * 60.f));
		m_pCurScene->AddGameObject(L"Map", pMapObject, false);

		// 집2 fence1
		pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\DesertMap\\Fence_01.fbx", FBX_TYPE::NEW_DESERT_MAP);
		//pMeshData->Save(pMeshData->GetPath());
		pMapObject = new CGameObject;
		pMapObject = pMeshData->Instantiate();
		pMapObject->SetName(L"Fence");
		pMapObject->FrustumCheck(false);

		z = (int)(3000.f / 60.f);
		bReverseQuad = ((z % 2) != 0);
		mapY = _terrain->GetHeight(1100.f, 2100.f, bReverseQuad);

		pMapObject->Transform()->SetLocalPos(Vector3(1100.f, mapY * 2, 2100.f));
		pMapObject->Transform()->SetLocalScale(Vector3(100.f, 150.f, 100.f));//(1.0f, 1.0f, 1.0f));
		pMapObject->Transform()->SetLocalRot(Vector3(-XM_PI / 2, XM_PI / 2, 0.f));
		pMapObject->AddComponent(new CCollider);
		pMapObject->Collider()->SetColliderType(COLLIDER_TYPE::MESH, L"Fence");
		pMapObject->Collider()->SetBoundingBox(BoundingBox(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingBoxExtents(pMapObject->Transform()->GetLocalScale())));
		pMapObject->Collider()->SetBoundingSphere(BoundingSphere(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingSphereRadius() * 60.f));
		m_pCurScene->AddGameObject(L"Map", pMapObject, false);

		// 집2 fence2
		pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\DesertMap\\Fence_01.fbx", FBX_TYPE::NEW_DESERT_MAP);
		//pMeshData->Save(pMeshData->GetPath());
		pMapObject = new CGameObject;
		pMapObject = pMeshData->Instantiate();
		pMapObject->SetName(L"Fence");
		pMapObject->FrustumCheck(false);

		z = (int)(3000.f / 60.f);
		bReverseQuad = ((z % 2) != 0);
		mapY = _terrain->GetHeight(1100.f, 1150.f, bReverseQuad);

		pMapObject->Transform()->SetLocalPos(Vector3(1100.f, mapY * 2, 1150.f));
		pMapObject->Transform()->SetLocalScale(Vector3(100.f, 150.f, 100.f));//(1.0f, 1.0f, 1.0f));
		pMapObject->Transform()->SetLocalRot(Vector3(-XM_PI / 2, XM_PI / 2, 0.f));
		pMapObject->AddComponent(new CCollider);
		pMapObject->Collider()->SetColliderType(COLLIDER_TYPE::MESH, L"Fence");
		pMapObject->Collider()->SetBoundingBox(BoundingBox(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingBoxExtents(pMapObject->Transform()->GetLocalScale())));
		pMapObject->Collider()->SetBoundingSphere(BoundingSphere(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingSphereRadius() * 60.f));
		m_pCurScene->AddGameObject(L"Map", pMapObject, false);

		for (int i = 0; i < 3; ++i) {
			// 집2 fence2
			pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\DesertMap\\Fence_02.fbx", FBX_TYPE::NEW_DESERT_MAP);
			//pMeshData->Save(pMeshData->GetPath());
			pMapObject = new CGameObject;
			pMapObject = pMeshData->Instantiate();
			pMapObject->SetName(L"Fence");
			pMapObject->FrustumCheck(false);

			z = (int)(3000.f / 60.f);
			bReverseQuad = ((z % 2) != 0);
			mapY = _terrain->GetHeight(1300.f, 1050.f, bReverseQuad);

			pMapObject->Transform()->SetLocalPos(Vector3(1300.f + (210.f * i), mapY * 2, 1050.f));
			pMapObject->Transform()->SetLocalScale(Vector3(100.f, 150.f, 100.f));//(1.0f, 1.0f, 1.0f));
			pMapObject->Transform()->SetLocalRot(Vector3(-XM_PI / 2, XM_PI, 0.f));
			pMapObject->AddComponent(new CCollider);
			pMapObject->Collider()->SetColliderType(COLLIDER_TYPE::MESH, L"Fence");
			pMapObject->Collider()->SetBoundingBox(BoundingBox(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingBoxExtents(pMapObject->Transform()->GetLocalScale())));
			pMapObject->Collider()->SetBoundingSphere(BoundingSphere(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingSphereRadius() * 60.f));
			m_pCurScene->AddGameObject(L"Map", pMapObject, false);
		}

		for (int i = 0; i < 3; ++i) {
			// 집2 fence2
			pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\DesertMap\\Fence_02.fbx", FBX_TYPE::NEW_DESERT_MAP);
			//pMeshData->Save(pMeshData->GetPath());
			pMapObject = new CGameObject;
			pMapObject = pMeshData->Instantiate();
			pMapObject->SetName(L"Fence");
			pMapObject->FrustumCheck(false);

			z = (int)(3000.f / 60.f);
			bReverseQuad = ((z % 2) != 0);
			mapY = _terrain->GetHeight(1300.f, 2100.f, bReverseQuad);

			pMapObject->Transform()->SetLocalPos(Vector3(1300.f + (210.f * i), mapY * 2 , 2100.f));
			pMapObject->Transform()->SetLocalScale(Vector3(100.f, 150.f, 100.f));//(1.0f, 1.0f, 1.0f));
			pMapObject->Transform()->SetLocalRot(Vector3(-XM_PI / 2, XM_PI, 0.f));
			pMapObject->AddComponent(new CCollider);
			pMapObject->Collider()->SetColliderType(COLLIDER_TYPE::MESH, L"Fence");
			pMapObject->Collider()->SetBoundingBox(BoundingBox(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingBoxExtents(pMapObject->Transform()->GetLocalScale())));
			pMapObject->Collider()->SetBoundingSphere(BoundingSphere(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingSphereRadius() * 60.f));
			m_pCurScene->AddGameObject(L"Map", pMapObject, false);
		}

		for (int i = 0; i < 2; ++i) {
			// 집2 fence2
			pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\DesertMap\\Fence_02.fbx", FBX_TYPE::NEW_DESERT_MAP);
			//pMeshData->Save(pMeshData->GetPath());
			pMapObject = new CGameObject;
			pMapObject = pMeshData->Instantiate();
			pMapObject->SetName(L"Fence");
			pMapObject->FrustumCheck(false);

			z = (int)(3000.f / 60.f);
			bReverseQuad = ((z % 2) != 0);
			mapY = _terrain->GetHeight(1720.f, 1260.f, bReverseQuad);

			pMapObject->Transform()->SetLocalPos(Vector3(1720.f, mapY * 2 , 1260.f + (210 * i)));
			pMapObject->Transform()->SetLocalScale(Vector3(100.f, 150.f, 100.f));//(1.0f, 1.0f, 1.0f));
			pMapObject->Transform()->SetLocalRot(Vector3(-XM_PI / 2, XM_PI / 2, 0.f));
			pMapObject->AddComponent(new CCollider);
			pMapObject->Collider()->SetColliderType(COLLIDER_TYPE::MESH, L"Fence");
			pMapObject->Collider()->SetBoundingBox(BoundingBox(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingBoxExtents(pMapObject->Transform()->GetLocalScale())));
			pMapObject->Collider()->SetBoundingSphere(BoundingSphere(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingSphereRadius() * 60.f));
			m_pCurScene->AddGameObject(L"Map", pMapObject, false);
		}
	}{}
	// 왼쪽 상점
	{
		// 나무
		pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\DesertMap\\Tree_01.fbx", FBX_TYPE::NEW_DESERT_MAP);
		//pMeshData->Save(pMeshData->GetPath());

		pMapObject = new CGameObject;
		pMapObject = pMeshData->Instantiate();
		pMapObject->SetName(L"Tree");
		pMapObject->FrustumCheck(false);

		z = (int)(3000.f / 60.f);
		bReverseQuad = ((z % 2) != 0);
		mapY = _terrain->GetHeight(1300.f, 3600.f, bReverseQuad);

		pMapObject->Transform()->SetLocalPos(Vector3(1300.f, mapY * 2 , 3600.f));
		pMapObject->Transform()->SetLocalScale(Vector3(150.f, 100.f, 100.f));//(1.0f, 1.0f, 1.0f));
		pMapObject->Transform()->SetLocalRot(Vector3(-XM_PI / 2, 0.f, 0.f));
		pMapObject->AddComponent(new CCollider);
		pMapObject->Collider()->SetColliderType(COLLIDER_TYPE::MESH, L"Tree");
		pMapObject->Collider()->SetBoundingBox(BoundingBox(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingBoxExtents(pMapObject->Transform()->GetLocalScale())));
		pMapObject->Collider()->SetBoundingSphere(BoundingSphere(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingSphereRadius() * 60.f));
		m_pCurScene->AddGameObject(L"Map", pMapObject, false);
		
		// 천막
		pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\DesertMap\\Market_Roof_02.fbx", FBX_TYPE::NEW_DESERT_MAP);
		//pMeshData->Save(pMeshData->GetPath());
		pMapObject = new CGameObject;
		pMapObject = pMeshData->Instantiate();
		pMapObject->SetName(L"Market_Roof");
		pMapObject->FrustumCheck(false);

		z = (int)(3000.f / 60.f);
		bReverseQuad = ((z % 2) != 0);
		mapY = _terrain->GetHeight(1300.f, 4100.f, bReverseQuad);

		pMapObject->Transform()->SetLocalPos(Vector3(1300.f, mapY * 2 , 4100.f));
		pMapObject->Transform()->SetLocalScale(Vector3(300.f, 200.f, 200.f));//(1.0f, 1.0f, 1.0f));
		pMapObject->Transform()->SetLocalRot(Vector3(-XM_PI / 2, -XM_PI / 2, 0.f));
		pMapObject->AddComponent(new CCollider);
		pMapObject->Collider()->SetColliderType(COLLIDER_TYPE::MESH, L"Market_Roof");
		pMapObject->Collider()->SetBoundingBox(BoundingBox(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingBoxExtents(pMapObject->Transform()->GetLocalScale())));
		pMapObject->Collider()->SetBoundingSphere(BoundingSphere(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingSphereRadius() * 60.f));
		m_pCurScene->AddGameObject(L"Map", pMapObject, false);

		// 테이블
		pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\DesertMap\\Market_Table_03.fbx", FBX_TYPE::NEW_DESERT_MAP);
		//pMeshData->Save(pMeshData->GetPath());
		pMapObject = new CGameObject;
		pMapObject = pMeshData->Instantiate();
		pMapObject->SetName(L"Market_Table");
		pMapObject->FrustumCheck(false);

		z = (int)(3000.f / 60.f);
		bReverseQuad = ((z % 2) != 0);
		mapY = _terrain->GetHeight(1300.f, 4100.f, bReverseQuad);

		pMapObject->Transform()->SetLocalPos(Vector3(1300.f, mapY * 2 , 4100.f));
		pMapObject->Transform()->SetLocalScale(Vector3(300.f, 200.f, 200.f));//(1.0f, 1.0f, 1.0f));
		pMapObject->Transform()->SetLocalRot(Vector3(-XM_PI / 2, -XM_PI / 2, 0.f));
		pMapObject->AddComponent(new CCollider);
		pMapObject->Collider()->SetColliderType(COLLIDER_TYPE::MESH, L"Market_Table");
		pMapObject->Collider()->SetBoundingBox(BoundingBox(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingBoxExtents(pMapObject->Transform()->GetLocalScale())));
		pMapObject->Collider()->SetBoundingSphere(BoundingSphere(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingSphereRadius() * 60.f));
		m_pCurScene->AddGameObject(L"Map", pMapObject, false);

		// 수레
		pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\DesertMap\\Cart_02.fbx", FBX_TYPE::NEW_DESERT_MAP);
		//pMeshData->Save(pMeshData->GetPath());
		pMapObject = new CGameObject;
		pMapObject = pMeshData->Instantiate();
		pMapObject->SetName(L"Cart");
		pMapObject->FrustumCheck(false);

		z = (int)(3000.f / 60.f);
		bReverseQuad = ((z % 2) != 0);
		mapY = _terrain->GetHeight(1600.f, 4400.f, bReverseQuad);

		pMapObject->Transform()->SetLocalPos(Vector3(1600.f, mapY * 2 , 4400.f));
		pMapObject->Transform()->SetLocalScale(Vector3(100.f, 150.f, 150.f));//(1.0f, 1.0f, 1.0f));
		pMapObject->Transform()->SetLocalRot(Vector3(-XM_PI / 2, -XM_PI / 2, 0.f));
		pMapObject->AddComponent(new CCollider);
		pMapObject->Collider()->SetColliderType(COLLIDER_TYPE::MESH, L"Cart");
		pMapObject->Collider()->SetBoundingBox(BoundingBox(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingBoxExtents(pMapObject->Transform()->GetLocalScale())));
		pMapObject->Collider()->SetBoundingSphere(BoundingSphere(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingSphereRadius() * 60.f));
		m_pCurScene->AddGameObject(L"Map", pMapObject, false);
	}
	// 집3
	{
		// 집3
		pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\DesertMap\\House_01.fbx", FBX_TYPE::NEW_DESERT_MAP);
		//pMeshData->Save(pMeshData->GetPath());
		pMapObject = new CGameObject;
		pMapObject = pMeshData->Instantiate();
		pMapObject->SetName(L"House_01");
		pMapObject->FrustumCheck(false);

		z = (int)(3000.f / 60.f);
		bReverseQuad = ((z % 2) != 0);
		mapY = _terrain->GetHeight(1000.f, 5200.f, bReverseQuad);

		pMapObject->Transform()->SetLocalPos(Vector3(1000.f, mapY * 2 , 5200.f));
		pMapObject->Transform()->SetLocalScale(Vector3(130.f, 130.f, 130.f));//(1.0f, 1.0f, 1.0f));
		pMapObject->Transform()->SetLocalRot(Vector3(-XM_PI / 2, -XM_PI / 2, 0.f));
		pMapObject->AddComponent(new CCollider);
		pMapObject->Collider()->SetColliderType(COLLIDER_TYPE::MESH, L"House_01");
		pMapObject->Collider()->SetBoundingBox(BoundingBox(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingBoxExtents(pMapObject->Transform()->GetLocalScale())));
		pMapObject->Collider()->SetBoundingSphere(BoundingSphere(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingSphereRadius() * 60.f));
		m_pCurScene->AddGameObject(L"Map", pMapObject, false);

		// 집3 fence1
		pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\DesertMap\\Fence_01.fbx", FBX_TYPE::NEW_DESERT_MAP);
		//pMeshData->Save(pMeshData->GetPath());
		pMapObject = new CGameObject;
		pMapObject = pMeshData->Instantiate();
		pMapObject->SetName(L"Fence");
		pMapObject->FrustumCheck(false);

		z = (int)(3000.f / 60.f);
		bReverseQuad = ((z % 2) != 0);
		mapY = _terrain->GetHeight(1100.f, 4720.f, bReverseQuad);

		pMapObject->Transform()->SetLocalPos(Vector3(1100.f, mapY * 2 , 4720.f));
		pMapObject->Transform()->SetLocalScale(Vector3(100.f, 150.f, 100.f));//(1.0f, 1.0f, 1.0f));
		pMapObject->Transform()->SetLocalRot(Vector3(-XM_PI / 2, XM_PI / 2, 0.f));
		pMapObject->AddComponent(new CCollider);
		pMapObject->Collider()->SetColliderType(COLLIDER_TYPE::MESH, L"Fence");
		pMapObject->Collider()->SetBoundingBox(BoundingBox(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingBoxExtents(pMapObject->Transform()->GetLocalScale())));
		pMapObject->Collider()->SetBoundingSphere(BoundingSphere(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingSphereRadius() * 60.f));
		m_pCurScene->AddGameObject(L"Map", pMapObject, false);

		// 집3 fence2
		pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\DesertMap\\Fence_01.fbx", FBX_TYPE::NEW_DESERT_MAP);
		//pMeshData->Save(pMeshData->GetPath());
		pMapObject = new CGameObject;
		pMapObject = pMeshData->Instantiate();
		pMapObject->SetName(L"Fence");
		pMapObject->FrustumCheck(false);

		z = (int)(3000.f / 60.f);
		bReverseQuad = ((z % 2) != 0);
		mapY = _terrain->GetHeight(1100.f, 5680.f, bReverseQuad);

		pMapObject->Transform()->SetLocalPos(Vector3(1100.f, mapY * 2 , 5680.f));
		pMapObject->Transform()->SetLocalScale(Vector3(100.f, 150.f, 100.f));//(1.0f, 1.0f, 1.0f));
		pMapObject->Transform()->SetLocalRot(Vector3(-XM_PI / 2, XM_PI / 2, 0.f));
		pMapObject->AddComponent(new CCollider);
		pMapObject->Collider()->SetColliderType(COLLIDER_TYPE::MESH, L"Fence");
		pMapObject->Collider()->SetBoundingBox(BoundingBox(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingBoxExtents(pMapObject->Transform()->GetLocalScale())));
		pMapObject->Collider()->SetBoundingSphere(BoundingSphere(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingSphereRadius() * 60.f));
		m_pCurScene->AddGameObject(L"Map", pMapObject, false);

		for (int i = 0; i < 3; ++i) {
			// 집2 fence3
			pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\DesertMap\\Fence_02.fbx", FBX_TYPE::NEW_DESERT_MAP);
			//pMeshData->Save(pMeshData->GetPath());
			pMapObject = new CGameObject;
			pMapObject = pMeshData->Instantiate();
			pMapObject->SetName(L"Fence");
			pMapObject->FrustumCheck(false);

			z = (int)(3000.f / 60.f);
			bReverseQuad = ((z % 2) != 0);
			mapY = _terrain->GetHeight(1300.f, 4610.f, bReverseQuad);

			pMapObject->Transform()->SetLocalPos(Vector3(1300.f + (210.f * i), mapY * 2 , 4610.f));
			pMapObject->Transform()->SetLocalScale(Vector3(100.f, 150.f, 100.f));//(1.0f, 1.0f, 1.0f));
			pMapObject->Transform()->SetLocalRot(Vector3(-XM_PI / 2, XM_PI, 0.f));
			pMapObject->AddComponent(new CCollider);
			pMapObject->Collider()->SetColliderType(COLLIDER_TYPE::MESH, L"Fence");
			pMapObject->Collider()->SetBoundingBox(BoundingBox(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingBoxExtents(pMapObject->Transform()->GetLocalScale())));
			pMapObject->Collider()->SetBoundingSphere(BoundingSphere(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingSphereRadius() * 60.f));
			m_pCurScene->AddGameObject(L"Map", pMapObject, false);
		}

		for (int i = 0; i < 3; ++i) {
			// 집2 fence4
			pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\DesertMap\\Fence_02.fbx", FBX_TYPE::NEW_DESERT_MAP);
			//pMeshData->Save(pMeshData->GetPath());
			pMapObject = new CGameObject;
			pMapObject = pMeshData->Instantiate();
			pMapObject->SetName(L"Fence");
			pMapObject->FrustumCheck(false);

			z = (int)(3000.f / 60.f);
			bReverseQuad = ((z % 2) != 0);
			mapY = _terrain->GetHeight(1300.f, 5790.f, bReverseQuad);

			pMapObject->Transform()->SetLocalPos(Vector3(1300.f + (210.f * i), mapY * 2 , 5790.f));
			pMapObject->Transform()->SetLocalScale(Vector3(100.f, 150.f, 100.f));//(1.0f, 1.0f, 1.0f));
			pMapObject->Transform()->SetLocalRot(Vector3(-XM_PI / 2, XM_PI, 0.f));
			pMapObject->AddComponent(new CCollider);
			pMapObject->Collider()->SetColliderType(COLLIDER_TYPE::MESH, L"Fence");
			pMapObject->Collider()->SetBoundingBox(BoundingBox(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingBoxExtents(pMapObject->Transform()->GetLocalScale())));
			pMapObject->Collider()->SetBoundingSphere(BoundingSphere(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingSphereRadius() * 60.f));
			m_pCurScene->AddGameObject(L"Map", pMapObject, false);
		}

		for (int i = 0; i < 2; ++i) {
			// 집3 fence2
			pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\DesertMap\\Fence_02.fbx", FBX_TYPE::NEW_DESERT_MAP);
			//pMeshData->Save(pMeshData->GetPath());
			pMapObject = new CGameObject;
			pMapObject = pMeshData->Instantiate();
			pMapObject->SetName(L"Fence");
			pMapObject->FrustumCheck(false);

			z = (int)(3000.f / 60.f);
			bReverseQuad = ((z % 2) != 0);
			mapY = _terrain->GetHeight(1720.f, 4820.f, bReverseQuad);

			pMapObject->Transform()->SetLocalPos(Vector3(1720.f, mapY * 2 , 4820.f + (210 * i)));
			pMapObject->Transform()->SetLocalScale(Vector3(100.f, 150.f, 100.f));//(1.0f, 1.0f, 1.0f));
			pMapObject->Transform()->SetLocalRot(Vector3(-XM_PI / 2, XM_PI / 2, 0.f));
			pMapObject->AddComponent(new CCollider);
			pMapObject->Collider()->SetColliderType(COLLIDER_TYPE::MESH, L"Fence");
			pMapObject->Collider()->SetBoundingBox(BoundingBox(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingBoxExtents(pMapObject->Transform()->GetLocalScale())));
			pMapObject->Collider()->SetBoundingSphere(BoundingSphere(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingSphereRadius() * 60.f));
			m_pCurScene->AddGameObject(L"Map", pMapObject, false);
		}
	}

	// --------------------- 오른쪽 --------------------- 
	{}
	// 집4
	{
		// 집3
		pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\DesertMap\\House_06.fbx", FBX_TYPE::NEW_DESERT_MAP);
		//pMeshData->Save(pMeshData->GetPath());
		pMapObject = new CGameObject;
		pMapObject = pMeshData->Instantiate();
		pMapObject->SetName(L"House2");
		pMapObject->FrustumCheck(false);

		z = (int)(3000.f / 60.f);
		bReverseQuad = ((z % 2) != 0);
		mapY = _terrain->GetHeight(6000.f, 2000.f, bReverseQuad);

		pMapObject->Transform()->SetLocalPos(Vector3(6000.f, mapY * 2 , 2000.f));
		pMapObject->Transform()->SetLocalScale(Vector3(130.f, 130.f, 130.f));//(1.0f, 1.0f, 1.0f));
		pMapObject->Transform()->SetLocalRot(Vector3(-XM_PI / 2, XM_PI / 2, 0.f));
		pMapObject->AddComponent(new CCollider);
		pMapObject->Collider()->SetColliderType(COLLIDER_TYPE::MESH, L"House2");
		pMapObject->Collider()->SetBoundingBox(BoundingBox(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingBoxExtents(pMapObject->Transform()->GetLocalScale())));
		pMapObject->Collider()->SetBoundingSphere(BoundingSphere(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingSphereRadius() * 60.f));
		m_pCurScene->AddGameObject(L"Map", pMapObject, false);

		// 집3 tile
		pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\DesertMap\\Tile8.fbx", FBX_TYPE::NEW_DESERT_MAP);
		//pMeshData->Save(pMeshData->GetPath());
		pMapObject = new CGameObject;
		pMapObject = pMeshData->Instantiate();
		pMapObject->SetName(L"Tile");
		pMapObject->FrustumCheck(false);

		z = (int)(3000.f / 60.f);
		bReverseQuad = ((z % 2) != 0);
		mapY = _terrain->GetHeight(5500.f, 1820.f, bReverseQuad);

		pMapObject->Transform()->SetLocalPos(Vector3(5500.f, mapY * 2 , 1820.f));
		pMapObject->Transform()->SetLocalScale(Vector3(150.f, 250.f, 100.f));//(1.0f, 1.0f, 1.0f));
		pMapObject->Transform()->SetLocalRot(Vector3(-XM_PI / 2, XM_PI / 4, 0.f));
		pMapObject->AddComponent(new CCollider);
		pMapObject->Collider()->SetColliderType(COLLIDER_TYPE::MESH, L"Tile5");
		pMapObject->Collider()->SetBoundingBox(BoundingBox(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingBoxExtents(pMapObject->Transform()->GetLocalScale())));
		pMapObject->Collider()->SetBoundingSphere(BoundingSphere(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingSphereRadius() * 60.f));
		m_pCurScene->AddGameObject(L"Map", pMapObject, false);

		// 집3 fence1
		pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\DesertMap\\Fence_01.fbx", FBX_TYPE::NEW_DESERT_MAP);
		//pMeshData->Save(pMeshData->GetPath());
		pMapObject = new CGameObject;
		pMapObject = pMeshData->Instantiate();
		pMapObject->SetName(L"Fence");
		pMapObject->FrustumCheck(false);

		z = (int)(3000.f / 60.f);
		bReverseQuad = ((z % 2) != 0);
		mapY = _terrain->GetHeight(5700.f, 1530.f, bReverseQuad);

		pMapObject->Transform()->SetLocalPos(Vector3(5700.f, mapY * 2 , 1530.f));
		pMapObject->Transform()->SetLocalScale(Vector3(100.f, 150.f, 100.f));//(1.0f, 1.0f, 1.0f));
		pMapObject->Transform()->SetLocalRot(Vector3(-XM_PI / 2, XM_PI / 2, 0.f));
		pMapObject->AddComponent(new CCollider);
		pMapObject->Collider()->SetColliderType(COLLIDER_TYPE::MESH, L"Fence");
		pMapObject->Collider()->SetBoundingBox(BoundingBox(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingBoxExtents(pMapObject->Transform()->GetLocalScale())));
		pMapObject->Collider()->SetBoundingSphere(BoundingSphere(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingSphereRadius() * 60.f));
		m_pCurScene->AddGameObject(L"Map", pMapObject, false);

		// 집3 fence2
		pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\DesertMap\\Fence_01.fbx", FBX_TYPE::NEW_DESERT_MAP);
		//pMeshData->Save(pMeshData->GetPath());
		pMapObject = new CGameObject;
		pMapObject = pMeshData->Instantiate();
		pMapObject->SetName(L"Fence");
		pMapObject->FrustumCheck(false);

		z = (int)(3000.f / 60.f);
		bReverseQuad = ((z % 2) != 0);
		mapY = _terrain->GetHeight(5700.f, 2850.f, bReverseQuad);

		pMapObject->Transform()->SetLocalPos(Vector3(5700.f, mapY * 2 , 2850.f));
		pMapObject->Transform()->SetLocalScale(Vector3(100.f, 150.f, 100.f));//(1.0f, 1.0f, 1.0f));
		pMapObject->Transform()->SetLocalRot(Vector3(-XM_PI / 2, XM_PI / 2, 0.f));
		pMapObject->AddComponent(new CCollider);
		pMapObject->Collider()->SetColliderType(COLLIDER_TYPE::MESH, L"Fence");
		pMapObject->Collider()->SetBoundingBox(BoundingBox(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingBoxExtents(pMapObject->Transform()->GetLocalScale())));
		pMapObject->Collider()->SetBoundingSphere(BoundingSphere(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingSphereRadius() * 60.f));
		m_pCurScene->AddGameObject(L"Map", pMapObject, false);

		for (int i = 0; i < 2; ++i) {
			// 집3 fence3
			pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\DesertMap\\Fence_02.fbx", FBX_TYPE::NEW_DESERT_MAP);
			//pMeshData->Save(pMeshData->GetPath());
			pMapObject = new CGameObject;
			pMapObject = pMeshData->Instantiate();
			pMapObject->SetName(L"Fence");
			pMapObject->FrustumCheck(false);

			z = (int)(3000.f / 60.f);
			bReverseQuad = ((z % 2) != 0);
			mapY = _terrain->GetHeight(5490.f, 1430.f, bReverseQuad);

			pMapObject->Transform()->SetLocalPos(Vector3(5490.f - (210.f * i), mapY * 2 , 1430.f));
			pMapObject->Transform()->SetLocalScale(Vector3(100.f, 150.f, 100.f));//(1.0f, 1.0f, 1.0f));
			pMapObject->Transform()->SetLocalRot(Vector3(-XM_PI / 2, 0.f, 0.f));
			pMapObject->AddComponent(new CCollider);
			pMapObject->Collider()->SetColliderType(COLLIDER_TYPE::MESH, L"Fence");
			pMapObject->Collider()->SetBoundingBox(BoundingBox(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingBoxExtents(pMapObject->Transform()->GetLocalScale())));
			pMapObject->Collider()->SetBoundingSphere(BoundingSphere(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingSphereRadius() * 60.f));
			m_pCurScene->AddGameObject(L"Map", pMapObject, false);
		}

		for (int i = 0; i < 2; ++i) {
			// 집3 fence3
			pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\DesertMap\\Fence_02.fbx", FBX_TYPE::NEW_DESERT_MAP);
			//pMeshData->Save(pMeshData->GetPath());
			pMapObject = new CGameObject;
			pMapObject = pMeshData->Instantiate();
			pMapObject->SetName(L"Fence");
			pMapObject->FrustumCheck(false);

			z = (int)(3000.f / 60.f);
			bReverseQuad = ((z % 2) != 0);
			mapY = _terrain->GetHeight(5490.f, 2950.f, bReverseQuad);

			pMapObject->Transform()->SetLocalPos(Vector3(5490.f - (210.f * i), mapY * 2 , 2950.f));
			pMapObject->Transform()->SetLocalScale(Vector3(100.f, 150.f, 100.f));//(1.0f, 1.0f, 1.0f));
			pMapObject->Transform()->SetLocalRot(Vector3(-XM_PI / 2, 0.f, 0.f));
			pMapObject->AddComponent(new CCollider);
			pMapObject->Collider()->SetColliderType(COLLIDER_TYPE::MESH, L"Fence");
			pMapObject->Collider()->SetBoundingBox(BoundingBox(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingBoxExtents(pMapObject->Transform()->GetLocalScale())));
			pMapObject->Collider()->SetBoundingSphere(BoundingSphere(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingSphereRadius() * 60.f));
			m_pCurScene->AddGameObject(L"Map", pMapObject, false);
		}

		for (int i = 0; i < 3; ++i) {
			// 집2 fence2
			pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\DesertMap\\Fence_02.fbx", FBX_TYPE::NEW_DESERT_MAP);
			//pMeshData->Save(pMeshData->GetPath());
			pMapObject = new CGameObject;
			pMapObject = pMeshData->Instantiate();
			pMapObject->SetName(L"Fence");
			pMapObject->FrustumCheck(false);

			z = (int)(3000.f / 60.f);
			bReverseQuad = ((z % 2) != 0);
			mapY = _terrain->GetHeight(5280.f, 2750.f, bReverseQuad);

			pMapObject->Transform()->SetLocalPos(Vector3(5280.f, mapY * 2 , 2750.f - (210 * i)));
			pMapObject->Transform()->SetLocalScale(Vector3(100.f, 150.f, 100.f));//(1.0f, 1.0f, 1.0f));
			pMapObject->Transform()->SetLocalRot(Vector3(-XM_PI / 2, -XM_PI / 2, 0.f));
			pMapObject->AddComponent(new CCollider);
			pMapObject->Collider()->SetColliderType(COLLIDER_TYPE::MESH, L"Fence");
			pMapObject->Collider()->SetBoundingBox(BoundingBox(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingBoxExtents(pMapObject->Transform()->GetLocalScale())));
			pMapObject->Collider()->SetBoundingSphere(BoundingSphere(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingSphereRadius() * 60.f));
			m_pCurScene->AddGameObject(L"Map", pMapObject, false);
		}

		pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\DesertMap\\Shelving_01.fbx", FBX_TYPE::NEW_DESERT_MAP);
		//pMeshData->Save(pMeshData->GetPath());
		pMapObject = new CGameObject;
		pMapObject = pMeshData->Instantiate();
		pMapObject->SetName(L"Fence");
		pMapObject->FrustumCheck(false);

		z = (int)(3000.f / 60.f);
		bReverseQuad = ((z % 2) != 0);
		mapY = _terrain->GetHeight(5800.f, 2850.f, bReverseQuad);

		pMapObject->Transform()->SetLocalPos(Vector3(5800.f, mapY * 2 , 3100.f));
		pMapObject->Transform()->SetLocalScale(Vector3(400.f, 300.f, 300.f));//(1.0f, 1.0f, 1.0f));
		pMapObject->Transform()->SetLocalRot(Vector3(-XM_PI / 2, XM_PI / 2, 0.f));
		pMapObject->AddComponent(new CCollider);
		pMapObject->Collider()->SetColliderType(COLLIDER_TYPE::MESH, L"Fence");
		pMapObject->Collider()->SetBoundingBox(BoundingBox(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingBoxExtents(pMapObject->Transform()->GetLocalScale())));
		pMapObject->Collider()->SetBoundingSphere(BoundingSphere(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingSphereRadius() * 60.f));
		m_pCurScene->AddGameObject(L"Map", pMapObject, false);
	}{}
	// 집5
	{
	
	}
	{}
	// 오른쪽 상점
	{
		pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\DesertMap\\Market_Roof_01.fbx", FBX_TYPE::NEW_DESERT_MAP);
		//pMeshData->Save(pMeshData->GetPath());
		pMapObject = new CGameObject;
		pMapObject = pMeshData->Instantiate();
		pMapObject->SetName(L"Market_Roof");
		pMapObject->FrustumCheck(false);

		z = (int)(3000.f / 60.f);
		bReverseQuad = ((z % 2) != 0);
		mapY = _terrain->GetHeight(5800.f, 3750.f, bReverseQuad);

		pMapObject->Transform()->SetLocalPos(Vector3(5800.f, mapY * 2 , 3750.f));
		pMapObject->Transform()->SetLocalScale(Vector3(300.f, 200.f, 200.f));//(1.0f, 1.0f, 1.0f));
		pMapObject->Transform()->SetLocalRot(Vector3(-XM_PI / 2, XM_PI / 2, 0.f));
		pMapObject->AddComponent(new CCollider);
		pMapObject->Collider()->SetColliderType(COLLIDER_TYPE::MESH, L"Market_Roof");
		pMapObject->Collider()->SetBoundingBox(BoundingBox(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingBoxExtents(pMapObject->Transform()->GetLocalScale())));
		pMapObject->Collider()->SetBoundingSphere(BoundingSphere(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingSphereRadius() * 60.f));
		m_pCurScene->AddGameObject(L"Map", pMapObject, false);

		// 테이블
		pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\DesertMap\\Market_Table_01.fbx", FBX_TYPE::NEW_DESERT_MAP);
		//pMeshData->Save(pMeshData->GetPath());
		pMapObject = new CGameObject;
		pMapObject = pMeshData->Instantiate();
		pMapObject->SetName(L"Market_Table");
		pMapObject->FrustumCheck(false);

		z = (int)(3000.f / 60.f);
		bReverseQuad = ((z % 2) != 0);
		mapY = _terrain->GetHeight(5800.f, 3750.f, bReverseQuad);

		pMapObject->Transform()->SetLocalPos(Vector3(5800.f, mapY * 2 , 3750.f));
		pMapObject->Transform()->SetLocalScale(Vector3(300.f, 200.f, 200.f));//(1.0f, 1.0f, 1.0f));
		pMapObject->Transform()->SetLocalRot(Vector3(-XM_PI / 2, XM_PI / 2, 0.f));
		pMapObject->AddComponent(new CCollider);
		pMapObject->Collider()->SetColliderType(COLLIDER_TYPE::MESH, L"Market_Table");
		pMapObject->Collider()->SetBoundingBox(BoundingBox(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingBoxExtents(pMapObject->Transform()->GetLocalScale())));
		pMapObject->Collider()->SetBoundingSphere(BoundingSphere(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingSphereRadius() * 60.f));
		m_pCurScene->AddGameObject(L"Map", pMapObject, false);
	}
	// 가운데 나무, 돌
	{
		// 가운데 나무
		pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\DesertMap\\Tree_01.fbx", FBX_TYPE::NEW_DESERT_MAP);
		//pMeshData->Save(pMeshData->GetPath());
		pMapObject = new CGameObject;
		pMapObject = pMeshData->Instantiate();
		pMapObject->SetName(L"Tree");
		pMapObject->FrustumCheck(false);
		pMapObject->AddComponent(new CTreeScript);
		
		z = (int)(3000.f / 60.f);
		bReverseQuad = ((z % 2) != 0);
		mapY = _terrain->GetHeight(3000.f, 3000.f, bReverseQuad);
		
		pMapObject->Transform()->SetLocalPos(Vector3(3000.f, mapY * 2 , 3000.f));
		pMapObject->Transform()->SetLocalScale(Vector3(200.f, 130.f, 160.f));//(1.0f, 1.0f, 1.0f));
		pMapObject->Transform()->SetLocalRot(Vector3(-XM_PI / 2, 0.f, 0.f));
		pMapObject->AddComponent(new CCollider);
		pMapObject->Collider()->SetColliderType(COLLIDER_TYPE::MESH, L"Tree");
		pMapObject->Collider()->SetBoundingBox(BoundingBox(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingBoxExtents(pMapObject->Transform()->GetLocalScale())));
		pMapObject->Collider()->SetBoundingSphere(BoundingSphere(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingSphereRadius() * 60.f));
		m_pCurScene->AddGameObject(L"Map", pMapObject, false);
		// 나무 충돌 스크립트
		// 나무 충돌 스크립트
		CTreeScript* pTreeScript = pMapObject->GetScript<CTreeScript>();
		//-------item load---------
		// 나뭇가지
		pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\Item\\Branch.fbx", FBX_TYPE::ITEM);
		pTreeScript->SetItemData(pMeshData);
		// 사과
		pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\Item\\Apple.fbx", FBX_TYPE::ITEM);
		pTreeScript->SetItemData(pMeshData);
		//-----------
		pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\DesertMap\\Stump_01.fbx", FBX_TYPE::NEW_DESERT_MAP);
		pTreeScript->SetStumpData(pMeshData);
		pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\DesertMap\\Stump_04.fbx", FBX_TYPE::NEW_DESERT_MAP);
		pTreeScript->SetStumpData(pMeshData);
		pTreeScript->SetTerrain(_terrain);	// Terrain
		//-----------------------------------
		
		// 가운데 나무2
		pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\DesertMap\\Tree_07.fbx", FBX_TYPE::NEW_DESERT_MAP);
		//pMeshData->Save(pMeshData->GetPath());
		pMapObject = new CGameObject;
		pMapObject = pMeshData->Instantiate();
		pMapObject->SetName(L"Tree");
		pMapObject->FrustumCheck(false);
		
		z = (int)(3000.f / 60.f);
		bReverseQuad = ((z % 2) != 0);
		mapY = _terrain->GetHeight(3100.f, 2700.f, bReverseQuad);
		
		pMapObject->Transform()->SetLocalPos(Vector3(3100.f, mapY * 2 , 2700.f));
		pMapObject->Transform()->SetLocalScale(Vector3(210.f, 130.f, 160.f));//(1.0f, 1.0f, 1.0f));
		pMapObject->Transform()->SetLocalRot(Vector3(-XM_PI / 2, 0.f, 0.f));
		pMapObject->AddComponent(new CCollider);
		pMapObject->Collider()->SetColliderType(COLLIDER_TYPE::MESH, L"Tree");
		pMapObject->Collider()->SetBoundingBox(BoundingBox(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingBoxExtents(pMapObject->Transform()->GetLocalScale())));
		pMapObject->Collider()->SetBoundingSphere(BoundingSphere(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingSphereRadius() * 60.f));
		m_pCurScene->AddGameObject(L"Map", pMapObject, false);
		
		// 가운데 나무3
		pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\DesertMap\\Tree_05.fbx", FBX_TYPE::NEW_DESERT_MAP);
		//pMeshData->Save(pMeshData->GetPath());
		pMapObject = new CGameObject;
		pMapObject = pMeshData->Instantiate();
		pMapObject->SetName(L"Tree");
		pMapObject->FrustumCheck(false);
		
		z = (int)(3000.f / 60.f);
		bReverseQuad = ((z % 2) != 0);
		mapY = _terrain->GetHeight(3000.f, 2400.f, bReverseQuad);
		
		pMapObject->Transform()->SetLocalPos(Vector3(3000.f, mapY * 2 , 2400.f));
		pMapObject->Transform()->SetLocalScale(Vector3(500.f, 350.f, 400.f));//(1.0f, 1.0f, 1.0f));
		pMapObject->Transform()->SetLocalRot(Vector3(-XM_PI / 2, 0.f, 0.f));
		pMapObject->AddComponent(new CCollider);
		pMapObject->Collider()->SetColliderType(COLLIDER_TYPE::MESH, L"Tree");
		pMapObject->Collider()->SetBoundingBox(BoundingBox(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingBoxExtents(pMapObject->Transform()->GetLocalScale())));
		pMapObject->Collider()->SetBoundingSphere(BoundingSphere(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingSphereRadius() * 60.f));
		m_pCurScene->AddGameObject(L"Map", pMapObject, false);
		
		// 가운데 돌1
		pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\DesertMap\\Rock_04.fbx", FBX_TYPE::NEW_DESERT_MAP);
		//pMeshData->Save(pMeshData->GetPath());
		pMapObject = new CGameObject;
		pMapObject = pMeshData->Instantiate();
		pMapObject->SetName(L"Rock");
		pMapObject->FrustumCheck(false);
		
		z = (int)(3000.f / 60.f);
		bReverseQuad = ((z % 2) != 0);
		mapY = _terrain->GetHeight(3300.f, 2600.f, bReverseQuad);
		
		pMapObject->Transform()->SetLocalPos(Vector3(3300.f, mapY * 2 , 2600.f));
		pMapObject->Transform()->SetLocalScale(Vector3(400.f, 550.f, 550.f));//(1.0f, 1.0f, 1.0f));
		pMapObject->Transform()->SetLocalRot(Vector3(-XM_PI / 2, 0.f, 0.f));
		pMapObject->AddComponent(new CCollider);
		pMapObject->Collider()->SetColliderType(COLLIDER_TYPE::MESH, L"Rock");
		pMapObject->Collider()->SetBoundingBox(BoundingBox(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingBoxExtents(pMapObject->Transform()->GetLocalScale())));
		pMapObject->Collider()->SetBoundingSphere(BoundingSphere(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingSphereRadius() * 60.f));
		m_pCurScene->AddGameObject(L"Map", pMapObject, false);
		
		// 가운데 돌1
		pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\DesertMap\\Rock_07.fbx", FBX_TYPE::NEW_DESERT_MAP);
		//pMeshData->Save(pMeshData->GetPath());
		pMapObject = new CGameObject;
		pMapObject = pMeshData->Instantiate();
		pMapObject->SetName(L"Rock");
		pMapObject->FrustumCheck(false);
		
		z = (int)(3000.f / 60.f);
		bReverseQuad = ((z % 2) != 0);
		mapY = _terrain->GetHeight(3200.f, 2900.f, bReverseQuad);
		
		pMapObject->Transform()->SetLocalPos(Vector3(3200.f, mapY * 2 , 2900.f));
		pMapObject->Transform()->SetLocalScale(Vector3(450., 450.f, 450.f));//(1.0f, 1.0f, 1.0f));
		pMapObject->Transform()->SetLocalRot(Vector3(-XM_PI / 2, 0.f, 0.f));
		pMapObject->AddComponent(new CCollider);
		pMapObject->Collider()->SetColliderType(COLLIDER_TYPE::MESH, L"Rock");
		pMapObject->Collider()->SetBoundingBox(BoundingBox(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingBoxExtents(pMapObject->Transform()->GetLocalScale())));
		pMapObject->Collider()->SetBoundingSphere(BoundingSphere(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingSphereRadius() * 60.f));
		m_pCurScene->AddGameObject(L"Map", pMapObject, false);
	}{}
	// 가운데 우물
	{
		// 우물
		pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\DesertMap\\Well.fbx", FBX_TYPE::NEW_DESERT_MAP);
		//pMeshData->Save(pMeshData->GetPath());
		pMapObject = new CGameObject;
		pMapObject = pMeshData->Instantiate();
		pMapObject->SetName(L"Well");
		pMapObject->FrustumCheck(false);

		z = (int)(3000.f / 60.f);
		bReverseQuad = ((z % 2) != 0);
		mapY = _terrain->GetHeight(3200.f, 3700.f, bReverseQuad);

		pMapObject->Transform()->SetLocalPos(Vector3(3200.f, mapY * 2 , 3700.f));
		pMapObject->Transform()->SetLocalScale(Vector3(300.f, 300.f, 300.f));//(1.0f, 1.0f, 1.0f));
		pMapObject->Transform()->SetLocalRot(Vector3(-XM_PI / 2, 0.f, 0.f));
		pMapObject->AddComponent(new CCollider);
		pMapObject->Collider()->SetColliderType(COLLIDER_TYPE::MESH, L"Well");
		pMapObject->Collider()->SetBoundingBox(BoundingBox(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingBoxExtents(pMapObject->Transform()->GetLocalScale())));
		pMapObject->Collider()->SetBoundingSphere(BoundingSphere(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingSphereRadius() * 60.f));
		m_pCurScene->AddGameObject(L"Map", pMapObject, false);

		// 테이블
		for (int i = 0; i < 2; ++i) {
			pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\DesertMap\\Table_01.fbx", FBX_TYPE::NEW_DESERT_MAP);
			//pMeshData->Save(pMeshData->GetPath());
			pMapObject = new CGameObject;
			pMapObject = pMeshData->Instantiate();
			pMapObject->SetName(L"Table_01");
			pMapObject->FrustumCheck(false);

			z = (int)(3000.f / 60.f);
			bReverseQuad = ((z % 2) != 0);
			mapY = _terrain->GetHeight(3900.f, 2800.f, bReverseQuad);

			pMapObject->Transform()->SetLocalPos(Vector3(3900.f, mapY * 2 , 2800.f - (900.f * i)));
			pMapObject->Transform()->SetLocalScale(Vector3(200.f, 150.f, 130.f));//(1.0f, 1.0f, 1.0f));
			pMapObject->Transform()->SetLocalRot(Vector3(-XM_PI / 2, 0.f, 0.f));
			pMapObject->AddComponent(new CCollider);
			pMapObject->Collider()->SetColliderType(COLLIDER_TYPE::MESH, L"Table_01");
			pMapObject->Collider()->SetBoundingBox(BoundingBox(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingBoxExtents(pMapObject->Transform()->GetLocalScale())));
			pMapObject->Collider()->SetBoundingSphere(BoundingSphere(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingSphereRadius() * 60.f));
			m_pCurScene->AddGameObject(L"Map", pMapObject, false);
		}

		// 벤치1
		for (int i = 0; i < 2; ++i) {
			pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\DesertMap\\Bench_02.fbx", FBX_TYPE::NEW_DESERT_MAP);
			//pMeshData->Save(pMeshData->GetPath());
			pMapObject = new CGameObject;
			pMapObject = pMeshData->Instantiate();
			pMapObject->SetName(L"Bench_02");
			pMapObject->FrustumCheck(false);

			z = (int)(3000.f / 60.f);
			bReverseQuad = ((z % 2) != 0);
			mapY = _terrain->GetHeight(3900.f, 2950.f, bReverseQuad);

			pMapObject->Transform()->SetLocalPos(Vector3(3900.f, mapY * 2 , 2950.f - (300.f * i)));
			pMapObject->Transform()->SetLocalScale(Vector3(200.f, 150.f, 130.f));//(1.0f, 1.0f, 1.0f));
			pMapObject->Transform()->SetLocalRot(Vector3(-XM_PI / 2, 0.f, 0.f));
			pMapObject->AddComponent(new CCollider);
			pMapObject->Collider()->SetColliderType(COLLIDER_TYPE::MESH, L"Bench_02");
			pMapObject->Collider()->SetBoundingBox(BoundingBox(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingBoxExtents(pMapObject->Transform()->GetLocalScale())));
			pMapObject->Collider()->SetBoundingSphere(BoundingSphere(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingSphereRadius() * 60.f));
			m_pCurScene->AddGameObject(L"Map", pMapObject, false);
		}

		// 벤치2
		for (int i = 0; i < 2; ++i) {
			pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\DesertMap\\Bench_02.fbx", FBX_TYPE::NEW_DESERT_MAP);
			//pMeshData->Save(pMeshData->GetPath());
			pMapObject = new CGameObject;
			pMapObject = pMeshData->Instantiate();
			pMapObject->SetName(L"Bench_02");
			pMapObject->FrustumCheck(false);

			z = (int)(3000.f / 60.f);
			bReverseQuad = ((z % 2) != 0);
			mapY = _terrain->GetHeight(3900.f, 2050.f, bReverseQuad);

			pMapObject->Transform()->SetLocalPos(Vector3(3900.f, mapY * 2 , 2050.f - (300.f * i)));
			pMapObject->Transform()->SetLocalScale(Vector3(200.f, 150.f, 130.f));//(1.0f, 1.0f, 1.0f));
			pMapObject->Transform()->SetLocalRot(Vector3(-XM_PI / 2, 0.f, 0.f));
			pMapObject->AddComponent(new CCollider);
			pMapObject->Collider()->SetColliderType(COLLIDER_TYPE::MESH, L"Bench_02");
			pMapObject->Collider()->SetBoundingBox(BoundingBox(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingBoxExtents(pMapObject->Transform()->GetLocalScale())));
			pMapObject->Collider()->SetBoundingSphere(BoundingSphere(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingSphereRadius() * 60.f));
			m_pCurScene->AddGameObject(L"Map", pMapObject, false);
		}
	}

	{

	}

	{

	}

	{

	}
	//Wall
	{
		//왼쪽
		float wallX = 1400.f;
		for (int i = 0; i < 55; ++i) {
			pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\Desert\\Wall.fbx", FBX_TYPE::DESERT_MAP);

			pMapObject = new CGameObject;

			pMapObject = pMeshData->Instantiate();
			pMapObject->SetName(L"Wall");
			pMapObject->FrustumCheck(true);


			z = (int)(30.f / 60.f);
			bReverseQuad = ((z % 2) != 0);
			mapY = _terrain->GetHeight(30.f, (wallX * i) + wallX / 2, true);

			pMapObject->Transform()->SetLocalPos(Vector3(250.f, mapY * 2, (wallX * i) + wallX / 2));
			pMapObject->Transform()->SetLocalScale(Vector3(310.f, 200.f, 1000.f));//(1.0f, 1.0f, 1.0f));
			pMapObject->Transform()->SetLocalRot(Vector3(-XM_PI / 2, -XM_PI / 2, 0.f));
			/*pMapObject->AddComponent(new CCollider);
			pMapObject->Collider()->SetColliderType(COLLIDER_TYPE::MESH, L"Wall");
			pMapObject->Collider()->SetBoundingBox(BoundingBox(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingBoxExtents() * pMapObject->Transform()->GetLocalScale()));
			pMapObject->Collider()->SetBoundingSphere(BoundingSphere(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingSphereRadius() * 60.f));*/

			m_pCurScene->AddGameObject(L"Map", pMapObject, false);
		}

		//오른쪽
		for (int i = 0; i < 55; ++i) {
			pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\Desert\\Wall.fbx", FBX_TYPE::DESERT_MAP);

			pMapObject = new CGameObject;

			pMapObject = pMeshData->Instantiate();
			pMapObject->SetName(L"Wall");
			pMapObject->FrustumCheck(true);


			z = (int)(30.f / 60.f);
			bReverseQuad = ((z % 2) != 0);
			mapY = _terrain->GetHeight(30.f, (wallX * i) + wallX / 2, true);

			pMapObject->Transform()->SetLocalPos(Vector3(76500.f, mapY * 2, (wallX * i) + wallX / 2));
			pMapObject->Transform()->SetLocalScale(Vector3(310.f, 200.f, 1000.f));//(1.0f, 1.0f, 1.0f));
			pMapObject->Transform()->SetLocalRot(Vector3(-XM_PI / 2, XM_PI / 2, 0.f));
			/*pMapObject->AddComponent(new CCollider);
			pMapObject->Collider()->SetColliderType(COLLIDER_TYPE::MESH, L"Wall");
			pMapObject->Collider()->SetBoundingBox(BoundingBox(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingBoxExtents() * pMapObject->Transform()->GetLocalScale()));
			pMapObject->Collider()->SetBoundingSphere(BoundingSphere(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingSphereRadius() * 60.f));*/

			m_pCurScene->AddGameObject(L"Map", pMapObject, false);
		}

		// 위
		for (int i = 0; i < 55; ++i) {
			pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\Desert\\Wall.fbx", FBX_TYPE::DESERT_MAP);

			pMapObject = new CGameObject;

			pMapObject = pMeshData->Instantiate();
			pMapObject->SetName(L"Wall");
			pMapObject->FrustumCheck(true);


			z = (int)(30.f / 60.f);
			bReverseQuad = ((z % 2) != 0);
			mapY = _terrain->GetHeight((wallX * i) + wallX / 2, 76300.f, true);

			pMapObject->Transform()->SetLocalPos(Vector3((wallX * i) + wallX / 2, mapY * 2, 76300.f));
			pMapObject->Transform()->SetLocalScale(Vector3(310.f, 200.f, 1000.f));//(1.0f, 1.0f, 1.0f));
			pMapObject->Transform()->SetLocalRot(Vector3(-XM_PI / 2, 0.f, 0.f));
			/*pMapObject->AddComponent(new CCollider);
			pMapObject->Collider()->SetColliderType(COLLIDER_TYPE::MESH, L"Wall");
			pMapObject->Collider()->SetBoundingBox(BoundingBox(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingBoxExtents() * pMapObject->Transform()->GetLocalScale()));
			pMapObject->Collider()->SetBoundingSphere(BoundingSphere(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingSphereRadius() * 60.f));*/

			m_pCurScene->AddGameObject(L"Map", pMapObject, false);
		}

		// 아래
		for (int i = 0; i < 55; ++i) {
			pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\Desert\\Wall.fbx", FBX_TYPE::DESERT_MAP);

			pMapObject = new CGameObject;

			pMapObject = pMeshData->Instantiate();
			pMapObject->SetName(L"Wall");
			pMapObject->FrustumCheck(true);


			z = (int)(30.f / 60.f);
			bReverseQuad = ((z % 2) != 0);
			mapY = _terrain->GetHeight((wallX * i) + wallX / 2, 300.f, true);

			pMapObject->Transform()->SetLocalPos(Vector3((wallX * i) + wallX / 2, mapY * 2, 300.f));
			pMapObject->Transform()->SetLocalScale(Vector3(310.f, 200.f, 1000.f));//(1.0f, 1.0f, 1.0f));
			pMapObject->Transform()->SetLocalRot(Vector3(-XM_PI / 2, XM_PI, 0.f));
			/*pMapObject->AddComponent(new CCollider);
			pMapObject->Collider()->SetColliderType(COLLIDER_TYPE::MESH, L"Wall");
			pMapObject->Collider()->SetBoundingBox(BoundingBox(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingBoxExtents() * pMapObject->Transform()->GetLocalScale()));
			pMapObject->Collider()->SetBoundingSphere(BoundingSphere(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingSphereRadius() * 60.f));*/

			m_pCurScene->AddGameObject(L"Map", pMapObject, false);
		}
	}

}

void CSceneMgr::LoadRes()
{
	Ptr<CTexture> piBow = CResMgr::GetInst()->Load<CTexture>(L"BOW_IMG", L"Texture\\UI\\Items\\Weapons\\01_BOW.png");
	Ptr<CTexture> piSword = CResMgr::GetInst()->Load<CTexture>(L"SWORD_IMG", L"Texture\\UI\\Items\\Weapons\\02_Sword.png");
	Ptr<CTexture> piHealPotion = CResMgr::GetInst()->Load<CTexture>(L"HP_POTION_IMG", L"Texture\\UI\\Items\\Resources\\15_Heal_potion.png");
}

void CSceneMgr::CreateNpc(CTerrain* _terrain)
{

	// Npc_1
	Ptr<CMeshData> pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\NPC\\common_people_female_4@Idle.fbx", FBX_TYPE::NPC);
	//Ptr<CMeshData> pAniData = CResMgr::GetInst()->LoadFBX(L"FBX\\Animation\\itempack@spraying.fbx", FBX_TYPE::ANI);
	CGameObject* pNpcObject = nullptr;
	pNpcObject = pMeshData->Instantiate();
	pNpcObject->SetName(L"Npc_1");
	pNpcObject->FrustumCheck(false);
	pNpcObject->Transform()->SetLocalPos(Vector3(5500.f, _terrain->GetHeight(5500.f, 3200.f, true) * 2 /*240.f*/, 3200.f));
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
	pNpcObject->Transform()->SetLocalPos(Vector3(1600.f, _terrain->GetHeight(1600.f, 1500.f, true) * 2 /*240.f*/, 1500.f));
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
	pNpcObject->Transform()->SetLocalPos(Vector3(1600.f, _terrain->GetHeight(1600.f, 4200.f, true) * 2 /*240.f*/, 4200.f));
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

	LoadRes();
	Ptr<CTexture> pTex = CResMgr::GetInst()->Load<CTexture>(L"TestTex", L"Texture\\Health.png");
	Ptr<CTexture> pExplosionTex = CResMgr::GetInst()->Load<CTexture>(L"Explosion", L"Texture\\Explosion\\Explosion80.png");
	Ptr<CTexture> pBlackTex = CResMgr::GetInst()->Load<CTexture>(L"Black", L"Texture\\asd.png");
	Ptr<CTexture> pSky01 = CResMgr::GetInst()->Load<CTexture>(L"Sky01", L"Texture\\Skybox\\Sky01.png");
	Ptr<CTexture> pSky02 = CResMgr::GetInst()->Load<CTexture>(L"Sky02", L"Texture\\Skybox\\Sky02.jpg");
	// UI
	Ptr<CTexture> pUiHug = CResMgr::GetInst()->Load<CTexture>(L"UiHug", L"Texture\\hug.png");
	Ptr<CTexture> pUiTemper = CResMgr::GetInst()->Load<CTexture>(L"UiTemper", L"Texture\\temper.png");
	Ptr<CTexture> pColor = CResMgr::GetInst()->Load<CTexture>(L"Tile", L"Texture\\Tile\\TILE_03.tga");
	Ptr<CTexture> pNormal = CResMgr::GetInst()->Load<CTexture>(L"Tile_n", L"Texture\\Tile\\TILE_03_N.tga");

	Ptr<CTexture> pDiffuseTargetTex = CResMgr::GetInst()->FindRes<CTexture>(L"DiffuseTargetTex");
	Ptr<CTexture> pNormalTargetTex = CResMgr::GetInst()->FindRes<CTexture>(L"NormalTargetTex");
	Ptr<CTexture> pPositionTargetTex = CResMgr::GetInst()->FindRes<CTexture>(L"PositionTargetTex");

	//==========================
	// Conversation Box
	//==========================
	Ptr<CTexture> pUiBoard = CResMgr::GetInst()->Load<CTexture>(L"UiBoard", L"Texture\\UIboard.png");
	pUiBoard = CResMgr::GetInst()->Load<CTexture>(L"npc1_quest1(1)", L"Texture\\Quest\\npc1_quest1(1).png");
	pUiBoard = CResMgr::GetInst()->Load<CTexture>(L"npc1_quest1(2)", L"Texture\\Quest\\npc1_quest1(2).png");
	pUiBoard = CResMgr::GetInst()->Load<CTexture>(L"npc1_quest1(3)", L"Texture\\Quest\\npc1_quest1(3).png");
	pUiBoard = CResMgr::GetInst()->Load<CTexture>(L"npc1_quest2(1)", L"Texture\\Quest\\npc1_quest2(1).png");
	pUiBoard = CResMgr::GetInst()->Load<CTexture>(L"npc1_quest2(2)", L"Texture\\Quest\\npc1_quest2(2).png");
	pUiBoard = CResMgr::GetInst()->Load<CTexture>(L"npc1_quest2(3)", L"Texture\\Quest\\npc1_quest2(3).png");
	pUiBoard = CResMgr::GetInst()->Load<CTexture>(L"npc1_done", L"Texture\\Quest\\npc1_done.png");
	pUiBoard = CResMgr::GetInst()->Load<CTexture>(L"npc1_why", L"Texture\\Quest\\npc1_why.png");
	pUiBoard = CResMgr::GetInst()->Load<CTexture>(L"npc2_quest1(1)", L"Texture\\Quest\\npc2_quest1(1).png");
	pUiBoard = CResMgr::GetInst()->Load<CTexture>(L"npc2_quest1(2)", L"Texture\\Quest\\npc2_quest1(2).png");
	pUiBoard = CResMgr::GetInst()->Load<CTexture>(L"npc2_quest1(3)", L"Texture\\Quest\\npc2_quest1(3).png");
	pUiBoard = CResMgr::GetInst()->Load<CTexture>(L"npc2_quest2(1)", L"Texture\\Quest\\npc2_quest2(1).png");
	pUiBoard = CResMgr::GetInst()->Load<CTexture>(L"npc2_quest2(2)", L"Texture\\Quest\\npc2_quest2(2).png");
	pUiBoard = CResMgr::GetInst()->Load<CTexture>(L"npc2_quest2(3)", L"Texture\\Quest\\npc2_quest2(3).png");
	pUiBoard = CResMgr::GetInst()->Load<CTexture>(L"npc2_done", L"Texture\\Quest\\npc2_done.png");
	pUiBoard = CResMgr::GetInst()->Load<CTexture>(L"npc2_why", L"Texture\\Quest\\npc2_why.png");
	pUiBoard = CResMgr::GetInst()->Load<CTexture>(L"npc3_start", L"Texture\\Quest\\npc3_start.png");
	
	//===========================
	// number texture
	//==========================
	Ptr<CTexture> pNumber = CResMgr::GetInst()->Load<CTexture>(L"0", L"Texture\\Number\\0.png");
	pNumber = CResMgr::GetInst()->Load<CTexture>(L"1", L"Texture\\Number\\1.png");
	pNumber = CResMgr::GetInst()->Load<CTexture>(L"2", L"Texture\\Number\\2.png");
	pNumber = CResMgr::GetInst()->Load<CTexture>(L"3", L"Texture\\Number\\3.png");
	pNumber = CResMgr::GetInst()->Load<CTexture>(L"4", L"Texture\\Number\\4.png");
	pNumber = CResMgr::GetInst()->Load<CTexture>(L"5", L"Texture\\Number\\5.png");
	pNumber = CResMgr::GetInst()->Load<CTexture>(L"6", L"Texture\\Number\\6.png");
	pNumber = CResMgr::GetInst()->Load<CTexture>(L"7", L"Texture\\Number\\7.png");
	pNumber = CResMgr::GetInst()->Load<CTexture>(L"8", L"Texture\\Number\\8.png");
	pNumber = CResMgr::GetInst()->Load<CTexture>(L"9", L"Texture\\Number\\9.png");

	//===========================
	// number texture
	//==========================
	Ptr<CTexture> pButtonTex = CResMgr::GetInst()->Load<CTexture>(L"BRANCH", L"Texture\\ItemButton\\BranchTex.png");
	pButtonTex = CResMgr::GetInst()->Load<CTexture>(L"APPLE", L"Texture\\ItemButton\\AppleTex.png");
	pButtonTex = CResMgr::GetInst()->Load<CTexture>(L"BOTTLE_STAMINA", L"Texture\\ItemButton\\BottleStaminaTex.png");
	pButtonTex = CResMgr::GetInst()->Load<CTexture>(L"BOTTLE_DASH", L"Texture\\ItemButton\\BottleDashTex.png");
	pButtonTex = CResMgr::GetInst()->Load<CTexture>(L"STEAK", L"Texture\\ItemButton\\MeatTex.png");
	pButtonTex = CResMgr::GetInst()->Load<CTexture>(L"MONEYBAG", L"Texture\\ItemButton\\MoneyTex.png");
	pButtonTex = CResMgr::GetInst()->Load<CTexture>(L"CARROT", L"Texture\\ItemButton\\CarrotTex.png");
	pButtonTex = CResMgr::GetInst()->Load<CTexture>(L"EMPTY", L"Texture\\ItemButton\\EmptyTex.png");
	pButtonTex = CResMgr::GetInst()->Load<CTexture>(L"SWORD", L"Texture\\ItemButton\\SwordTex.png");

	//==========================
	// UAV 용 Texture 생성
	//==========================
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
	m_pCurScene->GetLayer(6)->SetName(L"Item");
	m_pCurScene->GetLayer(7)->SetName(L"Terrain");	// 카메라 순서 때문에 오류나서 일단 터레인 레이어 추가함
	m_pCurScene->GetLayer(30)->SetName(L"UI");
	m_pCurScene->GetLayer(31)->SetName(L"PUI");

	CGameObject* pObject = nullptr;
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
	pPlayerObj->Collider()->SetBoundingSphere(BoundingSphere(pPlayerObj->Transform()->GetLocalPos(), pPlayerObj->MeshRender()->GetMesh()->GetBoundingSphereRadius() / 2.f));
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
	// PIck_UP
	pMeshDataKey = CResMgr::GetInst()->LoadFBX(L"FBX\\Player\\PlayerMale@PickUp.fbx", FBX_TYPE::PLAYER);
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
	pSword->Transform()->SetLocalScale(Vector3(1.f, 1.f, 1.f));//(1.0f, 1.0f, 1.0f));
	//pSword->Transform()->SetLocalRot(Vector3(0.f, XM_PI, 0.f));
	pSword->AddComponent(new CCollider);
	pSword->Collider()->SetColliderType(COLLIDER_TYPE::MESH, L"PlayerMale_Weapon_Sword");
	pSword->Collider()->SetBoundingBox(BoundingBox(pSword->Transform()->GetLocalPos(), pSword->MeshRender()->GetMesh()->GetBoundingBoxExtents()));
	pSword->Collider()->SetBoundingSphere(BoundingSphere(pSword->Transform()->GetLocalPos(), pSword->MeshRender()->GetMesh()->GetBoundingSphereRadius()));
	pSword->MeshRender()->SetDynamicShadow(true);
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
	//	item 객체 생성
	// ==================
	//CGameObject* pItem = new CGameObject;
	//pItem->SetName("")

	CItemMgr::GetInst()->SetItemData(CResMgr::GetInst()->LoadFBX(L"FBX\\Item\\Bottle_03.fbx", FBX_TYPE::ITEM)); // 물약1
	CItemMgr::GetInst()->SetItemData(CResMgr::GetInst()->LoadFBX(L"FBX\\Item\\Bottle_04.fbx", FBX_TYPE::ITEM));	// 물약2
	CItemMgr::GetInst()->SetItemData(CResMgr::GetInst()->LoadFBX(L"FBX\\Item\\Steak_02.fbx", FBX_TYPE::ITEM));	// 고기
	CItemMgr::GetInst()->SetItemData(CResMgr::GetInst()->LoadFBX(L"FBX\\Item\\MoneyBag.fbx", FBX_TYPE::ITEM));	// 돈
	CItemMgr::GetInst()->SetItemData(CResMgr::GetInst()->LoadFBX(L"FBX\\Item\\Carrot.fbx", FBX_TYPE::ITEM));	// 당근
	
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
	pUICam->Camera()->SetLayerCheck(31, true);
	m_pCurScene->FindLayer(L"Default")->AddGameObject(pUICam);
	pUICam->Camera()->SetWidth(CRenderMgr::GetInst()->GetResolution().fWidth);
	pUICam->Camera()->SetHeight(CRenderMgr::GetInst()->GetResolution().fHeight);
	CreateTargetUI(pUICam);
	
	//	-----------------------
	//	item UI, 상속되는 버튼들
	//	-----------------------
	pObject = new CGameObject;
	Vector3 vScale = Vector3(600.f, 800.f, 1.f);
	pObject->SetName(L"Item UI Object");
	pObject->FrustumCheck(false);
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CMeshRender);
	pObject->AddComponent(new CStaticUI);
	pObject->StaticUI()->init(UI_TYPE::PRIVATE_ITEM_UI);
	pObject->StaticUI()->CreatePickingObj();
	// 투영행렬 statiUI 컴포넌트에 등록 (ORTHOGRAPHIC 카메라 정보 필요)
	pObject->StaticUI()->SetCameraProj(pUICam->Camera());
	//	플레이어 스크립트(오브젝트)에 StaticUI 귀속
	pPlayerObj->GetScript<CPlayerScript>()->SetUIObj(pObject);
	// Transform 설정
	tResolution res = CRenderMgr::GetInst()->GetResolution();
	pObject->Transform()->SetLocalPos(Vector3(100.f, 80.f, 1.f));
	pObject->Transform()->SetLocalScale(vScale);
	// MeshRender 설정	
	pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	Ptr<CMaterial> pMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"UITexMtrl");
	pObject->MeshRender()->SetMaterial(pMtrl->Clone());
	Ptr<CTexture> itemUI = pObject->StaticUI()->m_pFrame;
	float fUI = 0.5f;
	pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, itemUI.GetPointer());
	pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::FLOAT_0, &fUI);
	// AddGameObject
	m_pCurScene->FindLayer(L"UI")->AddGameObject(pObject);
	
	//	Static Ui에 상속된 버튼들 Scene에 Obj로 추가
	vScale = Vector3(80.f, 120.f, 1.f);
	Vector3 vObjectPos = pObject->Transform()->GetLocalPos();
	Vector3 vObjectScale = pObject->Transform()->GetLocalScale(); 
	float	fEmptyY = (vObjectScale.y - 100.f - (vScale.y * 4.f)) / 5.f;
	float	fEmptyX = (vObjectScale.x - (vScale.x * 4.f)) / 5.f;
	for (int i = 0; i < pObject->StaticUI()->m_vecButton.size(); ++i)
	{
		Ptr<CTexture> itemUI = CResMgr::GetInst()->FindRes<CTexture>(L"ItemUiTex");
		CGameObject* pButtonObj = new CGameObject;
		pButtonObj->SetName(L"Button Object");
		pButtonObj->FrustumCheck(false);// 절두체 컬링 사용하지 않음
		pButtonObj->AddComponent(new CTransform);
		pButtonObj->AddComponent(new CMeshRender);
		pButtonObj->AddComponent(new CCollider);
		pButtonObj->Collider()->SetColliderType(COLLIDER_TYPE::RECT);
		//	버튼 Script 설정
		pButtonObj->AddComponent(pObject->StaticUI()->m_vecButton[i]);
		pObject->StaticUI()->m_vecButton[i]->SetParent(pObject->StaticUI());
		// Transform 설정
		tResolution res = CRenderMgr::GetInst()->GetResolution();
		Vector3 result = Vector3(vObjectPos.x - (vObjectScale.x / 2.f) + (vScale.x / 2.f + fEmptyX) + ((vScale.x + fEmptyX) * (i % 4))
			, vObjectPos.y + (vObjectScale.y / 2.f) - (vScale.y / 2.f + fEmptyY) - ((vScale.y + fEmptyY) * (i / 4))
			, 1.f);
		pButtonObj->Transform()->SetLocalPos(result);
		pButtonObj->Transform()->SetLocalScale(vScale);
		// MeshRender 설정
		pButtonObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
		Ptr<CMaterial> pMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"TexMtrl");
		pButtonObj->MeshRender()->SetMaterial(pMtrl->Clone());
		pButtonObj->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pObject->StaticUI()->m_vecButton[i]->GetImage().GetPointer());
		// AddGameObject
		m_pCurScene->FindLayer(L"UI")->AddGameObject(pButtonObj);

		pObject->StaticUI()->m_vecButton[i]->init();
	}

	////	상점 UI
	//pObject = new CGameObject;
	//vScale = Vector3(400.f, 600.f, 1.f);
	//pObject->SetName(L"Shop UI Object ");
	//pObject->FrustumCheck(false);
	//pObject->AddComponent(new CTransform);
	//pObject->AddComponent(new CMeshRender);
	//pObject->AddComponent(new CStaticUI);
	//pObject->StaticUI()->init(UI_TYPE::PUBLIC_SHOP_UI);
	//// 투영행렬 statiUI 컴포넌트에 등록 (ORTHOGRAPHIC 카메라 정보 필요)
	//pObject->StaticUI()->SetCameraProj(_camObj->Camera());
	//// Transform 설정
	//tResolution res = CRenderMgr::GetInst()->GetResolution();
	//pObject->Transform()->SetLocalPos(Vector3(-(res.fWidth / 2.f) + (vScale.x / 2.f) + (3 * vScale.x)
	//	, (res.fHeight / 2.f) - (vScale.y / 2.f)
	//	, 1.f));
	//pObject->Transform()->SetLocalScale(vScale);
	//// MeshRender 설정
	//pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	//Ptr<CMaterial> pMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"TestMtrl");
	//pObject->MeshRender()->SetMaterial(pMtrl->Clone());
	//Ptr<CTexture> itemUI = pObject->StaticUI()->m_pFrame;
	//pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, itemUI.GetPointer());
	//// AddGameObject
	//m_pCurScene->FindLayer(L"UI")->AddGameObject(pObject);
	////	Static Ui에 상속된 버튼들 Scene에 Obj로 추가
	//for (int i = 0; i < pObject->StaticUI()->m_vecButton.size(); ++i)
	//{
	//	vScale = Vector3(80.f, 120.f, 1.f);
	//	Ptr<CTexture> itemUI = CResMgr::GetInst()->FindRes<CTexture>(L"ItemUiTex");
	//	CGameObject* pButtonObj = new CGameObject;
	//	pButtonObj->SetName(L"Button Object");
	//	pButtonObj->FrustumCheck(false);	// 절두체 컬링 사용하지 않음
	//	pButtonObj->AddComponent(new CTransform);
	//	pButtonObj->AddComponent(new CMeshRender);
	//	pButtonObj->AddComponent(new CCollider);
	//	pButtonObj->Collider()->SetColliderType(COLLIDER_TYPE::RECT);
	//	//	버튼 Script 설정
	//	pButtonObj->AddComponent(pObject->StaticUI()->m_vecButton[i]);
	//	pObject->StaticUI()->m_vecButton[i]->SetParent(pObject->StaticUI());
	//	// Transform 설정
	//	tResolution res = CRenderMgr::GetInst()->GetResolution();
	//	pButtonObj->Transform()->SetLocalPos(Vector3(-(res.fWidth / 2.f) + (vScale.x / 2.f) + (i * vScale.x) + 200.f
	//		, (res.fHeight / 2.f) - (vScale.y / 2.f)
	//		, 1.f));
	//	pButtonObj->Transform()->SetLocalScale(vScale);
	//	// MeshRender 설정
	//	pButtonObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	//	Ptr<CMaterial> pMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"TestMtrl");
	//	pButtonObj->MeshRender()->SetMaterial(pMtrl->Clone());
	//	pButtonObj->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pObject->StaticUI()->m_vecButton[i]->GetImage().GetPointer());
	//	// AddGameObject
	//	m_pCurScene->FindLayer(L"UI")->AddGameObject(pButtonObj);
	//}


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


	//	테스트 텍스쳐
	pObject = new CGameObject;
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CMeshRender);
	// Transform 설정
	pObject->Transform()->SetLocalPos(Vector3(0.f, 100.f, 300.f));
	pObject->Transform()->SetLocalScale(Vector3(2000.f, 2000.f, 1.f));
	pObject->Transform()->SetLocalRot(Vector3(XM_PI / 2.f, 0.f, 0.f));
	// MeshRender 설정
	pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std3DMtrl"));
	pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pColor.GetPointer());
	pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_1, pNormal.GetPointer());
	pObject->MeshRender()->SetDynamicShadow(false);
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
	//m_pCurScene->FindLayer(L"Default")->AddGameObject(pObject);


	// Terrain
	CGameObject* pTerrainObject = new CGameObject;
	pTerrainObject->SetName(L"Terrain");
	pTerrainObject->AddComponent(new CTransform);
	pTerrainObject->AddComponent(new CMeshRender);
	pTerrainObject->AddComponent(new CTerrain);
	pTerrainObject->FrustumCheck(false);
	pTerrainObject->Transform()->SetLocalPos(Vector3(0.f, 470.f, 0.f));
	pTerrainObject->Transform()->SetLocalScale(Vector3(300.f , 6000.f, 300.f)); // 2배함
	pTerrainObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"TerrainMtrl"));
	pTerrainObject->Terrain()->init();
	m_pCurScene->FindLayer(L"Terrain")->AddGameObject(pTerrainObject);
	pPlayerObj->GetScript<CPlayerScript>()->SetTerrain(pTerrainObject->Terrain());
	g_netMgr.SetObj(pPlayerObj);
	
	//CreateMap(pPlayerObj->GetScript<CPlayerScript>()->GetTerrain());
	CreateNpc(pPlayerObj->GetScript<CPlayerScript>()->GetTerrain());
	CreateNewMap(pPlayerObj->GetScript<CPlayerScript>()->GetTerrain());
	CItemMgr::GetInst()->SetTerrain(pPlayerObj->GetScript<CPlayerScript>()->GetTerrain());



	// ====================
	// Compute Shader Test
	// ====================
	int i = 1;
	Ptr<CMaterial> pCSMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"CSTestMtrl");
	pCSMtrl->SetData(SHADER_PARAM::INT_0, &i);
	CDevice::GetInst()->SetUAVToRegister_CS(pTestUAVTexture.GetPointer(), UAV_REGISTER::u0);
	pCSMtrl->Dispatch(1, 1024, 1); // --> 컴퓨트 쉐이더 수행	


	//CCollisionMgr::GetInst()->CheckCollisionLayer(L"Player", L"Monster");
	//CCollisionMgr::GetInst()->CheckCollisionLayer(L"Player", L"Map");
	//CCollisionMgr::GetInst()->CheckCollisionLayer(L"Player", L"Npc");
	CCollisionMgr::GetInst()->CheckCollisionLayer(L"Bullet", L"Monster");
	CCollisionMgr::GetInst()->CheckCollisionLayer(L"Bullet", L"Map");
	//CCollisionMgr::GetInst()->CheckCollisionLayer(L"Bullet", L"Item");
	CCollisionMgr::GetInst()->CheckCollisionLayer(L"Item", L"Player");
	//CCollisionMgr::GetInst()->CheckCollisionLayer(L"UI", L"PUI");
	

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
	CItemMgr::GetInst()->update();
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
