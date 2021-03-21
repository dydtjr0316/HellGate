#pragma once
#include "Component.h"


class CTransform :
	public CComponent
{
private:
	Vector3		m_vLocalPos;	// GameObject 좌표
	Vector3		m_vLocalScale;	// GameObject 크기
	Vector3		m_vLocalRot;	// GameObject 회전량

	Vector3		m_vLocalDir[(UINT)DIR_TYPE::END]; // GameObject 의 Front, Up, Right 방향
	Vector3		m_vWorldDir[(UINT)DIR_TYPE::END]; // GameObject 의 Front, Up, Right 방향

	Matrix		m_matWorld;
	Matrix		m_matWorldInv;

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
	void SetLocalScale(const Vector3& _vScale) { if (m_vLocalScale != _vScale) { m_vLocalScale = _vScale; Changed(); } }
	void SetLocalRot(const Vector3& _vRot) { if (m_vLocalRot != _vRot) { m_vLocalRot = _vRot; Changed(); } }

	bool IsCasting(const Vector3& _vPos);
	float GetMaxScale();

	// Transform 정보를 상수데이터 및 레지스터로 전달한다.
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

