#pragma once

#include "Script.h"

class CSenserScript
	: public CScript
{
private:

public:
	virtual void update();
	virtual void OnCollisionEnter(CCollider* _pOther);

public:
	CLONE(CSenserScript);

public:
	CSenserScript();
	virtual ~CSenserScript();
};

