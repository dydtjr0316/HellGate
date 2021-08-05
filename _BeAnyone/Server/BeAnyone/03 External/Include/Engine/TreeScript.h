#pragma once
#include "Script.h"

class CTreeScript :
	public CScript
{
private:
	bool						m_bIsAttack;
	bool						m_bMakeObjects = true;
	int							m_iAttackNum;
	
	vector<Ptr<CMeshData>>		m_pStumpMeshData;
	vector<Ptr<CMeshData>>		m_pItemMeshData;

	// item Object
	vector<CGameObject*>		m_vItem;
	float						m_iTemTime = 0.0f;
	bool						m_bFirst = true;
	bool						m_bisUp = false;
	bool						m_bisDown = false;

	// Terrain
	CTerrain*					m_Terrain;

public:
	virtual void update();


public:
	CLONE(CTreeScript);

	// collision
	virtual void OnCollisionEnter(CCollider* _pOther);
	virtual void OnCollisionExit(CCollider* _pOther);
	virtual void OnCollsion(CCollider* _pOther);

	// attack
	void DestroyTree();
	void SetStumpData(Ptr<CMeshData> _pMeshData) { m_pStumpMeshData.push_back(_pMeshData); }

	// item
	void MakeItem();
	void SetItemData(Ptr<CMeshData> _pMeshData) { m_pItemMeshData.push_back(_pMeshData); };

	// Terrain
	void SetTerrain(CTerrain* _Terrain) { m_Terrain = _Terrain; }
public:
	CTreeScript();
	virtual ~CTreeScript();
};

