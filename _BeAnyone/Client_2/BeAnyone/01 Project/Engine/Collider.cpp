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
	, m_vOffsetPos(Vec3(0.f, 0.f, 0.f))
	, m_vOffsetScale(Vec3(1.f, 1.f, 1.f))
	, m_pColMesh(nullptr)
	, m_pColMtrl(nullptr)
	, m_iColID(g_iColID++)
	, m_iCollisionCount(0)
	, m_eType(COLLIDER_TYPE::RECT)
{
	m_pColMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"ColliderMtrl_0");
	SetColliderType(m_eType);
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

	Vec3 vFinalPos = m_vOffsetPos;
	vFinalPos = vFinalPos / Transform()->GetWorldScale(); // GetWorldScale() ·Î º¯°æ

	Matrix matTranslation = XMMatrixTranslation(vFinalPos.x, vFinalPos.y, vFinalPos.z);
	Matrix matScale = XMMatrixScaling(m_vOffsetScale.x, m_vOffsetScale.y, m_vOffsetScale.z);

	m_matColWorld = matScale * matTranslation;
	m_matColWorld *= Transform()->GetWorldMat();

	auto i = this;


}

void CCollider::render()
{
	if (!IsActive())
		return;

	static CConstantBuffer* pCB = CDevice::GetInst()->GetCB(CONST_REGISTER::b0);

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
	}
	else if (COLLIDER_TYPE::SPHERE == m_eType)
	{
		m_pColMesh = CResMgr::GetInst()->FindRes<CMesh>(L"ColSphereMesh");
	}
}

void CCollider::OnCollisionEnter(CCollider* _pOther)
{
	m_iCollisionCount += 1;

	const vector<CScript*>& vecScripts = GetObj()->GetScripts();
	for (size_t i = 0; i < vecScripts.size(); ++i)
	{
		vecScripts[i]->OnCollisionEnter(_pOther);
	}
}

void CCollider::OnCollision(CCollider* _pOther)
{
	if (0 < m_iCollisionCount)
	{
		m_pColMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"ColliderMtrl_1");
	}

	const vector<CScript*>& vecScripts = GetObj()->GetScripts();
	for (size_t i = 0; i < vecScripts.size(); ++i)
	{
		vecScripts[i]->OnCollision(_pOther);
	}
}

void CCollider::OnCollisionExit(CCollider* _pOther)
{
	m_iCollisionCount -= 1;
	if (m_iCollisionCount == 0)
		m_pColMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"ColliderMtrl_0");

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

	fwrite(&m_vOffsetPos, sizeof(Vec3), 1, _pFile);
	fwrite(&m_vOffsetScale, sizeof(Vec3), 1, _pFile);
	fwrite(&m_eType, sizeof(UINT), 1, _pFile);
}

void CCollider::LoadFromScene(FILE* _pFile)
{
	fread(&m_vOffsetPos, sizeof(Vec3), 1, _pFile);
	fread(&m_vOffsetScale, sizeof(Vec3), 1, _pFile);
	fread(&m_eType, sizeof(UINT), 1, _pFile);
	SetColliderType(m_eType);
}