#pragma once

union tColID
{
	struct {
		UINT iColID1;
		UINT iColID2;
	};
	DWORD_PTR ID;
};

class CCollider;
class CLayer;

class CCollisionMgr
{
	SINGLE(CCollisionMgr);
private:
	UINT					m_LayerCheck[MAX_LAYER];
	map<DWORD_PTR, bool>	m_mapCol;

public:
	void init();
	void update();

public:
	void CheckCollisionLayer(const wstring& _strLayerName1, const wstring& _strLayerName2);
	void CheckCollisionLayer(int _iLayerIdx1, int _iLyaerIdx2);

private:
	void CollisionLayer(const CLayer* _pLayer1, const CLayer* _pLayer2);
	bool IsCollision(CCollider* _pCollider1, CCollider* _pCollider2);

	

private:
	bool CollisionSphere(CCollider* _pCollider1, CCollider* _pCollider2);
	bool CollisionBox(CCollider* _pCollider1, CCollider* _pCollider2);

private:
	bool CollisionRect(CCollider* _pCollider1, CCollider* _pCollider2);
	bool CollisionCircle(CCollider* _pCollider1, CCollider* _pCollider2);
	bool CollisionRectCircle(CCollider* _pCollider1, CCollider* _pCollider2);

public:
	void SetBoundingSphereCollisionMesh(float, wstring );
	void SetBoundingCubeCollisionMesh(Vector4* , wstring );

};

