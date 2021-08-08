#include "pch.h"
#include "ItemMgr.h"
#include <random>

#include "Transform.h"
#include "MeshRender.h"
#include "Collider.h"

#include "Terrain.h"

#include "DummyItemScript.h"

//
#include "MeshData.h"

default_random_engine dreMonsterItem;
uniform_int_distribution<int> uidMonsterItem(0, 4);

CItemMgr::CItemMgr()
{
}

CItemMgr::~CItemMgr()
{
}


void CItemMgr::init()
{
	
}

void CItemMgr::update()
{
	if (m_bMakeItem == true) {
		if (m_bFirst == true) {
			for (int i = 0; i < m_vItemPos.size() - m_iOldPosNum; ++i) {
				m_bMakeFirst.push_back(true);
				m_bisUp.push_back(false);
				m_bisDown.push_back(false);
			}
			m_bFirst = false;
			MakeObject();
		}
		MakeItem();
	}
}

void CItemMgr::MakeObject()
{
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
	CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"Item")->AddGameObject(pItem);

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
	CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"Item")->AddGameObject(pItem);

	// item3
	pItem = new CGameObject;
	pItem->SetName(L"Item3");
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
	CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"Item")->AddGameObject(pItem);
}

void CItemMgr::MakeItem()
{
	
	float mapY;
	int randNum;

	for (int i = m_iOldPosNum; i < m_vItemPos.size(); ++i) {

		mapY = m_Terrain->GetHeight(m_vItemPos[i].x, m_vItemPos[i].z, true) * 2.f;

		// item1
		Vector3 vItem1Pos = m_vItem[0 + (i * 3)]->Transform()->GetLocalPos();
		Vector3 vItem1Rot = m_vItem[0 + (i * 3)]->Transform()->GetLocalRot();
		Vector3 vItem1FromtDir = m_vItem[0 + (i * 3)]->Transform()->GetWorldDir(DIR_TYPE::FRONT);
		Vector3 vItem1UpDir = m_vItem[0 + (i * 3)]->Transform()->GetWorldDir(DIR_TYPE::UP);
	
		// item2
		Vector3 vItem2Pos = m_vItem[1 + (i * 3)]->Transform()->GetLocalPos();
		Vector3 vItem2Rot = m_vItem[1 + (i * 3)]->Transform()->GetLocalRot();
		Vector3 vItem2FromtDir = m_vItem[1 + (i * 3)]->Transform()->GetWorldDir(DIR_TYPE::FRONT);
		Vector3 vItem2UpDir = m_vItem[1 + (i * 3)]->Transform()->GetWorldDir(DIR_TYPE::UP);
	
		// item3
		Vector3 vItem3Pos = m_vItem[2 + (i * 3)]->Transform()->GetLocalPos();
		Vector3 vItem3Rot = m_vItem[2 + (i * 3)]->Transform()->GetLocalRot();
		Vector3 vItem3FromtDir = m_vItem[2 + (i * 3)]->Transform()->GetWorldDir(DIR_TYPE::FRONT);
		Vector3 vItem3UpDir = m_vItem[2 + (i * 3)]->Transform()->GetWorldDir(DIR_TYPE::UP);
	
		if (m_bMakeFirst[i] == true) {
			vItem1Pos = m_vItemPos[i];
			vItem2Pos = m_vItemPos[i];
			vItem3Pos = m_vItemPos[i];

			vItem1Pos.y -= 30.f;
			vItem2Pos.y -= 30.f;
			vItem3Pos.y -= 30.f;

			// È¸Àü
			vItem1Rot.y = XM_PI / 2;
			vItem2Rot.y = XM_PI / 8;
			vItem3Rot.y = XM_PI;

			/*vItem1Rot.x = XM_PI / 2;
			vItem2Rot.x = XM_PI / 2;
			vItem3Rot.x = XM_PI / 2;*/

			m_vItem[0 + (i * 3)]->Transform()->SetLocalRot(vItem1Rot);
			m_vItem[1 + (i * 3)]->Transform()->SetLocalRot(vItem2Rot);
			m_vItem[2 + (i * 3)]->Transform()->SetLocalRot(vItem3Rot);

			for (int j = m_iOldPosNum * 3; j < m_vItem.size(); ++j) {
				randNum = uidMonsterItem(dreMonsterItem);

				m_vItem[j]->MeshRender()->SetMesh(m_pItemMeshData[randNum]->GetMesh());
				m_vItem[j]->MeshRender()->SetMaterial(m_pItemMeshData[randNum]->GetMtrl());
			}

			m_bMakeFirst[i] = false;
			m_bisUp[i] = true;
		}

		// up
		if(vItem1Pos.y <= mapY + 50.f && m_bisUp[i] == true) {
			// m_iTemTime += DT;
			vItem1Pos += 60.f * DT * vItem1FromtDir;	// item1
			vItem1Pos += 50.f * DT * vItem1UpDir;
			vItem2Pos += 60.f * DT * vItem2FromtDir;	// item2
			vItem2Pos += 50.f * DT * vItem2UpDir;
			vItem3Pos += 60.f * DT * vItem3FromtDir;	// item3
			vItem3Pos += 50.f * DT * vItem3UpDir;
		}
		else {
			m_bisUp[i] = false;
			m_bisDown[i] = true;
		}

		// down
		if (vItem1Pos.y >= mapY && m_bisDown[i] == true) {
			vItem1Pos += 60.f * DT * vItem1FromtDir;	// item1
			vItem1Pos += 50.f * DT * -vItem1UpDir;
			vItem2Pos += 60.f * DT * vItem2FromtDir;	// item2
			vItem2Pos += 50.f * DT * -vItem2UpDir;
			vItem3Pos += 60.f * DT * vItem3FromtDir;	// item3
			vItem3Pos += 50.f * DT * -vItem3UpDir;
		}
		else if(m_bisUp[i] != true){
			m_bisDown[i] = false;
			m_bMakeItem = false;
			m_bFirst = true;
			m_iOldPosNum = m_vItemPos.size();
		}

		m_vItem[0 + (i * 3)]->Transform()->SetLocalPos(vItem1Pos);
		m_vItem[1 + (i * 3)]->Transform()->SetLocalPos(vItem2Pos);
		m_vItem[2 + (i * 3)]->Transform()->SetLocalPos(vItem3Pos);
		
	}
	

}
