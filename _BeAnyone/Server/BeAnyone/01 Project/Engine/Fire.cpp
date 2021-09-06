#include "pch.h"
#include "Fire.h"
#include "TimeMgr.h"

void CFireScript::init()
{
	Ptr<CMaterial> pMtrl = MeshRender()->GetSharedMaterial();
	pMtrl->SetData(SHADER_PARAM::FLOAT_0, &m_NoiseBuffer.frameTime);
	pMtrl->SetData(SHADER_PARAM::FLOAT_1, &m_NoiseBuffer.padding);
	pMtrl->SetData(SHADER_PARAM::VEC4_0, &m_NoiseBuffer.scrollSpeeds);
	pMtrl->SetData(SHADER_PARAM::VEC4_1, &m_NoiseBuffer.scales);

	pMtrl->SetData(SHADER_PARAM::VEC2_0, &m_DistortionBuffer.distortion1);
	pMtrl->SetData(SHADER_PARAM::VEC2_1, &m_DistortionBuffer.distortion2);
	pMtrl->SetData(SHADER_PARAM::VEC2_2, &m_DistortionBuffer.distortion3);
	pMtrl->SetData(SHADER_PARAM::FLOAT_2, &m_DistortionBuffer.distortionScale);
	pMtrl->SetData(SHADER_PARAM::FLOAT_3, &m_DistortionBuffer.distortionBias);

}

void CFireScript::update()
{
	//	빌보드
	//Vector3 PlayerRot = g_Object.find(g_myid)->second->Transform()->GetLocalRot();
	//Transform()->SetLocalRot(Vector3(PlayerRot + Vector3(0.f, XM_PI, 0.f)));

	//	frametime ConstBuffer Parameter 최신화
	m_NoiseBuffer.frameTime += DT;
	//cout << m_NoiseBuffer.frameTime << endl;
	if (m_NoiseBuffer.frameTime > 1000.f)
		m_NoiseBuffer.frameTime = 0.f;
	MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::FLOAT_0, &m_NoiseBuffer.frameTime);

}

void CFireScript::OnCollisionEnter(CCollider* _pOther)
{
}


CFireScript::CFireScript()
	:CScript((UINT)COMPONENT_TYPE::SCRIPT)
{
	m_NoiseBuffer.scrollSpeeds = Vector4(1.3f, 2.1f, 2.3f, 1.f);
	m_NoiseBuffer.scales = Vector4(1.f, 2.f, 3.f, 1.f);
	m_NoiseBuffer.padding = 0.0f;
	m_NoiseBuffer.frameTime = 0.f;
	//	
	m_NoiseBuffer.frameTime = CTimeMgr::GetInst()->GetAccTime();

	m_DistortionBuffer.distortion1 = Vector2(0.1f, 0.2f);
	m_DistortionBuffer.distortion2 = Vector2(0.1f, 0.3f);
	m_DistortionBuffer.distortion3 = Vector2(0.1f, 0.1f);
	m_DistortionBuffer.distortionBias = 0.8f;
	m_DistortionBuffer.distortionScale = 0.5f;
}

CFireScript::~CFireScript()
{
}
