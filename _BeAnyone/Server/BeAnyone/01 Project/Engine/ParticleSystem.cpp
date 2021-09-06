#include "pch.h"
#include "ParticleSystem.h"

#include "StructuredBuffer.h"
#include "ResMgr.h"
#include "KeyMgr.h"
#include "TimeMgr.h"
#include "EventMgr.h"

#include "Transform.h"

CParticleSystem::CParticleSystem()
	: CComponent(COMPONENT_TYPE::PARTICLESYSTEM)
	, m_pParticleBuffer(nullptr)
	, m_pSharedBuffer(nullptr)
	, m_iMaxParticle(1000)
	, m_fFrequency(0.002f)
	, m_fAccTime(0.f)
	, m_fMinLifeTime(0.5f)
	, m_fMaxLifeTime(0.8f)
	, m_fMinSpeed(100)
	, m_fMaxSpeed(50.f)
	, m_fStartScale(2.5f)
	, m_fEndScale(10.f)
	, m_vStartColor(Vector4(0.8f, 0.8f, 0.0f, 1.4f))
	, m_vEndColor(Vector4(1.0f, 1.0f, 1.0f, 1.0f))
	, m_bType(true)
{
	// 구조화 버퍼 생성
	m_pParticleBuffer = new CStructuredBuffer;
	m_pParticleBuffer->Create(sizeof(tParticle), m_iMaxParticle, nullptr);

	m_pSharedBuffer = new CStructuredBuffer;
	m_pSharedBuffer->Create(sizeof(tParticleShared), 1, nullptr);

	// 사각형 Mesh
	m_pMesh = CResMgr::GetInst()->FindRes<CMesh>(L"PointMesh");

	// Material
	m_pMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"ParticleMtrl");
	Ptr<CTexture> pParticle = CResMgr::GetInst()->Load<CTexture>(L"Texture\\Particle\\CartoonSmoke.png", L"Texture\\Particle\\Sparks.png");
	m_pMtrl->SetData(SHADER_PARAM::TEX_0, pParticle.GetPointer());

	// ParticleUpdate
	m_pUpdateMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"ParticleUpdateMtrl");
}

CParticleSystem::~CParticleSystem()
{
	SAFE_DELETE(m_pParticleBuffer);
	SAFE_DELETE(m_pSharedBuffer);
}

void CParticleSystem::finalupdate()
{
	// 추가될 파티클 개수 계산
	m_fAccTime += DT;

	int iAdd = 0;

	if (m_bType)
		iAdd = InfiniteParticle();
	else
		iAdd = finiteParticle();


	m_pParticleBuffer->UpdateRWData(UAV_REGISTER::u0);
	m_pSharedBuffer->UpdateRWData(UAV_REGISTER::u1);

	m_pUpdateMtrl->SetData(SHADER_PARAM::INT_0, &m_iMaxParticle);
	m_pUpdateMtrl->SetData(SHADER_PARAM::INT_1, &iAdd);
	m_pUpdateMtrl->SetData(SHADER_PARAM::FLOAT_0, &m_fMinLifeTime);
	m_pUpdateMtrl->SetData(SHADER_PARAM::FLOAT_1, &m_fMaxLifeTime);

	m_pUpdateMtrl->SetData(SHADER_PARAM::FLOAT_2, &m_fMinSpeed);
	m_pUpdateMtrl->SetData(SHADER_PARAM::FLOAT_3, &m_fMaxSpeed);

	m_pUpdateMtrl->Dispatch(1, 1, 1);
}

void CParticleSystem::render()
{
	Transform()->UpdateData();

	m_pParticleBuffer->UpdateData(TEXTURE_REGISTER::t10);

	m_pMtrl->SetData(SHADER_PARAM::VEC4_0, &m_vStartColor);
	m_pMtrl->SetData(SHADER_PARAM::VEC4_1, &m_vEndColor);
	m_pMtrl->SetData(SHADER_PARAM::FLOAT_0, &m_fStartScale);
	m_pMtrl->SetData(SHADER_PARAM::FLOAT_1, &m_fEndScale);

	m_pMtrl->UpdateData();
	m_pMesh->render_instancing(m_iMaxParticle);
}

void CParticleSystem::Init(float frequency, float acctime, float minLife, float maxLife, float minSpeed, float maxSpeed, float StartScale, float EndScale, Vector4 StartColor, Vector4 EndColor)
{
	m_fFrequency = frequency;
	m_fAccTime = acctime;

	m_fMinLifeTime = minLife;
	m_fMaxLifeTime = maxLife;

	m_fMinSpeed = minSpeed;
	m_fMaxSpeed = maxSpeed;

	m_fStartScale = StartScale;
	m_fEndScale = EndScale;

	m_vStartColor = StartColor;
	m_vEndColor = EndColor;
}

int CParticleSystem::InfiniteParticle()
{
	int iAdd = 0;
	if (m_fFrequency < m_fAccTime)
	{
		m_fAccTime = m_fAccTime - m_fFrequency;
		iAdd = 1;
	}
	return iAdd;
}

int CParticleSystem::finiteParticle()
{
	int iAdd = 3;
	if (m_fAccTime > m_fMaxLifeTime)
	{
		m_fFrequency = 0.0f;
		//m_fAccTime = m_fAccTime - m_fFrequency;

		iAdd = 0;
	}
	else
	{
		//m_fAccTime = m_fMaxLifeTime;
		int a = 0;
	}
	//cout << m_fAccTime << endl;
	return iAdd;
}

void CParticleSystem::SaveToScene(FILE* _pFile)
{
}

void CParticleSystem::LoadFromScene(FILE* _pFile)
{
}
