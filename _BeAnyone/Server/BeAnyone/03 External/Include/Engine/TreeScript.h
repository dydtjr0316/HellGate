#pragma once
#include "Script.h"

class CTreeScript :
	public CScript
{
private:
	bool						m_bIsAttack;
	int							m_iAttackNum;
	
	vector<Ptr<CMeshData>>		m_pStumpMeshData;
	vector<Ptr<CMeshData>>		m_pItemMeshData;

	// item Object
	CGameObject*				m_pItem1;
	CGameObject*				m_pItem2;
	CGameObject*				m_pItem3;

	float						m_iTemTime = 0.0f;
	bool						m_bFirst = true;
	bool						m_bisUp = false;
	bool						m_bisDown = false;

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

public:
	CTreeScript();
	virtual ~CTreeScript();
};

