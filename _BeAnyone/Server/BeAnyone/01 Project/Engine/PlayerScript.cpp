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
	/*Vec3 vPos = Transform()->GetLocalPos();
Vec3 vRot = Transform()->GetLocalRot();*/

	Vector3 localPos = g_Object.find(g_myid)->second->Transform()->GetLocalPos();

	if (KEY_HOLD(KEY_TYPE::KEY_W))
	{
		g_netMgr.Send_Move_Packet(MV_FRONT, localPos, -Transform()->GetWorldDir(DIR_TYPE::FRONT)*200.f*DT);	
		// ��ŸŸ�� ���� �������� ����
	}

	if (KEY_HOLD(KEY_TYPE::KEY_S))
	{
		//vPos.z -= DT * 200.f;
		g_netMgr.Send_Move_Packet(MV_BACK, localPos, Transform()->GetWorldDir(DIR_TYPE::FRONT) * 200.f * DT);
	}

	if (KEY_HOLD(KEY_TYPE::KEY_A))
	{
		//vPos.x -= DT * 200.f;
		g_netMgr.Send_Move_Packet(MV_LEFT, localPos, Transform()->GetWorldDir(DIR_TYPE::RIGHT) * 200.f * DT);
	}

	if (KEY_HOLD(KEY_TYPE::KEY_D))
	{
		//vPos.x += DT * 200.f;
		g_netMgr.Send_Move_Packet(MV_RIGHT, localPos, -Transform()->GetWorldDir(DIR_TYPE::RIGHT) * 200.f * DT);
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
		// �̱� ������Ʈ ȸ��
		//Vector2 vDrag = CKeyMgr::GetInst()->GetDragDir();
		//Vector3 vRot = Transform()->GetLocalRot();
	
		//////vRot.x -= vDrag.y * DT * 3.f;
		//vRot.y += vDrag.x * DT * 0.1f;

		Vector2 vDrag = CKeyMgr::GetInst()->GetDragDir();
		Vector3 vRot = Transform()->GetLocalRot();
		//Vector3 vRot = Transform()->GetLocalRot();
		vRot.y += vDrag.x * DT * 0.1f;
		//vDrag.x = vDrag.x * DT * 0.1f;
		 

		g_netMgr.Send_Rotate_Packet(Rotate_LBTN, vDrag, vRot);

		//g_netMgr.Send_Rotate_Packet(Rotate_LBTN, vDrag, vRot, DT);
		

		// �̱� ������Ʈ ȸ��
		//g_Object.find(g_myid)->second->Transform()->SetLocalRot(vRot);
		
		/*g_Object.find(g_myid)->second->Transform()->SetLocalRot(vRot);

		cout << "================================================================" << endl;
		cout << g_Object.find(g_myid)->second->Transform()->GetLocalRot().x << endl;
		cout << g_Object.find(g_myid)->second->Transform()->GetLocalRot().y << endl;
		cout << g_Object.find(g_myid)->second->Transform()->GetLocalRot().z << endl;
		cout << "================================================================" << endl;
		cout << Transform()->GetLocalRot().x << endl;
		cout << Transform()->GetLocalRot().y << endl;
		cout << Transform()->GetLocalRot().z << endl;
		cout << "================================================================" << endl;*/

	}
}