#pragma once
#include "Component.h"

class CTransform :
	public CComponent

{
private:
	Vector3	m_vLocalPos;	// GameObect ��ǥ
	Vector3	m_vLocalRot;	// GameObject ũ��
	Vector3	m_vLocalScale;	// GameObject ȸ����

	Vector3	m_vLocalDir[(UINT)DIR_TYPE::END]; // GameObject �� Front, Up, Right ����
	Vector3	m_vWorldDir[(UINT)DIR_TYPE::END]; // GameObject �� Front, Up, Right ����

	XMMATRIX	m_matWorld;

public:
	void SetLocalPos(const XMFLOAT3& _vPos) { m_vLocalPos = _vPos; }
	const Vector3& GetWorldPos() { return m_vLocalPos; }

	const Vector3& GetLocalScale() { return m_vLocalScale; }
	Vector3 GetWorldScale();

	const Vector3& GetLocalRot() { return m_vLocalRot; }
	const Matrix& GetWorldMat() { return m_matWorld; }

	const Vector3& GetLocalDir(DIR_TYPE _eType) { return m_vLocalDir[(UINT)_eType]; }
	const Vector3& GetWorldDir(DIR_TYPE _eType) { return m_vWorldDir[(UINT)_eType]; }

	void SetLocalPos(const Vector3& _vPos) { if (m_vLocalPos != _vPos) { m_vLocalPos = _vPos; Changed(); } }
	void SetLocalScale(const Vector3& _vScale) { if (m_vLocalScale != _vScale) { m_vLocalScale = _vScale; Changed(); } }
	void SetLocalRot(const Vector3& _vRot) { if (m_vLocalRot != _vRot) { m_vLocalRot = _vRot; Changed(); } }

	bool IsCasting(const Vector3& _vPos);
	float GetMaxScale();

	// Transform ������ ��������� �� �������ͷ� �����Ѵ�.
	void UpdateData();

public:
	virtual void finalupdate();

	virtual void SaveToScene(FILE* _pFile);
	virtual void LoadFromScene(FILE* _pFile);

public:
	virtual CTransform* Clone() { return new CTransform(*this); }

public:
	CTransform();
	virtual ~CTransform();
};

