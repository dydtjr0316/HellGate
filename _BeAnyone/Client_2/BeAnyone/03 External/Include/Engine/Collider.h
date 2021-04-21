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
	Vec3			m_vOffsetPos; 
	Vec3			m_vOffsetScale;
	COLLIDER_TYPE	m_eType;
	int				m_iCollisionCount;	// �浹 Ƚ��

	Ptr<CMesh>      m_pColMesh;			// �浹ü�� �׸��� ���� Mesh
	Ptr<CMaterial>	m_pColMtrl;			// �浹ü�� �׸��� ���� Material

	Matrix			m_matColWorld;		// �浹ü ���� ���

	UINT			m_iColID;			// �浹ü ���� ID ��


public:
	virtual void update();
	virtual void finalupdate();
	void render();

	void OnCollisionEnter(CCollider* _pOther); // �浹ü�� ó�� �浹
	void OnCollision(CCollider* _pOther);      // �浹 ��
	void OnCollisionExit(CCollider* _pOther);  // �浹ü�� ���� ������ ��

public:
	void SetOffsetPos(const Vec3& _vPos) { m_vOffsetPos = _vPos; }
	void SetOffsetScale(const Vec3& _vScale) { m_vOffsetScale = _vScale; }

	const Vec3& GetOffsetPos() { return m_vOffsetPos; }
	const Vec3& GetOffsetScale() { return m_vOffsetScale; }
	COLLIDER_TYPE GetColliderType() { return m_eType; }

	void SetColliderType(COLLIDER_TYPE _eType);
	UINT GetColID() { return m_iColID; }
	const Matrix& GetColliderWorldMat() { return m_matColWorld; }

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