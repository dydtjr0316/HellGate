#pragma once
#include "Component.h"

#include "Ptr.h"
#include "Mesh.h"
#include "Material.h"
#include "Texture.h"

class CTerrain :
	public CComponent
{
private:
	UINT			m_iFaceX;
	UINT			m_iFaceZ;
	float			m_fMaxTess;

	Ptr<CTexture>	m_pHeightMap;

	Vec2			m_vBrushScale;

	UINT			m_iBrushIdx;
	UINT			m_iTileIdx;
	
	BYTE*			m_pHeightMapPixels;

public:

	void init(Ptr<CTexture> pMap = nullptr);
	virtual void update();
	virtual void finalupdate();

	float GetHeight(float, float, bool);
	Vec2 GetScale() { return Vec2(m_iFaceX, m_iFaceZ); } 

private:

	void ChangeFaceCount( UINT _iFaceX, UINT _iFaceZ );
	void CreateTerrainMesh();
	void CreateHeightmapPixelsInfo();
	void CreateHeightmapPixelsInfo__();

	Vec2 GetCollisionPos();


public:

	virtual void SaveToScene( FILE* _pFile );
	virtual void LoadFromScene( FILE* _pFile );


public:
	CLONE( CTerrain );

public:
	CTerrain();
	virtual ~CTerrain();
};

