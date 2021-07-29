#include "pch.h"
#include "MeshRender.h"

#include "Transform.h"
#include "ResMgr.h"
#include "Animator3D.h"
#include "GameObject.h"
#include "SwordScript.h"

CMeshRender::CMeshRender()
	:CComponent(COMPONENT_TYPE::MESHRENDER)
	, m_bDynamicShadow(false)
{
}

CMeshRender::~CMeshRender()
{
}

void CMeshRender::render()
{

	if (IsActive() == false || nullptr == m_pMesh)
		return;

	int a = 1;
	for (size_t i = 0; i < m_vecMtrl.size(); ++i)
	{
		if (nullptr == m_vecMtrl[i] || nullptr == m_vecMtrl[i]->GetShader())
			continue;

		// Transform ���� ������Ʈ
		Transform()->UpdateData();

		// Animator3D ������Ʈ�� �ִ� ���...
		if (Animator3D())
		{
			Animator3D()->UpdateData();
			a = 1;
			m_vecMtrl[i]->SetData(SHADER_PARAM::INT_0, &a); // Animation Mesh �˸���
		}

		//Sword�� ���
		if (GetObj()->GetName() == L"sword")
		{
			CSwordScript* swordScript = GetObj()->GetScript<CSwordScript>();
			//CSwordScript* SwordScript = pSword->GetScript<CSwordScript>();
			swordScript->BoneMatrender();
			a = 1;
			m_vecMtrl[i]->SetData(SHADER_PARAM::INT_3, &a); // Animation Mesh �˸���
		}

		m_vecMtrl[i]->UpdateData();
		m_pMesh->render((UINT)i);

		m_vecMtrl[i]->SetData(SHADER_PARAM::INT_0, &a); // Animation Mesh �˸���
		m_vecMtrl[i]->SetData(SHADER_PARAM::INT_3, &a); // Animation Mesh �˸���
	}

}

void CMeshRender::render_shadowmap()
{
	int a = 1;
	Ptr<CMaterial> pMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"ShadowMapMtrl");

	for (UINT i = 0; i < m_pMesh->GetSubsetCount(); ++i)
	{
		if (Animator3D())
		{
			Animator3D()->UpdateData();
			pMtrl->SetData(SHADER_PARAM::INT_0, &a); // Animation Mesh �˸���
		}

		Transform()->UpdateData();
		pMtrl->UpdateData();
		m_pMesh->render(i);
	}

	// ����
	if (Animator3D())
	{
		a = 0;
		pMtrl->SetData(SHADER_PARAM::INT_0, &a);
	}
}

Ptr<CMaterial> CMeshRender::GetCloneMaterial(UINT _iSubSet)
{
	if (nullptr == m_vecMtrl[_iSubSet])
		return nullptr;

	m_vecMtrl[_iSubSet] = m_vecMtrl[_iSubSet]->Clone();
	return m_vecMtrl[_iSubSet];
}

void CMeshRender::SetMaterial(Ptr<CMaterial> _pMtrl, UINT _iSubset)
{
	if (m_vecMtrl.size() <= (size_t)_iSubset)
		m_vecMtrl.resize(_iSubset + 1);

	m_vecMtrl[_iSubset] = _pMtrl;
}

void CMeshRender::SaveToScene(FILE* _pFile)
{
}

void CMeshRender::LoadFromScene(FILE* _pFile)
{
}
