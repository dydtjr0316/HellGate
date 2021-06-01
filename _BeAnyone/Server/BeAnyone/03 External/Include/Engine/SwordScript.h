#pragma once
#include "Script.h"

class CSwordScript :
	public CScript
{
private:
	CStructuredBuffer* m_pBoneFinalMat;

public:
	virtual void update();
	void SetBoneFinalMat(CStructuredBuffer* _BoneFinalMat) { m_pBoneFinalMat = _BoneFinalMat; }

	void BoneMatrender();

public:
	CLONE(CSwordScript);

public:
	CSwordScript();
	virtual ~CSwordScript();
};

