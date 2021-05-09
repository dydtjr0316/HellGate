#include "pch.h"
#include "PlayerScript.h"
bool checkOnce = true;

CPlayerScript::CPlayerScript()
	: CScript((UINT)SCRIPT_TYPE::PLAYERSCRIPT)
	, m_pOriginMtrl(nullptr)
	, m_pCloneMtrl(nullptr)
{
}

CPlayerScript::~CPlayerScript()
{
}

void CPlayerScript::awake()
{
	m_pOriginMtrl = MeshRender()->GetSharedMaterial();
	m_pCloneMtrl = m_pOriginMtrl->Clone();

	int a = 1;
	m_pCloneMtrl->SetData(SHADER_PARAM::INT_0, &a);
}

void CPlayerScript::update()
{
	//Vector3 vPos = g_Object.find(g_myid)->second->Transform()->GetLocalPos();
	////Vector3 vRot = Transform()->GetLocalRot();

	Vector3 localPos = g_Object.find(g_myid)->second->Transform()->GetLocalPos();

	if (KEY_HOLD(KEY_TYPE::KEY_W))
	{
		g_netMgr.Send_Move_Packet(MV_FRONT, localPos, -g_Object.find(g_myid)->second->Transform()->GetWorldDir(DIR_TYPE::FRONT) * 200.f * DT);
		// ��ŸŸ�� ���� �������� ����
	
		localPos += -g_Object.find(g_myid)->second->Transform()->GetWorldDir(DIR_TYPE::FRONT) * 200.f * DT;
		g_Object.find(g_myid)->second->Transform()->SetLocalPos(localPos);
	}

	if (KEY_HOLD(KEY_TYPE::KEY_S))
	{
		//vPos.z -= DT * 200.f;
		g_netMgr.Send_Move_Packet(MV_BACK, localPos, g_Object.find(g_myid)->second->Transform()->GetWorldDir(DIR_TYPE::FRONT) * 200.f * DT);
	
		localPos += g_Object.find(g_myid)->second->Transform()->GetWorldDir(DIR_TYPE::FRONT) * 200.f * DT;
		g_Object.find(g_myid)->second->Transform()->SetLocalPos(localPos);
	}

	if (KEY_HOLD(KEY_TYPE::KEY_A))
	{
		//vPos.x -= DT * 200.f;
		g_netMgr.Send_Move_Packet(MV_LEFT, localPos, g_Object.find(g_myid)->second->Transform()->GetWorldDir(DIR_TYPE::RIGHT) * 200.f * DT);
		localPos += g_Object.find(g_myid)->second->Transform()->GetWorldDir(DIR_TYPE::RIGHT) * 200.f * DT;
		g_Object.find(g_myid)->second->Transform()->SetLocalPos(localPos);
	}

	if (KEY_HOLD(KEY_TYPE::KEY_D))
	{
		g_netMgr.Send_Move_Packet(MV_RIGHT, localPos, -g_Object.find(g_myid)->second->Transform()->GetWorldDir(DIR_TYPE::RIGHT) * 200.f * DT);
		
		localPos += -g_Object.find(g_myid)->second->Transform()->GetWorldDir(DIR_TYPE::RIGHT) * 200.f * DT;
		g_Object.find(g_myid)->second->Transform()->SetLocalPos(localPos);
	}

	// z Ű�� ������ z �� ȸ��
	if (KEY_HOLD(KEY_TYPE::KEY_Z))
	{
		//vRot.z += DT * XM_PI;

		// ���� ���׸����� MeshRender �� ����
		MeshRender()->SetMaterial(m_pCloneMtrl);
	}
	else if (KEY_AWAY(KEY_TYPE::KEY_Z))
	{
		// z Ű�� ���� ���� ���׸���� ���ư�
		MeshRender()->SetMaterial(m_pOriginMtrl);
	}

	if (KEY_HOLD(KEY_TYPE::KEY_LBTN))
	{
		Vector2 vDrag = CKeyMgr::GetInst()->GetDragDir();
		Vector3 vRot = g_Object.find(g_myid)->second->Transform()->GetLocalRot();

		vRot.y += vDrag.x * DT * ROTATE_SPEED;
		
		g_netMgr.Send_Rotate_Packet(Rotate_LBTN, vRot.y);

		g_Object.find(g_myid)->second->Transform()->SetLocalRot(vRot);

	}
	
	localPos.y -= m_xmf3Velocity.y;

	g_Object.find(g_myid)->second->Transform()->SetLocalPos(localPos);

	OnPlayerUpdateCallback();

}


void CPlayerScript::OnPlayerUpdateCallback()
{
	CTerrain* pTerrain = (CTerrain*)m_pTerrainObj;

	XMFLOAT3 xmf3Scale = pTerrain->Transform()->GetLocalScale();

	//	��������
	XMFLOAT3 xmf3PlayerPosition = g_Object.find(g_myid)->second->Transform()->GetLocalPos();

	int z = (int)(xmf3PlayerPosition.z / xmf3Scale.z);
	bool bReverseQuad = ((z % 2) != 0);
	float fHeight = pTerrain->GetHeight(xmf3PlayerPosition.x, xmf3PlayerPosition.z, bReverseQuad) * 1.f + 30.0f;

	if (xmf3PlayerPosition.y < fHeight)
	{
		XMFLOAT3 xmf3PlayerVelocity = g_Object.find(g_myid)->second->GetScript<CPlayerScript>()->GetVelocity();
		xmf3PlayerVelocity.y = 0.0f;
		g_Object.find(g_myid)->second->GetScript<CPlayerScript>()->SetVelocity(xmf3PlayerVelocity);
		xmf3PlayerPosition.y = fHeight;
		g_Object.find(g_myid)->second->Transform()->SetLocalPos(xmf3PlayerPosition);
	}

	if (xmf3PlayerPosition.y > fHeight + 6.0f)
	{
		m_xmf3Velocity.y = 0.3f;
		g_Object.find(g_myid)->second->GetScript<CPlayerScript>()->SetVelocity(xmf3PlayerPosition);
	}
}
