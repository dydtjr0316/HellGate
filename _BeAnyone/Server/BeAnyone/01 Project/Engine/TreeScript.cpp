#include "pch.h"
#include "TreeScript.h"

CTreeScript::CTreeScript()
	: CScript((UINT)COMPONENT_TYPE::SCRIPT) //CScript((UINT)SCRIPT_TYPE::MONSTERSCRIPT),
{

}

CTreeScript::~CTreeScript()
{
}

void CTreeScript::update()
{

}

void CTreeScript::OnCollisionEnter(CCollider* _pOther)
{
	cout << "Tree collider" << endl;
}

void CTreeScript::OnCollisionExit(CCollider* _pOther)
{
	//cout << "³ª°¨" << endl;
}