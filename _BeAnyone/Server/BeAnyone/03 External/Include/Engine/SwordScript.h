#pragma once
#include "Script.h"

class CSwordScript :
	public CScript
{
private:
	CStructuredBuffer*			m_pBoneFinalMat;
	vector<Ptr<CMeshData>>		m_pWeaponeMeshData;

public:
	virtual void update();
	void SetBoneFinalMat(CStructuredBuffer* _BoneFinalMat) { m_pBoneFinalMat = _BoneFinalMat; }
	void SetWeaponeData(Ptr<CMeshData> _pMeshData) { m_pWeaponeMeshData.push_back(_pMeshData); }
	void SetMeshData(WEAPONE_TYPE _eType);

	void BoneMatrender();

public:
	CLONE(CSwordScript);

public:
	CSwordScript();
	virtual ~CSwordScript();
};

