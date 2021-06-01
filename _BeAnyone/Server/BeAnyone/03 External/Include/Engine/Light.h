#pragma once
#include "Component.h"

#include "Mesh.h"
#include "Material.h"
#include "Ptr.h"

class CLight
	: public CComponent
{
private:
	tLight	m_tLightInfo;

	Ptr<CMesh>		m_pVolumeMesh;
	Ptr<CMaterial>	m_pLightMtrl;

	int				m_iArrIdx;

	CGameObject*	m_pCamObj;

public:
	void SetLightType(LIGHT_TYPE _eType);
	void SetLightPos(const Vector3& _vPos);
	void SetDiffuseColor(const Vector3& _vDiffuse) { m_tLightInfo.tColor.vDiff = _vDiffuse; }
	void SetSpecular(const Vector3& _vSpec) { m_tLightInfo.tColor.vSpec = _vSpec; }
	void SetAmbient(const Vector3& _vAmb) { m_tLightInfo.tColor.vAmb = _vAmb; }
	void SetLightDir(const Vector3& _vDir);
	void SetLightRange(float _fRange) { m_tLightInfo.fRange = _fRange; }
	const tLight& GetLightInfo() { return m_tLightInfo; }

public:
	virtual void finalupdate();
	void render();
	void Render_Shadowmap();

	virtual void SaveToScene(FILE* _pFile);
	virtual void LoadFromScene(FILE* _pFile);

public:
	CLONE(CLight);

public:
	CLight();
	virtual ~CLight();
};

