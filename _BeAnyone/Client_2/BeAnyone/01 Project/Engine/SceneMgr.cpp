#include "pch.h"
#include "SceneMgr.h"
#include "Scene.h"
#include "Layer.h"
#include "GameObject.h"
#include "ResMgr.h"
#include "Shader.h"
#include "Mesh.h"
#include "Texture.h"
#include "Transform.h"
#include "TimeMgr.h"
#include "KeyMgr.h"
#include "Camera.h"
#include "Device.h"
#include "Core.h"
#include "MeshRenderer.h"
#include "ToolCamScript.h"
#include "RenderMgr.h"
#include "Terrain.h"
#include "Light3D.h"
#include "CollisionMgr.h"
#include "EventMgr.h"
#include "PlayerScript.h"
#include "GridScript.h"
//#include "MonsterScript.h"

CScene* CSceneMgr::GetCurScene() {
	return m_pCurScene;
}

void CSceneMgr::ChangeScene( CScene* _pNextScene ) {
	SAFE_DELETE( m_pCurScene );
	m_pCurScene = _pNextScene;
}

CSceneMgr::CSceneMgr()
	: m_pCurScene( nullptr ) {
}

CSceneMgr::~CSceneMgr() {
	SAFE_DELETE( m_pCurScene );
}

void CSceneMgr::CreateTargetUI()
{
	Vec3 vScale(150.f, 150.f, 1.f);

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

		pObject->Transform()->SetLocalPos(Vec3(-(res.fWidth / 2.f) + (vScale.x / 2.f) + (i * vScale.x)
			, (res.fHeight / 2.f) - (vScale.y / 2.f)
			, 1.f));

		pObject->Transform()->SetLocalScale(vScale);

		// MeshRender 설정
		pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
		Ptr<CMaterial> pMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"TexMtrl");
		pObject->MeshRender()->SetMaterial(pMtrl->Clone());
		pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, arrTex[i].GetPointer());

		// AddGameObject
		m_pCurScene->FindLayer(L"UI")->AddGameObject(pObject);
		if (i == 3)
			continue;
	}
}

void CSceneMgr::init() {

	// Texture 로드
	Ptr<CTexture> pTex = CResMgr::GetInst()->Load<CTexture>( L"TestTex", L"Texture\\cookie.png" );
	Ptr<CTexture> pColor = CResMgr::GetInst()->Load<CTexture>( L"Tile", L"Texture\\Tile\\TILE_03.tga" );
	Ptr<CTexture> pNormal = CResMgr::GetInst()->Load<CTexture>( L"Tile_n", L"Texture\\Tile\\TILE_03_N.tga" );
	Ptr<CTexture> pSky01 = CResMgr::GetInst()->Load<CTexture>(L"Sky01", L"Texture\\Skybox\\Sky01.png");
	Ptr<CTexture> pSky02 = CResMgr::GetInst()->Load<CTexture>(L"Sky02", L"Texture\\Skybox\\Sky02.jpg");
	Ptr<CTexture> pDiffuseTargetTex = CResMgr::GetInst()->FindRes<CTexture>( L"DiffuseTargetTex" );
	Ptr<CTexture> pNormalTargetTex = CResMgr::GetInst()->FindRes<CTexture>( L"NormalTargetTex" );
	Ptr<CTexture> pPositionTargetTex = CResMgr::GetInst()->FindRes<CTexture>( L"PositionTargetTex" );
	Ptr<CMaterial> pPM = CResMgr::GetInst()->FindRes<CMaterial>(L"MergeLightMtrl");

	//static Ptr<CTexture> g_pHeightmapImage = CResMgr::GetInst()->Load<CTexture>( L"HeightMap", L"Texture\\Terrain\\HeightMap_01.png" );

	pPM->SetData(SHADER_PARAM::TEX_3, pSky01.GetPointer());
	pPM = CResMgr::GetInst()->FindRes<CMaterial>(L"PointLightMtrl");
	pPM->SetData(SHADER_PARAM::TEX_2, pSky01.GetPointer());


	// Test Scene 생성
	m_pCurScene = new CScene;
	m_pCurScene->SetName( L"Test Scene" );
	// Layer 이름 지정
	m_pCurScene->GetLayer(0)->SetName(L"Default");
	m_pCurScene->GetLayer(1)->SetName(L"Player");
	/* 0408 MRT 적용시 Layer index값 바꾸면 이상해짐 해결 필요*/
	m_pCurScene->GetLayer(30)->SetName(L"UI");
	m_pCurScene->GetLayer(31)->SetName(L"Tool");


	CGameObject* pObject = nullptr;
	CGameObject* pTerrainObject = new CGameObject;


	// Player 오브젝트 생성
	CGameObject* pPlayerObj = new CGameObject;
	pPlayerObj->SetName(L"Player Object");
	pPlayerObj->AddComponent(new CTransform);
	pPlayerObj->AddComponent(new CMeshRender);
	// Collider 컴포넌트 추가
	pPlayerObj->AddComponent(new CCollider);
	pPlayerObj->Collider()->SetColliderType(COLLIDER_TYPE::SPHERE);
	// Script 설정
	pPlayerObj->AddComponent(new CPlayerScript);
	// Transform 설정
	pPlayerObj->Transform()->SetLocalPos(Vec3(500.f, 1000.f, 500.f));
	pPlayerObj->Transform()->SetLocalScale(Vec3(3.f, 6.f, 3.f));
	pPlayerObj->Transform()->SetLocalRot(Vec3(/*XM_PI / 2.f*/0.f, XM_PI, 0.f));
	// MeshRender 설정
	pPlayerObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh"));
	pPlayerObj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std3DMtrl"));
	pPlayerObj->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pColor.GetPointer());
	pPlayerObj->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_1, pNormal.GetPointer());
	// AddGameObject
	m_pCurScene->FindLayer(L"Player")->AddGameObject(pPlayerObj);


	// Camera Object 생성
	CGameObject* pMainCam = new CGameObject;
	pMainCam->SetName( L"MainCam" );
	pMainCam->AddComponent( new CTransform );
	pMainCam->AddComponent( new CCamera );
	pMainCam->AddComponent( new CToolCamScript );
	pMainCam->Camera()->SetProjType( PROJ_TYPE::PERSPECTIVE );
	pMainCam->Camera()->SetFar( 1000000.f );	//	1,000,000.f
	//pMainCam->Camera()->SetFOV();
	pMainCam->Camera()->SetLayerAllCheck();
	pMainCam->Camera()->SetLayerCheck(30, false);
	CToolCamScript* camScript = pMainCam->GetScript<CToolCamScript>();
	camScript->SetPlayer(pPlayerObj);
	m_pCurScene->GetLayer( 0 )->AddGameObject( pMainCam );

	 
	// UI Camera Obj
	CGameObject* pUICam = new CGameObject;
	pUICam->SetName(L"MainCam");
	pUICam->AddComponent(new CTransform);
	pUICam->AddComponent(new CCamera);
	pUICam->Camera()->SetProjType(PROJ_TYPE::ORTHGRAPHIC);
	pUICam->Camera()->SetFar(100.f);
	pUICam->Camera()->SetLayerCheck(30, true);
	m_pCurScene->FindLayer(L"UI")->AddGameObject(pUICam);
	CreateTargetUI();
	

	// 3D Light Object
	pObject = new CGameObject;
	pObject->AddComponent( new CTransform );
	pObject->AddComponent( new CLight3D );

	pObject->Light3D()->SetLightPos( Vec3( 0.f, 1000.f, 1000.f ) );
	pObject->Light3D()->SetLightType( LIGHT_TYPE::DIR );
	pObject->Light3D()->SetDiffuseColor( Vec3( 1.f, 1.f, 1.f ) );
	pObject->Light3D()->SetSpecular( Vec3( 0.3f, 0.3f, 0.3f ) );
	pObject->Light3D()->SetAmbient( Vec3( 0.0f, 0.0f, 0.0f ) );
	pObject->Light3D()->SetLightDir( Vec3( 1.f, -1.f, 1.f ) );
	pObject->Light3D()->SetLightRange( 5000.f );
	pObject->Transform()->SetLocalPos( Vec3( -1000.f, 1000.f, -1000.f ) );
	m_pCurScene->GetLayer( 0 )->AddGameObject( pObject );
	

	// Terrain
	pTerrainObject->SetName(L"Terrain");
	pTerrainObject->AddComponent(new CTransform);
	pTerrainObject->AddComponent(new CMeshRender);
	pTerrainObject->AddComponent(new CTerrain);
	pTerrainObject->FrustumCheck(false);
	pTerrainObject->Transform()->SetLocalPos(Vec3(0.f, 10.f, 0.f));
	pTerrainObject->Transform()->SetLocalScale(Vec3(100.f, 300.f, 100.f));
	pTerrainObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"TerrainMtrl"));
	pTerrainObject->Terrain()->init();
	m_pCurScene->FindLayer(L"Default")->AddGameObject(pTerrainObject);

	pPlayerObj->GetScript<CPlayerScript>()->SetTerrain(pTerrainObject->Terrain());
	pPlayerObj->GetScript<CPlayerScript>();

	//static CGameObject* g_pTerrainObj = pTerrainObject;


	// temp 오브젝트 생성
	pObject = new CGameObject;
	pObject->AddComponent( new CTransform );
	pObject->AddComponent( new CMeshRender );
	// Transform 설정
	pObject->Transform()->SetLocalPos( Vec3( 300.f, 400.f, 700.f ) );
	pObject->Transform()->SetLocalScale( Vec3( 150.f, 150.f, 150.f ) );
	// MeshRender 설정
	pObject->MeshRender()->SetMesh( CResMgr::GetInst()->FindRes<CMesh>( L"SphereMesh" ) );
	pObject->MeshRender()->SetMaterial( CResMgr::GetInst()->FindRes<CMaterial>( L"Std3DMtrl" ) );
	pObject->MeshRender()->GetSharedMaterial()->SetData( SHADER_PARAM::TEX_0, pTex.GetPointer() );
	pObject->MeshRender()->GetSharedMaterial()->SetData( SHADER_PARAM::TEX_1, pNormal.GetPointer() );
	// AddGameObject
	m_pCurScene->GetLayer( 0 )->AddGameObject( pObject );


	// Skybox 오브젝트 생성
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


	// Grid 오브젝트 생성
	// ====================
	pObject = new CGameObject;
	pObject->SetName( L"Grid" );
	pObject->FrustumCheck( false );
	pObject->AddComponent( new CTransform );
	pObject->AddComponent( new CMeshRender );
	pObject->AddComponent( new CGridScript );

	// Transform 설정
	pObject->Transform()->SetLocalScale( Vec3( 100000.f, 100000.f, 1.f ) );
	pObject->Transform()->SetLocalRot( Vec3( XM_PI / 2.f, 0.f, 0.f ) );
	// MeshRender 설정
	pObject->MeshRender()->SetMesh( CResMgr::GetInst()->FindRes<CMesh>( L"RectMesh" ) );
	pObject->MeshRender()->SetMaterial( CResMgr::GetInst()->FindRes<CMaterial>( L"GridMtrl" ) );
	// Script 설정	
	pObject->GetScript<CGridScript>()->SetToolCamera( pMainCam );
	pObject->GetScript<CGridScript>()->SetGridColor( Vec3( 0.8f, 0.2f, 0.2f ) );

	// AddGameObject
	m_pCurScene->FindLayer(L"Default")->AddGameObject( pObject );

	m_pCurScene->awake();
	m_pCurScene->start();

}

void CSceneMgr::update() {
	m_pCurScene->update();
	m_pCurScene->lateupdate();

	// rendermgr 카메라 초기화
	CRenderMgr::GetInst()->ClearCamera();

	m_pCurScene->finalupdate();

	// 충돌 처리
	//CCollisionMgr::GetInst()->update();
}

void CSceneMgr::update_tool() {
	// rendermgr 카메라 초기화
	CRenderMgr::GetInst()->ClearCamera();
	m_pCurScene->finalupdate();
}

void CSceneMgr::FindGameObjectByTag( const wstring& _strTag, vector<CGameObject*>& _vecFindObj ) {
	for (int i = 0; i < MAX_LAYER; ++i)
	{
		const vector<CGameObject*>& vecObject = m_pCurScene->GetLayer( i )->GetObjects();
		for (size_t j = 0; j < vecObject.size(); ++j)
		{
			if (_strTag == vecObject[j]->GetName())
			{
				_vecFindObj.push_back( vecObject[j] );
			}
		}
	}
}

bool Compare( CGameObject* _pLeft, CGameObject* _pRight ) {
	return (_pLeft->Transform()->GetWorldPos().z < _pRight->Transform()->GetWorldPos().z);
}


