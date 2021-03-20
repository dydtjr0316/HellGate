#include "pch.h"
#include "Light.h"

#include "Transform.h"
#include "RenderMgr.h"

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
	CRenderMgr::GetInst()->RegisterLight(this);
}

void CLight::SaveToScene(FILE* _pFile)
{
}

void CLight::LoadFromScene(FILE* _pFile)
{
}
