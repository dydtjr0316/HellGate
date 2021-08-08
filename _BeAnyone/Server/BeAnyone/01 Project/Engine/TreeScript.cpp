#include "pch.h"
#include "TreeScript.h"
#include "Terrain.h"
#include <random>
#include "DummyItemScript.h"
//#include <chrono>
//
//auto current = chrono::system_clock::now();
//auto duration = current.time_since_epoch();
//auto millis = chrono::duration_cast<chrono::milliseconds>(duration).count();
//
//mt19937_64 genMT(millis);

default_random_engine dreItem;
uniform_int_distribution<int> uidItem(0, 1);

uniform_int_distribution<int> uidStump(0, 1);

CTreeScript::CTreeScript()
	: CScript((UINT)COMPONENT_TYPE::SCRIPT) //CScript((UINT)SCRIPT_TYPE::MONSTERSCRIPT),
	, m_bIsAttack(false)
	, m_iAttackNum(0)
{
	// m_pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\Player\\PlayerMale@nWalk_F.fbx", FBX_TYPE::PLAYER);
}

CTreeScript::~CTreeScript()
{
}

void CTreeScript::update()
{
	if (m_bIsAttack == true) {
		if(m_bMakeObjects == true)
			MakeItem(); m_bMakeObjects = false;
		DestroyTree();
	}
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
	float   mapY = m_Terrain->GetHeight(vTreePos.x, vTreePos.z, true) * 2.f + 50.f;
	
	// item1
	Vector3 vItem1Pos = m_vItem[0]->Transform()->GetLocalPos();
	Vector3 vItem1Rot = m_vItem[0]->Transform()->GetLocalRot();
	Vector3 vItem1FromtDir = m_vItem[0]->Transform()->GetWorldDir(DIR_TYPE::FRONT);
	Vector3 vItem1UpDir = m_vItem[0]->Transform()->GetWorldDir(DIR_TYPE::UP);

	// item2
	Vector3 vItem2Pos = m_vItem[1]->Transform()->GetLocalPos();
	Vector3 vItem2Rot = m_vItem[1]->Transform()->GetLocalRot();
	Vector3 vItem2FromtDir = m_vItem[1]->Transform()->GetWorldDir(DIR_TYPE::FRONT);
	Vector3 vItem2UpDir = m_vItem[1]->Transform()->GetWorldDir(DIR_TYPE::UP);

	// item3
	Vector3 vItem3Pos = m_vItem[2]->Transform()->GetLocalPos();
	Vector3 vItem3Rot = m_vItem[2]->Transform()->GetLocalRot();
	Vector3 vItem3FromtDir = m_vItem[2]->Transform()->GetWorldDir(DIR_TYPE::FRONT);
	Vector3 vItem3UpDir = m_vItem[2]->Transform()->GetWorldDir(DIR_TYPE::UP);

	if (m_bFirst == true) {
		vItem1Pos = vTreePos;
		vItem2Pos = vTreePos;
		vItem3Pos = vTreePos;
		
		// 회전
		vItem1Rot.y = XM_PI / 2;
		vItem2Rot.y = XM_PI / 8;
		vItem3Rot.y = XM_PI;
	
		m_vItem[0]->Transform()->SetLocalRot(vItem1Rot);
		m_vItem[1]->Transform()->SetLocalRot(vItem2Rot);
		m_vItem[2]->Transform()->SetLocalRot(vItem3Rot);

		int randNum;
		for (int i = 0; i < m_vItem.size(); ++i) {
			randNum = uidItem(dreItem);
			m_vItem[i]->MeshRender()->SetMesh(m_pItemMeshData[randNum]->GetMesh());
			m_vItem[i]->MeshRender()->SetMaterial(m_pItemMeshData[randNum]->GetMtrl());

			if (m_vItem[i]->MeshRender()->GetMesh()->GetName() == L"Mesh\\Branch.mesh") {
				/*switch (i) {
				case 0:
					vItem1Rot.x = XM_PI / 2; break;
				case 1:
					vItem2Rot.x = XM_PI / 2; break;
				case 2:
					vItem3Rot.x = XM_PI / 2; break;
				default:
					break;
				}*/
				m_vItem[i]->Transform()->SetLocalScale(Vector3(120.f, 120.f, 120.f));
			}
		}
		randNum = uidStump(dreItem);
		GetObj()->MeshRender()->SetMesh(m_pStumpMeshData[randNum]->GetMesh());
		GetObj()->MeshRender()->SetMaterial(m_pStumpMeshData[randNum]->GetMtrl());

		m_bFirst = false;
		m_bisUp = true;
	}

	// up
	if (vItem1Pos.y <= mapY + 50.f && m_bisUp == true) {
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
	if (vItem1Pos.y >= mapY && m_bisDown == true) {
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

	m_vItem[0]->Transform()->SetLocalPos(vItem1Pos);
	m_vItem[1]->Transform()->SetLocalPos(vItem2Pos);
	m_vItem[2]->Transform()->SetLocalPos(vItem3Pos);

}

void CTreeScript::MakeItem()
{
	//


	// -------------------
	// item1
	CGameObject* pItem = new CGameObject;
	pItem->SetName(L"Item1");
	pItem->FrustumCheck(true);
	pItem->AddComponent(new CTransform);
	pItem->AddComponent(new CMeshRender);
	pItem->AddComponent(new CDummyItemScript);
	pItem->AddComponent(new CCollider);
	pItem->Transform()->SetLocalPos(Vector3(0.f, 0.f, 0.f));
	pItem->Transform()->SetLocalScale(Vector3(200.f, 200.f, 200.f));
	pItem->Transform()->SetLocalRot(Vector3(0.f, 0.f, 0.f));
	pItem->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh"));
	pItem->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"TestMtrl"));
	pItem->Collider()->SetColliderType(COLLIDER_TYPE::MESH, L"Branch");
	pItem->Collider()->SetBoundingBox(BoundingBox(pItem->Transform()->GetLocalPos(), pItem->MeshRender()->GetMesh()->GetBoundingBoxExtents()));
	pItem->Collider()->SetBoundingSphere(BoundingSphere(pItem->Transform()->GetLocalPos(), pItem->MeshRender()->GetMesh()->GetBoundingSphereRadius() / 2.f));
	//m_pItem1->SetUiRenderCheck(false);
	CDummyItemScript* pItemScript = pItem->GetScript<CDummyItemScript>();

	m_vItem.push_back(pItem);
	CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"Item")->AddGameObject(m_vItem[0]);

	// item2
	pItem = new CGameObject;
	pItem->SetName(L"Item2");
	pItem->FrustumCheck(true);
	pItem->AddComponent(new CTransform);
	pItem->AddComponent(new CMeshRender);
	pItem->AddComponent(new CDummyItemScript);
	pItem->AddComponent(new CCollider);
	pItem->Transform()->SetLocalPos(Vector3(0.f, 0.f, 0.f));
	pItem->Transform()->SetLocalScale(Vector3(200.f, 200.f, 200.f));
	pItem->Transform()->SetLocalRot(Vector3(0.f, 0.f, 0.f));
	pItem->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh"));
	pItem->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"TestMtrl"));
	pItem->Collider()->SetColliderType(COLLIDER_TYPE::MESH, L"Branch");
	pItem->Collider()->SetBoundingBox(BoundingBox(pItem->Transform()->GetLocalPos(), pItem->MeshRender()->GetMesh()->GetBoundingBoxExtents()));
	pItem->Collider()->SetBoundingSphere(BoundingSphere(pItem->Transform()->GetLocalPos(), pItem->MeshRender()->GetMesh()->GetBoundingSphereRadius() / 2.f));
	//m_pItem2->SetUiRenderCheck(false);
	pItemScript = pItem->GetScript<CDummyItemScript>();

	m_vItem.push_back(pItem);
	CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"Item")->AddGameObject(m_vItem[1]);

	// item3
	pItem = new CGameObject;
	pItem ->SetName(L"Item3");
	pItem->FrustumCheck(true);
	pItem->AddComponent(new CTransform);
	pItem->AddComponent(new CMeshRender);
	pItem->AddComponent(new CDummyItemScript);
	pItem->AddComponent(new CCollider);
	pItem->Transform()->SetLocalPos(Vector3(0.f, 0.f, 0.f));
	pItem->Transform()->SetLocalScale(Vector3(200.f, 200.f, 200.f));
	pItem->Transform()->SetLocalRot(Vector3(0.f, 0.f, 0.f));
	pItem->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh"));
	pItem->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"TestMtrl"));
	pItem->Collider()->SetColliderType(COLLIDER_TYPE::MESH, L"Branch");
	pItem->Collider()->SetBoundingBox(BoundingBox(pItem->Transform()->GetLocalPos(), pItem->MeshRender()->GetMesh()->GetBoundingBoxExtents()));
	pItem->Collider()->SetBoundingSphere(BoundingSphere(pItem->Transform()->GetLocalPos(), pItem->MeshRender()->GetMesh()->GetBoundingSphereRadius() / 2.f));
	//m_pItem3->SetUiRenderCheck(false);
	pItemScript = pItem->GetScript<CDummyItemScript>();

	m_vItem.push_back(pItem);
	CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"Item")->AddGameObject(m_vItem[2]);
}