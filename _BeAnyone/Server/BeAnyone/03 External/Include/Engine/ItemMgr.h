#pragma once

class CGameObject;
class CMeshData;

class CItemMgr
{
	SINGLE(CItemMgr);
private:
	vector<CGameObject*>		m_vItem;
	vector<int>					m_vItemID;
	bool						m_bIsReserved = false;

	vector<Vector3>				m_vItemPos;	// server
	CTerrain*					m_Terrain;

	bool						m_bMakeItem = false;
	bool						m_bFirst = true;
	vector<bool>				m_bMakeFirst;

	// Item Pos 이전 개수 저장
	int							m_iOldPosNum = 0;

	// up down
	vector<bool>				m_bisUp;
	vector<bool>				m_bisDown;

	vector<Ptr<CMeshData>>		m_pItemMeshData;
public:
	void init();
	void update();

public:
	void SetTerrain(CTerrain* _Terrain) { m_Terrain = _Terrain; }
	void SetIsMake(bool _isMake) { m_bMakeItem = _isMake; }
	// item meshData
	void SetItemData(Ptr<CMeshData> _pMeshData) { m_pItemMeshData.push_back(_pMeshData); };


	// Server
	//void SetItemID();
	void SetItemObj(CGameObject* _pObject) { m_vItem.push_back(_pObject); }
	void SetReservePaket(bool _bool) { m_bIsReserved = _bool; }
	void SetItemPos(Vector3 _itemPos) { m_vItemPos.push_back(_itemPos); }	// server
	void DeleteItemObj(Vector3 _itemPos);
public:
	//void MakeObject();
	void MakeItem();
};

