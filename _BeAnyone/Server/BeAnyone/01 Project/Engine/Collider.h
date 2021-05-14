#pragma once
#include "Component.h"

#include "Ptr.h"
#include "Mesh.h"
#include "Material.h"

class CCollider :
	public CComponent
{
private:
	static			UINT  g_iColID;

private:
	Vector3			m_vOffsetPos; 
	Vector3			m_vOffsetScale;
	COLLIDER_TYPE	m_eType;
	int				m_iCollisionCount;	// 충돌 횟수

	Ptr<CMesh>      m_pColMesh;			// 충돌체를 그리기 위한 Mesh
	Ptr<CMaterial>	m_pColMtrl;			// 충돌체를 그리기 위한 Material

	Matrix			m_matColWorld;		// 충돌체 월드 행렬

	UINT			m_iColID;			// 충돌체 고유 ID 값

	BoundingBox		m_bbx;

public:
	virtual void update();
	virtual void finalupdate();
	void render();

	void OnCollisionEnter(CCollider* _pOther); // 충돌체가 처음 충돌
	void OnCollision(CCollider* _pOther);      // 충돌 중
	void OnCollisionExit(CCollider* _pOther);  // 충돌체가 서로 떨어질 때

public:
	void SetOffsetPos(const Vector3& _vPos) { m_vOffsetPos = _vPos; }
	void SetOffsetScale(const Vector3& _vScale) { m_vOffsetScale = _vScale; }

	const Vector3& GetOffsetPos() { return m_vOffsetPos; }
	const Vector3& GetOffsetScale() { return m_vOffsetScale; }
	COLLIDER_TYPE GetColliderType() { return m_eType; }

	void SetColliderType(COLLIDER_TYPE _eType);
	void SetColliderType(COLLIDER_TYPE _eType, wstring);

	UINT GetColID() { return m_iColID; }
	const Matrix& GetColliderWorldMat() { return m_matColWorld; }

public:
	BoundingBox GetBoundingBox() {		return m_bbx;	}
	void SetBoundingBox(BoundingBox _b) { m_bbx = _b; }

public:
	CLONE(CCollider);
	virtual void SaveToScene(FILE* _pFile);
	virtual void LoadFromScene(FILE* _pFile);

public:
	void operator = (const CCollider& _other);

public:
	CCollider();
	CCollider(const CCollider& _other);
	virtual ~CCollider();
};