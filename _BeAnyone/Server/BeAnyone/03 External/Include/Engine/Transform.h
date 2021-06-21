#pragma once
#include "Component.h"


class CTransform :
	public CComponent
{
private:
	Vector3		m_vLocalPos;	// GameObject ��ǥ
	Vector3		m_vLocalScale;	// GameObject ũ��
	Vector3		m_vLocalRot;	// GameObject ȸ����

	Vector3		m_vLocalDir[(UINT)DIR_TYPE::END]; // GameObject �� Front, Up, Right ����
	Vector3		m_vWorldDir[(UINT)DIR_TYPE::END]; // GameObject �� Front, Up, Right ����

	Matrix		m_matWorld;
	Matrix		m_matWorldInv;

	// 3��Ī ī�޶� �˻�
	bool m_bCamera = false;
	Vector2 m_vDrag;
	Vector3 m_vPlayerPos;
	Vector3 m_vPlayerRot;
	Matrix m_mPlayerWolrd;
	Vector3 m_vPlayerDir[(UINT)DIR_TYPE::END];

	

public:
	const Vector3& GetLocalPos() { return m_vLocalPos; }
	Vector3 GetWorldPos() { return m_matWorld.Translation(); }

	const Vector3& GetLocalScale() { return m_vLocalScale; }
	Vector3 GetWorldScale();

	const Vector3& GetLocalRot() { return m_vLocalRot; }
	const Matrix& GetWorldMat() { return m_matWorld; }

	const Vector3& GetLocalDir(DIR_TYPE _eType) { return m_vLocalDir[(UINT)_eType]; }
	const Vector3& GetWorldDir(DIR_TYPE _eType) { return m_vWorldDir[(UINT)_eType]; }

	void SetLocalPos(const Vector3& _vPos) { if (m_vLocalPos != _vPos) { m_vLocalPos = _vPos; Changed(); } }
	void SetLocalPosY(const float& _y) { if (m_vLocalPos.y != _y) { m_vLocalPos.y = _y; Changed(); } }

	void SetLocalScale(const Vector3& _vScale) { if (m_vLocalScale != _vScale) { m_vLocalScale = _vScale; Changed(); } }
	void SetLocalRot(const Vector3& _vRot) { if (m_vLocalRot != _vRot) { m_vLocalRot = _vRot; Changed(); } }
	void SetLocalRot(const float& _vRotY) { if (m_vLocalRot.y != _vRotY) { m_vLocalRot.y = _vRotY; Changed(); } }

	bool IsCasting(const Vector3& _vPos);
	float GetMaxScale();

	void LookAt(const Vector3& _vLook);

	// Transform ������ ��������� �� �������ͷ� �����Ѵ�.
	void UpdateData();

	// 3��Ī ī�޶� �˻�
	void Set3Camera(bool _bCamera) { m_bCamera = _bCamera; }
	void SetPlayerPosition(Vector3 _vPlayerPos) { m_vPlayerPos = _vPlayerPos; }
	void SetPlayerRotation(Vector3 _vPlayerRot) { m_vPlayerRot = _vPlayerRot; }
	void SetDrag(Vector2 _vDrag) { m_vDrag = _vDrag; }
	void SetPlayerWorldMat(Matrix _mPlayerWorld) { m_mPlayerWolrd = _mPlayerWorld; }
	void SetWorldDir(Vector3 _vPlayerDir, DIR_TYPE _eType) { m_vPlayerDir[(UINT)_eType] = _vPlayerDir; }

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

