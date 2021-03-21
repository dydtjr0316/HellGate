#include "pch.h"
#include "Light.h"

#include "Transform.h"
#include "RenderMgr.h"

#include "ResMgr.h"

CLight::CLight()
	:CComponent(COMPONENT_TYPE::LIGHT)
{
}

CLight::~CLight()
{
}

void CLight::SetLightType(LIGHT_TYPE _eType)
{
	m_tLightInfo.iLightType = (int)_eType;

	if (LIGHT_TYPE::DIR == (LIGHT_TYPE)m_tLightInfo.iLightType)
	{
		m_pVolumeMesh = CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh");
		m_pLightMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"DirLightMtrl");
	}
	else if (LIGHT_TYPE::POINT == (LIGHT_TYPE)m_tLightInfo.iLightType)
	{
		m_pVolumeMesh = CResMgr::GetInst()->FindRes<CMesh>(L"SphereMesh");
		m_pLightMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"PointLightMtrl");
	}
	else if (LIGHT_TYPE::SPOT == (LIGHT_TYPE)m_tLightInfo.iLightType)
	{
		m_pVolumeMesh = CResMgr::GetInst()->FindRes<CMesh>(L"ConeMesh");
	}
}

void CLight::SetLightPos(const Vector3& _vPos)
{
	m_tLightInfo.vLightPos = _vPos;
	Transform()->SetLocalPos(_vPos);
}

void CLight::SetLightDir(const Vector3& _vDir)
{
	m_tLightInfo.vLightDir = _vDir;
	m_tLightInfo.vLightDir.Normalize();
}

void CLight::finalupdate()
{
	m_tLightInfo.vLightPos = Transform()->GetWorldPos();
	Transform()->SetLocalScale(Vector3(m_tLightInfo.fRange, m_tLightInfo.fRange, m_tLightInfo.fRange));
	m_iArrIdx = CRenderMgr::GetInst()->RegisterLight(this);
}

void CLight::render()
{
	if (-1 == m_iArrIdx)
		return;

	Transform()->UpdateData();
	m_pLightMtrl->SetData(SHADER_PARAM::INT_0, &m_iArrIdx); // ±¤¿ø ¹è¿­ ÀÎµ¦½ºÁ¤º¸ ¾÷µ¥ÀÌÆ®
	m_pLightMtrl->UpdateData(); // int_0, ±¤¿ø ÀÎµ¦½º , tex_0 : normaltarget, tex_1 : positiontarget 
	m_pVolumeMesh->render();
}

void CLight::SaveToScene(FILE* _pFile)
{
}

void CLight::LoadFromScene(FILE* _pFile)
{
}
