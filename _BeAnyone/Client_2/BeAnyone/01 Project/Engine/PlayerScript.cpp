#include "pch.h"
#include "PlayerScript.h"
#include "Terrain.h"

//#include "BulletScript.h"
//#include "TestScript.h"

CPlayerScript::CPlayerScript()
	: CScript( (UINT)SCRIPT_TYPE::PLAYERSCRIPT )
	, m_pOriginMtrl( nullptr )
	, m_pCloneMtrl( nullptr )
	, m_xmf3Velocity(XMFLOAT3(0.f, 0.3f, 0.0f))
{
}

CPlayerScript::CPlayerScript(CTerrain* _terrain)
	: CScript((UINT)SCRIPT_TYPE::PLAYERSCRIPT)
	, m_pOriginMtrl(nullptr)
	, m_pCloneMtrl(nullptr)
	, m_pTerrainObj(_terrain)
	, m_xmf3Velocity(XMFLOAT3(0.f,0.3f,0.0f))
{
}

CPlayerScript::~CPlayerScript()
{
}

void CPlayerScript::awake() {
	m_pOriginMtrl = MeshRender()->GetSharedMaterial();
	m_pCloneMtrl = m_pOriginMtrl->Clone();

	int a = 1;
	m_pCloneMtrl->SetData( SHADER_PARAM::INT_0, &a );
}

void CPlayerScript::update() {
	Vec3 vPos = Transform()->GetLocalPos();
	Vec3 vRot = Transform()->GetLocalRot();
		
	if (KEY_HOLD( KEY_TYPE::KEY_UP ))
	{
		Vec3 vFront = -Transform()->GetWorldDir(DIR_TYPE::FRONT);
		vPos += vFront * DT * 200.f;
		//vPos.y += DT * 200.f;
	}

	if (KEY_HOLD( KEY_TYPE::KEY_DOWN ))                         
	{
		Vec3 vBack = Transform()->GetWorldDir(DIR_TYPE::FRONT);
		vPos += vBack * DT * 200.f;
		//vPos.y -= DT * 200.f;
	}

	if (KEY_HOLD( KEY_TYPE::KEY_LEFT ))
	{
		Vec3 vLeft = Transform()->GetWorldDir(DIR_TYPE::RIGHT);
		vPos += vLeft * DT * 200.f;
		//vPos.x -= DT * 200.f;
	}

	if (KEY_HOLD( KEY_TYPE::KEY_RIGHT ))
	{
		Vec3 vRight = -Transform()->GetWorldDir(DIR_TYPE::RIGHT);
		vPos += vRight * DT * 200.f;
		//vPos.x += DT * 200.f;
	}

	// z Ű�� ������ z �� ȸ��
	if (KEY_HOLD( KEY_TYPE::KEY_Z ))
	{
		vRot.z += DT * XM_PI;

		// ���� ���׸����� MeshRender �� ����
		MeshRender()->SetMaterial( m_pCloneMtrl );
	}
	else if (KEY_AWAY( KEY_TYPE::KEY_Z ))
	{
		// z Ű�� ���� ���� ���׸���� ���ư�
		MeshRender()->SetMaterial( m_pOriginMtrl );
	}

	if (KEY_HOLD(KEY_TYPE::KEY_RBTN))
	{
		Vec2 vDrag = CKeyMgr::GetInst()->GetDragDir();
		Vec3 vRot = Transform()->GetLocalRot();

		//vRot.x -= vDrag.y * DT * 3.f;
		vRot.y += vDrag.x * DT * 1.5f;

		Transform()->SetLocalRot(vRot);
	}

	//	 ���� �߷�
	vPos.y -= m_xmf3Velocity.y;

	Transform()->SetLocalPos( vPos );
	OnPlayerUpdateCallback();

	//Transform()->SetLocalRot( vRot );

}

// ����Ʈ�� �浹ó��
void CPlayerScript::OnPlayerUpdateCallback()
{
	CTerrain* pTerrain = (CTerrain*)m_pTerrainObj;
	XMFLOAT3 xmf3Scale = XMFLOAT3(1.0f, 1.0f, 1.0f);/*pTerrain->GetScale();*/
	XMFLOAT3 xmf3PlayerPosition = Transform()->GetLocalPos();

	int z = (int)(xmf3PlayerPosition.z / xmf3Scale.z);
	bool bReverseQuad = ((z % 2) != 0);
	float fHeight = pTerrain->GetHeight(xmf3PlayerPosition.x, xmf3PlayerPosition.z, bReverseQuad) /*+ 6.0f*/;

	if (xmf3PlayerPosition.y < fHeight)
	{
		XMFLOAT3 xmf3PlayerVelocity = GetVelocity();
		xmf3PlayerVelocity.y = 0.0f;
		SetVelocity(xmf3PlayerVelocity);
		xmf3PlayerPosition.y = fHeight;
		Transform()->SetLocalPos(xmf3PlayerPosition);
	}

	if (xmf3PlayerPosition.y > fHeight + 6.0f)
	{
		m_xmf3Velocity.y = 0.3f;
		SetVelocity(xmf3PlayerPosition);
	}
}



//CPlayerScript::CPlayerScript(CTerrain* _pTerrain)
//	: CScript((UINT)SCRIPT_TYPE::PLAYERSCRIPT)
//	, m_pOriginMtrl(nullptr)
//	, m_pCloneMtrl(nullptr)
//	, m_pTerrainObj(_pTerrain)
//{
//}
