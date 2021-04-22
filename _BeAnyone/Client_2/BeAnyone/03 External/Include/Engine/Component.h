#pragma once
#include "Entity.h"

#include "GameObject.h"

class CComponent :
	public CEntity
{
private:
	CGameObject* m_pObject;
	COMPONENT_TYPE		m_eComType;
	bool				m_bChange;
	bool				m_bActive;

public:
	virtual void awake() {};
	virtual void start() {};
	virtual void update() {};
	virtual void lateupdate() {}
	virtual void finalupdate() {}

	virtual void enable() {}
	virtual void disable() {}

	virtual void SaveToScene( FILE* _pFile ) = 0;
	virtual void LoadFromScene( FILE* _pFile ) = 0;

public:
	void SetActive( bool _bTrue );
	bool IsActive() { return m_bActive; }

public:
	COMPONENT_TYPE GetComponentType() { return m_eComType; }
	CGameObject* GetObj() { return m_pObject; }

	CTransform* Transform() { return m_pObject->Transform(); }
	CMeshRender* MeshRender() { return m_pObject->MeshRender(); }
	CCamera* Camera() { return m_pObject->Camera(); }
	CLight3D* Light3D() { return m_pObject->Light3D(); }


private:
	void SetGameObject( CGameObject* _pObject ) { m_pObject = _pObject; }

protected:
	void Changed() { m_bChange = true; }

public:
	bool IsChanged() { return m_bChange; }

public:
	virtual CComponent* Clone() = 0;

public:
	CComponent( COMPONENT_TYPE _eType );
	virtual ~CComponent();

	friend class CGameObject;
	friend class CEventMgr;
	//friend void ComponentSetChange( CComponent*, bool );
};