#include "pch.h"
#include "Collider.h"

#include "ResMgr.h"

#include "GameObject.h"
#include "Transform.h"
#include "Device.h"
#include "ConstantBuffer.h"

#include "Script.h"

UINT CCollider::g_iColID = 0;

CCollider::CCollider()
	: CComponent(COMPONENT_TYPE::COLLIDER)
	, m_vOffsetPos(Vector3(0.f, 0.f, 0.f))
	, m_vOffsetScale(Vector3(1.f, 1.f, 1.f))
	, m_pColMesh(nullptr)
	, m_pColMtrl(nullptr)
	, m_iColID(g_iColID++)
	, m_iCollisionCount(0)
	, m_eType(COLLIDER_TYPE::BOX)
{
	m_pColMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"ColMtrl");
	SetColliderType(m_eType);

	m_bbx = BoundingBox(XMFLOAT3(0.f, 0.f, 0.f), XMFLOAT3(1.f, 1.f, 1.f));
	m_bSp = BoundingSphere(XMFLOAT3(0.f, 0.f, 0.f), 1.f);
}

CCollider::CCollider(const CCollider& _other)
	: CComponent(COMPONENT_TYPE::COLLIDER)
	, m_vOffsetPos(_other.m_vOffsetPos)
	, m_vOffsetScale(_other.m_vOffsetScale)
	, m_eType(_other.m_eType)
	, m_iCollisionCount(0)
	, m_pColMesh(_other.m_pColMesh)
	, m_pColMtrl(_other.m_pColMtrl)
	, m_matColWorld(_other.m_matColWorld)
	, m_iColID(g_iColID++)
{
}

void CCollider::operator=(const CCollider& _other)
{
	UINT iColID = m_iColID;
	memcpy(this, &_other, sizeof(CCollider));
	m_iColID = iColID;
}

CCollider::~CCollider()
{
}

void CCollider::update()
{
}

void CCollider::finalupdate()
{
	if (!IsActive())
		return;

	Vector3 vFinalPos = m_vOffsetPos;
	vFinalPos = vFinalPos / Transform()->GetWorldScale(); // GetWorldScale() ·Î º¯°æ

	Matrix matTranslation = XMMatrixTranslation(vFinalPos.x, vFinalPos.y, vFinalPos.z);
	Matrix matScale = XMMatrixScaling(m_vOffsetScale.x, m_vOffsetScale.y, m_vOffsetScale.z);

	m_matColWorld = matScale * matTranslation;
	m_matColWorld *= Transform()->GetWorldMat();


	m_bbx.Center = Transform()->GetLocalPos();

	m_bSp.Center = Transform()->GetLocalPos();
	m_bSp.Center.y = Transform()->GetLocalPos().y + m_bSp.Radius;

	/*m_bbx.Extents = XMFLOAT3(1.0f, 1.0f, 1.0f);
	m_bbx.Extents = (Transform()->GetLocalScale()) * m_bbx.Extents;*/

	
}

void CCollider::render()
{
	if (!IsActive())
	{
		int i = 0;
		return;
	}

	static CConstantBuffer* pCB = CDevice::GetInst()->GetCB(CONST_REGISTER::b0);
	m_matColWorld._42 += m_bSp.Radius;
	g_transform.matWorld = m_matColWorld;
	

	CDevice::GetInst()->SetConstBufferToRegister(pCB, pCB->AddData(&g_transform));

	m_pColMtrl->UpdateData();
	m_pColMesh->render();

	memset(&m_matColWorld, 0, sizeof(Matrix));
}

void CCollider::SetColliderType(COLLIDER_TYPE _eType)
{
	m_eType = _eType;

	if (COLLIDER_TYPE::RECT == m_eType)
	{
		m_pColMesh = CResMgr::GetInst()->FindRes<CMesh>(L"ColRectMesh");
	}
	else if (COLLIDER_TYPE::CIRCLE == m_eType)
	{
		m_pColMesh = CResMgr::GetInst()->FindRes<CMesh>(L"ColCircleMesh");
	}
	else if (COLLIDER_TYPE::BOX == m_eType)
	{
		m_pColMesh = CResMgr::GetInst()->FindRes<CMesh>(L"ColBoxMesh");
		//m_pColMesh = CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh");
	}
	else if (COLLIDER_TYPE::SPHERE == m_eType)
	{
		m_pColMesh = CResMgr::GetInst()->FindRes<CMesh>(L"ColSphereMesh");
	}
	else
	{
		m_pColMesh = CResMgr::GetInst()->FindRes<CMesh>(L"ColSphereMesh");
	}
	/*else if (COLLIDER_TYPE::MESH == m_eType)
	{
		m_pColMesh = CResMgr::GetInst()->FindRes<CMesh>(L"xMesh");
	}*/
}

void CCollider::SetColliderType(COLLIDER_TYPE _eType, wstring _str)
{
	m_eType = _eType;

	if (COLLIDER_TYPE::RECT == m_eType)
	{
		m_pColMesh = CResMgr::GetInst()->FindRes<CMesh>(L"ColRectMesh");
	}
	else if (COLLIDER_TYPE::CIRCLE == m_eType)
	{
		m_pColMesh = CResMgr::GetInst()->FindRes<CMesh>(L"ColCircleMesh");
	}
	else if (COLLIDER_TYPE::BOX == m_eType)
	{
		m_pColMesh = CResMgr::GetInst()->FindRes<CMesh>(L"ColBoxMesh");
		//m_pColMesh = CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh");
	}
	else if (COLLIDER_TYPE::SPHERE == m_eType)
	{
		//m_pColMesh = CResMgr::GetInst()->FindRes<CMesh>(L"ColSphereMesh");
	}
	else if (COLLIDER_TYPE::MESH == m_eType)
	{
		m_pColMesh = CResMgr::GetInst()->FindRes<CMesh>(_str);
	}
	else
	{
		m_pColMesh = CResMgr::GetInst()->FindRes<CMesh>(L"ColSphereMesh");
	}

}

void CCollider::OnCollisionEnter(CCollider* _pOther)
{
	m_iCollisionCount++;
	const vector<CScript*>& vecScripts = GetObj()->GetScripts();
	for (size_t i = 0; i < vecScripts.size(); ++i)
	{
		vecScripts[i]->OnCollisionEnter(_pOther);
	}
}

void CCollider::OnCollision(CCollider* _pOther)
{
	/*if (0 < m_iCollisionCount)
	{
		m_pColMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"ColliderMtrl_1");
	}*/

	const vector<CScript*>& vecScripts = GetObj()->GetScripts();
	for (size_t i = 0; i < vecScripts.size(); ++i)
	{
		vecScripts[i]->OnCollision(_pOther);
	}
}

void CCollider::OnCollisionExit(CCollider* _pOther)
{
	m_iCollisionCount -= 1;
	//if (m_iCollisionCount == 0)
	//	m_pColMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"ColliderMtrl_0");

	const vector<CScript*>& vecScripts = GetObj()->GetScripts();
	for (size_t i = 0; i < vecScripts.size(); ++i)
	{
		vecScripts[i]->OnCollisionExit(_pOther);
	}
}


void CCollider::SaveToScene(FILE* _pFile)
{
	UINT iType = (UINT)GetComponentType();
	fwrite(&iType, sizeof(UINT), 1, _pFile);

	fwrite(&m_vOffsetPos, sizeof(Vector3), 1, _pFile);
	fwrite(&m_vOffsetScale, sizeof(Vector3), 1, _pFile);
	fwrite(&m_eType, sizeof(UINT), 1, _pFile);
}

void CCollider::LoadFromScene(FILE* _pFile)
{
	fread(&m_vOffsetPos, sizeof(Vector3), 1, _pFile);
	fread(&m_vOffsetScale, sizeof(Vector3), 1, _pFile);
	fread(&m_eType, sizeof(UINT), 1, _pFile);
	SetColliderType(m_eType);
}