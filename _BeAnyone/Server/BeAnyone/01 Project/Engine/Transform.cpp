#include "pch.h"
#include "Transform.h"

#include "ConstantBuffer.h"
#include "Device.h"

tTransform	g_transform;
Vector3 vAsis[3] = { Vector3::Right , Vector3::Up , Vector3::Front };

CTransform::CTransform()
	: CComponent(COMPONENT_TYPE::TRANSFORM)
	, m_vLocalPos(Vector3(0.f, 0.f, 0.f))
	, m_vLocalRot(Vector3(0.f, 0.f, 0.f))
	, m_vLocalScale(Vector3(1.f, 1.f, 1.f))
{
}

CTransform::~CTransform()
{
}

void CTransform::finalupdate()
{
	Matrix matTranslation = XMMatrixTranslation(m_vLocalPos.x, m_vLocalPos.y, m_vLocalPos.z);
	Matrix matScale = XMMatrixScaling(m_vLocalScale.x, m_vLocalScale.y, m_vLocalScale.z);


	Matrix matRot = XMMatrixRotationX(m_vLocalRot.x);
	matRot *= XMMatrixRotationY(m_vLocalRot.y);
	matRot *= XMMatrixRotationZ(m_vLocalRot.z);

	static Vector3 arrDefault[(UINT)DIR_TYPE::END] = { Vector3::Right, Vector3::Up, Vector3::Front };

	for (UINT i = 0; i < (UINT)DIR_TYPE::END; ++i)
	{
		m_vLocalDir[i] = XMVector3TransformNormal(arrDefault[i], matRot);
	}

	// 로컬 x (크기 x 회전 x 이동)(월드행렬)
	m_matWorld = matScale * matRot * matTranslation;

	if (GetObj()->GetParent())
	{
		m_matWorld *= GetObj()->GetParent()->Transform()->GetWorldMat();

		for (UINT i = 0; i < (UINT)DIR_TYPE::END; ++i)
		{
			m_vWorldDir[i] = XMVector3TransformNormal(arrDefault[i], m_matWorld);
			m_vWorldDir[i].Normalize();
		}
	}
	else
	{
		memcpy(m_vWorldDir, m_vLocalDir, sizeof(Vector3) * 3);
	}
	
	// 역행렬 계산
	m_matWorldInv = XMMatrixInverse(nullptr, m_matWorld);
}

void CTransform::UpdateData()
{
	static CConstantBuffer* pCB = CDevice::GetInst()->GetCB(CONST_REGISTER::b0);

	g_transform.matWorld = m_matWorld;
	g_transform.matWV = g_transform.matWorld * g_transform.matView;
	g_transform.matWVP = g_transform.matWV * g_transform.matProj;

	UINT iOffsetPos = pCB->AddData(&g_transform);
	CDevice::GetInst()->SetConstBufferToRegister(pCB, iOffsetPos);
}


Vector3 CTransform::GetWorldScale()
{
	CGameObject* pParent = GetObj()->GetParent();
	Vector3 vWorldScale = m_vLocalScale;
	while (pParent)
	{
		vWorldScale *= pParent->Transform()->GetLocalScale();

		pParent = pParent->GetParent();
	}
	return vWorldScale;
}

bool CTransform::IsCasting(const Vector3& _vPos)
{
	Vector3 vWorldPos = GetWorldPos();
	Vector3 vWorldScale = GetWorldScale();
	if (vWorldPos.x - vWorldScale.x / 2.f < _vPos.x && _vPos.x < vWorldPos.x + vWorldScale.x / 2.f
		&& vWorldPos.y - vWorldScale.y / 2.f < _vPos.y && _vPos.y < vWorldPos.y + vWorldScale.y / 2.f)
	{
		return true;
	}
	return false;
}

float CTransform::GetMaxScale()
{
	Vector3 vWorldScale = GetWorldScale();
	float fMax = max(vWorldScale.x, vWorldScale.y);
	fMax = max(fMax, vWorldScale.z);
	return fMax;
}

void CTransform::LookAt(const Vector3& _vLook)
{
	Vector3 vFront = _vLook;
	vFront.Normalize();

	Vector3 vRight = Vector3::Up.Cross(_vLook);
	vRight.Normalize();

	Vector3 vUp = vFront.Cross(vRight);
	vUp.Normalize();

	Matrix matRot = XMMatrixIdentity();

	matRot.Right(vRight);
	matRot.Up(vUp);
	matRot.Front(vFront);

	m_vLocalRot = DecomposeRotMat(matRot);

	// 방향벡터(우, 상, 전) 갱신하기	
	Matrix matRotate = XMMatrixRotationX(m_vLocalRot.x);
	matRotate *= XMMatrixRotationY(m_vLocalRot.y);
	matRotate *= XMMatrixRotationZ(m_vLocalRot.z);

	for (UINT i = 0; i < (UINT)DIR_TYPE::END; ++i)
	{
		m_vLocalDir[i] = XMVector3TransformNormal(vAsis[i], matRotate);
		m_vLocalDir[i].Normalize();
		m_vWorldDir[i] = m_vLocalDir[i];
	}
}

void CTransform::SaveToScene(FILE* _pFile)
{
	UINT iType = (UINT)GetComponentType();
	fwrite(&iType, sizeof(UINT), 1, _pFile);

	fwrite(&m_vLocalPos, sizeof(Vector3), 1, _pFile);
	fwrite(&m_vLocalScale, sizeof(Vector3), 1, _pFile);
	fwrite(&m_vLocalRot, sizeof(Vector3), 1, _pFile);
}

void CTransform::LoadFromScene(FILE* _pFile)
{
	fread(&m_vLocalPos, sizeof(Vector3), 1, _pFile);
	fread(&m_vLocalScale, sizeof(Vector3), 1, _pFile);
	fread(&m_vLocalRot, sizeof(Vector3), 1, _pFile);
}