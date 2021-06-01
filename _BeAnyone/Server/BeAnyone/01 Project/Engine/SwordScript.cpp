#include "pch.h"
#include "SwordScript.h"

CSwordScript::CSwordScript()
	: CScript((UINT)COMPONENT_TYPE::SCRIPT)
{
	m_pBoneFinalMat = nullptr;
}

CSwordScript::~CSwordScript()
{
}

void CSwordScript::update()
{
	
}

void CSwordScript::BoneMatrender()
{
	m_pBoneFinalMat->UpdateData(TEXTURE_REGISTER::t7);
}