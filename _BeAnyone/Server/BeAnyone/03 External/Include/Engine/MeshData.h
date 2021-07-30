#pragma once

#include "FBXLoader.h"

#include "Ptr.h"
#include "Material.h"
#include "Mesh.h"

class CGameObject;


class CMeshData :
	public CResource
{
private:
	Ptr<CMesh>				m_pMesh;
	vector<Ptr<CMaterial>>	m_vecMtrl;
	wstring					m_wstring[(int)FBX_TYPE::END];

	// Animation3D
public:
	static CMeshData* LoadFromFBX(const wstring& _strFilePath, FBX_TYPE _fbxType);

	virtual void Load(const wstring& _strFilePath);
	virtual void Save(const wstring& _strFilePath);

	CGameObject* Instantiate();
	Ptr<CMesh> GetMesh() { return m_pMesh; }
	Ptr<CMaterial> GetMtrl(UINT _iSubSet = 0) { return m_vecMtrl[_iSubSet]; }

public:
	CMeshData();
	virtual ~CMeshData();
};