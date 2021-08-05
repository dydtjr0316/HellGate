#include "pch.h"
#include "TreeScript.h"
#include <random>
//#include <chrono>
//
//auto current = chrono::system_clock::now();
//auto duration = current.time_since_epoch();
//auto millis = chrono::duration_cast<chrono::milliseconds>(duration).count();
//
//mt19937_64 genMT(millis);

default_random_engine dreItem;
uniform_int_distribution<int> uidItem(0, 4);

uniform_int_distribution<int> uidStump(0, 1);

CTreeScript::CTreeScript()
	: CScript((UINT)COMPONENT_TYPE::SCRIPT) //CScript((UINT)SCRIPT_TYPE::MONSTERSCRIPT),
	, m_bIsAttack(false)
	, m_iAttackNum(0)
{
	// m_pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\Player\\PlayerMale@nWalk_F.fbx", FBX_TYPE::PLAYER);
	
	MakeItem();
}

CTreeScript::~CTreeScript()
{
}

void CTreeScript::update()
{
	if (m_bIsAttack == true)
		DestroyTree();
}

void CTreeScript::OnCollisionEnter(CCollider* _pOther)
{
	cout << "Tree collider" << endl;

	if (L"Attack Object" == _pOther->GetObj()->GetName()) {
		m_bIsAttack = true;
	}
}

void CTreeScript::OnCollsion(CCollider* _pOther)
{
	cout << "Tree collider aaaaa" << endl;
}


void CTreeScript::OnCollisionExit(CCollider* _pOther)
{
	//cout << "나감" << endl;
}

void CTreeScript::DestroyTree()
{
	Vector3 vTreePos = GetObj()->Transform()->GetLocalPos();
	
	// item1
	Vector3 vItem1Pos = m_pItem1->Transform()->GetLocalPos();
	Vector3 vItem1Rot = m_pItem1->Transform()->GetLocalRot();
	Vector3 vItem1FromtDir = m_pItem1->Transform()->GetWorldDir(DIR_TYPE::FRONT);
	Vector3 vItem1UpDir = m_pItem1->Transform()->GetWorldDir(DIR_TYPE::UP);

	// item2
	Vector3 vItem2Pos = m_pItem2->Transform()->GetLocalPos();
	Vector3 vItem2Rot = m_pItem2->Transform()->GetLocalRot();
	Vector3 vItem2FromtDir = m_pItem2->Transform()->GetWorldDir(DIR_TYPE::FRONT);
	Vector3 vItem2UpDir = m_pItem2->Transform()->GetWorldDir(DIR_TYPE::UP);

	// item3
	Vector3 vItem3Pos = m_pItem3->Transform()->GetLocalPos();
	Vector3 vItem3Rot = m_pItem3->Transform()->GetLocalRot();
	Vector3 vItem3FromtDir = m_pItem3->Transform()->GetWorldDir(DIR_TYPE::FRONT);
	Vector3 vItem3UpDir = m_pItem3->Transform()->GetWorldDir(DIR_TYPE::UP);

	if (m_bFirst == true) {
		vItem1Pos = vTreePos;
		vItem2Pos = vTreePos;
		vItem3Pos = vTreePos;
		
		// 회전
		vItem1Rot.y = XM_PI / 2;
		vItem2Rot.y = XM_PI / 8;
		vItem3Rot.y = XM_PI;
	
		m_pItem1->Transform()->SetLocalRot(vItem1Rot);
		m_pItem2->Transform()->SetLocalRot(vItem2Rot);
		m_pItem3->Transform()->SetLocalRot(vItem3Rot);

		int randNum = uidItem(dreItem);
		m_pItem1->MeshRender()->SetMesh(m_pItemMeshData[randNum]->GetMesh());
		m_pItem1->MeshRender()->SetMaterial(m_pItemMeshData[randNum]->GetMtrl());
		randNum = uidItem(dreItem);
		m_pItem2->MeshRender()->SetMesh(m_pItemMeshData[randNum]->GetMesh());
		m_pItem2->MeshRender()->SetMaterial(m_pItemMeshData[randNum]->GetMtrl());
		randNum = uidItem(dreItem);
		m_pItem3->MeshRender()->SetMesh(m_pItemMeshData[randNum]->GetMesh());
		m_pItem3->MeshRender()->SetMaterial(m_pItemMeshData[randNum]->GetMtrl());

		randNum = uidStump(dreItem);
		GetObj()->MeshRender()->SetMesh(m_pStumpMeshData[randNum]->GetMesh());
		GetObj()->MeshRender()->SetMaterial(m_pStumpMeshData[randNum]->GetMtrl());

		m_bFirst = false;
		m_bisUp = true;
	}

	// up
	if (vItem1Pos.y <= 2850.f && m_bisUp == true) {
		// m_iTemTime += DT;
		vItem1Pos += 60.f * DT * vItem1FromtDir;	// item1
		vItem1Pos += 50.f * DT * vItem1UpDir;
		vItem2Pos += 60.f * DT * vItem2FromtDir;	// item2
		vItem2Pos += 50.f * DT * vItem2UpDir;
		vItem3Pos += 60.f * DT * vItem3FromtDir;	// item3
		vItem3Pos += 50.f * DT * vItem3UpDir;
	}
	else {
		m_bisUp = false;
		m_bisDown = true;
	}

	// down
	if (vItem1Pos.y >= 2768.f && m_bisDown == true) {
		vItem1Pos += 60.f * DT * vItem1FromtDir;	// item1
		vItem1Pos += 50.f * DT * -vItem1UpDir;
		vItem2Pos += 60.f * DT * vItem2FromtDir;	// item2
		vItem2Pos += 50.f * DT * -vItem2UpDir;
		vItem3Pos += 60.f * DT * vItem3FromtDir;	// item3
		vItem3Pos += 50.f * DT * -vItem3UpDir;
	}
	else {
		m_bisDown = false;
	}

	m_pItem1->Transform()->SetLocalPos(vItem1Pos);
	m_pItem2->Transform()->SetLocalPos(vItem2Pos);
	m_pItem3->Transform()->SetLocalPos(vItem3Pos);

}

void CTreeScript::MakeItem()
{
	// item1
	m_pItem1 = new CGameObject;
	m_pItem1->SetName(L"Item1");
	m_pItem1->FrustumCheck(true);
	m_pItem1->AddComponent(new CTransform);
	m_pItem1->AddComponent(new CMeshRender);
	m_pItem1->Transform()->SetLocalPos(Vector3(0.f, 0.f, 0.f));
	m_pItem1->Transform()->SetLocalScale(Vector3(200.f, 200.f, 200.f));
	m_pItem1->Transform()->SetLocalRot(Vector3(0.f, 0.f, 0.f));
	m_pItem1->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh"));
	//Ptr<CMaterial> pMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"TestMtrl");
	m_pItem1->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"TestMtrl"));
	//m_pItem1->SetUiRenderCheck(false);
	CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"Item")->AddGameObject(m_pItem1);

	// item2
	m_pItem2 = new CGameObject;
	m_pItem2->SetName(L"Item2");
	m_pItem2->FrustumCheck(true);
	m_pItem2->AddComponent(new CTransform);
	m_pItem2->AddComponent(new CMeshRender);
	m_pItem2->Transform()->SetLocalPos(Vector3(0.f, 0.f, 0.f));
	m_pItem2->Transform()->SetLocalScale(Vector3(200.f, 200.f, 200.f));
	m_pItem2->Transform()->SetLocalRot(Vector3(0.f, 0.f, 0.f));
	m_pItem2->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh"));
	//Ptr<CMaterial> pMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"TestMtrl");
	m_pItem2->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"TestMtrl"));
	//m_pItem2->SetUiRenderCheck(false);
	CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"Item")->AddGameObject(m_pItem2);

	// item3
	m_pItem3 = new CGameObject;
	m_pItem3->SetName(L"Item3");
	m_pItem3->FrustumCheck(true);
	m_pItem3->AddComponent(new CTransform);
	m_pItem3->AddComponent(new CMeshRender);
	m_pItem3->Transform()->SetLocalPos(Vector3(0.f, 0.f, 0.f));
	m_pItem3->Transform()->SetLocalScale(Vector3(200.f, 200.f, 200.f));
	m_pItem3->Transform()->SetLocalRot(Vector3(0.f, 0.f, 0.f));
	m_pItem3->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh"));
	//Ptr<CMaterial> pMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"TestMtrl");
	m_pItem3->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"TestMtrl"));
	//m_pItem3->SetUiRenderCheck(false);
	CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"Item")->AddGameObject(m_pItem3);
}