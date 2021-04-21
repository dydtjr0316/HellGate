#pragma once
#include "Entity.h"

class CComponent;
class CTransform;
class CMeshRender;
class CCamera;
class CScript;
class CTerrain;
class CLight3D;

class CCollider;



class CGameObject :
	public CEntity
{
private:
	CComponent*				m_arrCom[(UINT)COMPONENT_TYPE::END];
	vector<CScript*>		m_vecScript;
	vector<CGameObject*>	m_vecChild;
	CGameObject*			m_pParentObj;

	int						m_iLayerIdx;

	bool					m_bDead;
	bool					m_bActive;
	bool					m_bFrustumCheck;

public:
	void awake();
	void start();
	void update();
	void lateupdate();
	void finalupdate();
	void enable();
	void disable();

	void render();

public:
	void SetActive( bool _bTrue );
	bool IsActive() { return m_bActive; }

	void FrustumCheck( bool _bCheck ) { m_bFrustumCheck = _bCheck; }
	bool GetFrustumCheck() { return m_bFrustumCheck; }

public:
	void AddComponent( CComponent* _pCom );
	CComponent* GetComponent( COMPONENT_TYPE _eType ) { return m_arrCom[(UINT)_eType]; }
	CMeshRender* MeshRender() { return (CMeshRender*)GetComponent( COMPONENT_TYPE::MESHRENDER ); }
	CTransform* Transform() { return (CTransform*)GetComponent( COMPONENT_TYPE::TRANSFORM ); }
	CCamera* Camera() { return (CCamera*)m_arrCom[(UINT)COMPONENT_TYPE::CAMERA]; }
	CLight3D* Light3D() { return (CLight3D*)m_arrCom[(UINT)COMPONENT_TYPE::LIGHT3D]; }
	CTerrain* Terrain() { return (CTerrain*)m_arrCom[(UINT)COMPONENT_TYPE::TERRAIN]; }

	CCollider* Collider() { return (CCollider*)m_arrCom[(UINT)COMPONENT_TYPE::COLLIDER]; }
	const vector<CScript*>& GetScripts() const { return m_vecScript; }


	template<typename T>
	T* GetScript();

	void AddChild( CGameObject* _pChildObj );
	bool IsAncestor( CGameObject* _pObj );
	void ClearParent( CGameObject* _pNextParent = nullptr );
	CGameObject* GetParent() { return m_pParentObj; }
	int GetLayerIdx() { return m_iLayerIdx; }
	const vector<CGameObject*>& GetChild() { return m_vecChild; }
	bool IsDead() { return m_bDead; }
	void SetDead();

	void RegisterToLayer();

public:
	CLONE( CGameObject );

public:
	CGameObject();
	virtual ~CGameObject();

	friend class CLayer;
	friend class CEventMgr;

private:
	CGameObject( const CGameObject& _origin );
	void SetLayerIdx( int _iLayerIdx ) { m_iLayerIdx = _iLayerIdx; }

};

template<typename T>
inline T* CGameObject::GetScript() {
	T* pScript = nullptr;
	for (size_t i = 0; i < m_vecScript.size(); ++i)
	{
		pScript = dynamic_cast<T*>(m_vecScript[i]);
		if (nullptr != pScript)
		{
			return pScript;
		}
	}

	return pScript;
}