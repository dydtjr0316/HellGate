#pragma once

class CGameObject;
class CMeshData;

class CItemMgr
{
	SINGLE(CItemMgr);
private:
	vector<CGameObject*>		m_vItem;

	vector<Vector3>				m_vItemPos;
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
	void SetItemPos(Vector3 _itemPos) { m_vItemPos.push_back(_itemPos); }
	void SetTerrain(CTerrain* _Terrain) { m_Terrain = _Terrain; }
	void SetIsMake(bool _isMake) { m_bMakeItem = _isMake; }
	// item meshData
	void SetItemData(Ptr<CMeshData> _pMeshData) { m_pItemMeshData.push_back(_pMeshData); };


public:
	void MakeObject();
	void MakeItem();
};

