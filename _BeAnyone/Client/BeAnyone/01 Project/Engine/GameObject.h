#pragma once
#include "Entity.h"

class CComponent;
class CTransform;
class CMeshRender;
// class CCollider2D;
// class CCollider3D;
// class CAnimator2D;
// class CAnimator3D;
// class CScript;
// class CLight2D;
// class CLight3D;
class CCamera;

class CGameObject :
	public CEntity
{
private:
	CComponent* m_arrCom[(UINT)COMPONENT_TYPE::END];
	//vector<CScript*>		m_vecScript;
	vector<CGameObject*>	m_vecChild;
	CGameObject*			m_pParentObj;
	int						m_iLayerIdx;
	bool					m_bDead;
	bool					m_bActive;
	// bool					m_bFrustumCheck;


public:
	void awake();
	void start();
	void update();
	void lateupdate();
	void finalupdate();
	// void enable();
	// void disable();

	void render();

	//template<typename T>
	//void AddComponent(T* _pCom);

public:
	void SetActive(bool _bTrue);
	bool IsActive() { return m_bActive; }

	// void FrustumCheck(bool _bCheck) { m_bFrustumCheck = _bCheck; }
	// bool GetFrustumCheck() { return m_bFrustumCheck; }


public:
	void AddComponent(CComponent* _pCom);
	CComponent* GetComponent(COMPONENT_TYPE _eType) { return m_arrCom[(UINT)_eType]; }
	CTransform* Transform() { return (CTransform*)GetComponent(COMPONENT_TYPE::TRANSFORM); }		// 다운 캐스팅
	CMeshRender* MeshRender() { return (CMeshRender*)GetComponent(COMPONENT_TYPE::MESHRENDER); }	// 다운 캐스팅
	CCamera* Camera() { return (CCamera*)m_arrCom[(UINT)COMPONENT_TYPE::CAMERA]; }

	// const vector<CScript*>& GetScripts() const { return m_vecScript; }
	// 
	// template<typename T>
	// T* GetScript();

	void AddChild(CGameObject* _pChildObj);
	bool IsAncestor(CGameObject* _pObj);
	void ClearParent(CGameObject* _pNextParent = nullptr);
	CGameObject* GetParent() { return m_pParentObj; }
	int GetLayerIdx() { return m_iLayerIdx; }
	const vector<CGameObject*>& GetChild() { return m_vecChild; }
	bool IsDead() { return m_bDead; }
	void SetDead();

	void RegisterToLayer();

//public:
	//CLONE(CGameObject);

private:
	CGameObject(const CGameObject& _origin);
	void SetLayerIdx(int _iLayerIdx) { m_iLayerIdx = _iLayerIdx; }

public:
	CGameObject();
	virtual ~CGameObject();

	friend class CLayer;
	//friend class CEventMgr;
};