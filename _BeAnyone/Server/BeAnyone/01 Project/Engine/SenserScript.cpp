#include "pch.h"
#include "SenserScript.h"


CSenserScript::CSenserScript()
	: CScript((UINT)COMPONENT_TYPE::SCRIPT)
{
}

CSenserScript::~CSenserScript()
{
}

   
void CSenserScript::update()
{
}

void CSenserScript::OnCollisionEnter(CCollider* _pOther)
{
	cout << "senser ´êÀ½" << endl;
}
