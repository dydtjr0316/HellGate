#pragma once
#include "Entity.h"

class CGameObject;

class CLayer :
	public CEntity
{
private:	
	vector<CGameObject*>		m_vecParentObj; // �ֻ��� �θ� ������Ʈ��
	vector<CGameObject*>		m_vecLayerObj;  // �ش� ���̾ ���� ��� ������Ʈ

	int							m_iLayerIdx;

public:
	void AddGameObject(CGameObject* _pObject, bool _bMoveAll = false);
	int GetLayerIdx() { return m_iLayerIdx; }
	const vector<CGameObject*>& GetParentObj() const { return m_vecParentObj; }
	const vector<CGameObject*>& GetObjects() const { return m_vecLayerObj; }
	void RegisterObj(CGameObject* _pObj) { m_vecLayerObj.push_back(_pObj); }
	void CheckParentObj();
	void RemoveParentObj(CGameObject* _pObject);

public:
	void awake();
	void start();
	void update();
	void lateupdate();
	void finalupdate();
	void render();




private:
	void SetLayerIdx(int _iIdx) { m_iLayerIdx = _iIdx; }


public:
	CLayer();
	virtual ~CLayer();

	friend class CScene;
};

