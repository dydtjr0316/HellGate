#include "pch.h"
#include "SwordScript.h"

CSwordScript::CSwordScript()
	: CScript((UINT)COMPONENT_TYPE::SCRIPT)
{
	m_pBoneFinalMat = nullptr;
}

CSwordScript::~CSwordScript()
{
}

void CSwordScript::update()
{
	
}

void CSwordScript::SetMeshData(WEAPONE_TYPE _eType)
{
	GetObj()->MeshRender()->SetMesh(m_pWeaponeMeshData[(UINT)_eType]->GetMesh());
	GetObj()->MeshRender()->SetMaterial(m_pWeaponeMeshData[(UINT)_eType]->GetMtrl());
}

void CSwordScript::BoneMatrender()
{
	m_pBoneFinalMat->UpdateData(TEXTURE_REGISTER::t7);
}