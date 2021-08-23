#pragma once
#include "Entity.h"
#include <iostream>
using namespace std;

class CComponent;
class CTransform;
class CMeshRender;
class CCollider;
class CAnimator3D;
class CScript;
class CLight;
class CCamera;
class CTerrain;
class CStaticUI;
class CQuest;
class CParticleSystem;
typedef unsigned short uShort;
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

	uShort					m_iID;

	//0506
	//CToolCamScript*			m_CamObj;
	CGameObject*			m_pCamObj;

	// UI
	bool					m_bUiRenderCheck = true;
public:
	void awake();
	void start();
	void update();
	void lateupdate();
	void finalupdate();
	void enable();
	void disable();

public:
	void SetActive(bool _bTrue);
	bool IsActive() { return m_bActive; }

	void FrustumCheck(bool _bCheck) { m_bFrustumCheck = _bCheck; }
	bool GetFrustumCheck() { return m_bFrustumCheck; }

	// UI
	void SetUiRenderCheck(bool _bCheck) { m_bUiRenderCheck = _bCheck; }
	bool GetUiRenderCheck() { return m_bUiRenderCheck; }


public:
	void AddComponent(CComponent* _pCom);
	CComponent* GetComponent(COMPONENT_TYPE _eType) { assert(_eType != COMPONENT_TYPE::SCRIPT); return m_arrCom[(UINT)_eType]; }
	CTransform* Transform() { return (CTransform*)GetComponent(COMPONENT_TYPE::TRANSFORM); }		// 다운 캐스팅
	CMeshRender* MeshRender() { return (CMeshRender*)GetComponent(COMPONENT_TYPE::MESHRENDER); }	// 다운 캐스팅
	CCamera* Camera() { return (CCamera*)m_arrCom[(UINT)COMPONENT_TYPE::CAMERA]; }
	CLight* Light() { return (CLight*)m_arrCom[(UINT)COMPONENT_TYPE::LIGHT]; }
	CTerrain* Terrain() { return (CTerrain*)m_arrCom[(UINT)COMPONENT_TYPE::TERRAIN];}
	CAnimator3D* Animator3D() { return (CAnimator3D*)m_arrCom[(UINT)COMPONENT_TYPE::ANIMATOR3D]; }
	CCollider* Collider() { return (CCollider*)m_arrCom[(UINT)COMPONENT_TYPE::COLLIDER]; }
	CStaticUI* StaticUI() { return (CStaticUI*)m_arrCom[(UINT)COMPONENT_TYPE::UI]; }
	CQuest* Quest() { return (CQuest*)m_arrCom[(UINT)COMPONENT_TYPE::QUEST]; }
	CParticleSystem* Particlesystem() { return (CParticleSystem*)m_arrCom[(UINT)COMPONENT_TYPE::PARTICLESYSTEM]; }


	const vector<CScript*>& GetScripts() const { return m_vecScript; }
	
	template<typename T>
	T* GetScript();

	void AddChild(CGameObject* _pChildObj);
	bool IsAncestor(CGameObject* _pObj);
	void ClearParent(CGameObject* _pNextParent = nullptr);
	CGameObject* GetParent() { return m_pParentObj; }
	int GetLayerIdx() { return m_iLayerIdx; }
	const vector<CGameObject*>& GetChild() { return m_vecChild; }
	bool IsDead() { return m_bDead; }
	void SetDead();

	void RegisterToLayer();

	//0506
	//CToolCamScript* GetCamScript() { return m_CamObj; }
	//void SetCamScript(CToolCamScript* obj) { m_CamObj = obj; }

	CGameObject* GetCam() { return m_pCamObj; }
	void SetCam(CGameObject* obj) { m_pCamObj = obj; }

	void SetID(const uShort id) { m_iID = id; }
	uShort GetID() { return m_iID; }

public:
	CLONE(CGameObject);

private:
	CGameObject(const CGameObject& _origin);
	void SetLayerIdx(int _iLayerIdx) { m_iLayerIdx = _iLayerIdx; }

public:
	CGameObject();
	virtual ~CGameObject();

	friend class CLayer;
	friend class CEventMgr;
};

template<typename T>
inline T* CGameObject::GetScript()
{
	T* pScript = nullptr;
	if (this == nullptr)return nullptr;
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
