#include "stdafx.h"
#include "GameObject.h"

#include "Component.h"
#include "MeshRender.h"
#include "Collider2D.h"
#include "Script.h"

#include "SceneMgr.h"
#include "Scene.h"
#include "Layer.h"

CGameObject::CGameObject()
	: m_arrCom{}
	, m_pParentObj(nullptr)
	, m_iLayerIdx(-1)
	, m_bDead(false)
	, m_bActive(true)
	, m_bFrustumCheck(true)
{
}

CGameObject::CGameObject(const CGameObject & _origin)
	: CEntity(_origin)
	, m_arrCom{}
	, m_iLayerIdx(-1)
	, m_bDead(false)
	, m_bActive(true)
	, m_bFrustumCheck(_origin.m_bFrustumCheck)
{
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (nullptr != _origin.m_arrCom[i])
		{
			AddComponent(_origin.m_arrCom[i]->Clone());
		}
	}
}

CGameObject::~CGameObject()
{
	Safe_Delete_Array(m_arrCom);
	Safe_Delete_Vector(m_vecChild);
	Safe_Delete_Vector(m_vecScript);
}

void CGameObject::AddChild(CGameObject * _pChildObj)
{
	// ���� 1
	// �ڱ��ڽ��� �ڽ��� �Ǵ� ���
	assert(this != _pChildObj);

	// ���� 2.
	// �ڽ����� ������ ������Ʈ��, �θ���� ������Ʈ�� ���
	assert(!IsAncestor(_pChildObj));

	// ���� 3.
	// �ڽ� ������Ʈ�� ������ �ٸ� �θ� ������Ʈ�� �־��ٸ�
	_pChildObj->ClearParent(this);

	// �ڽ� ������Ʈ�� Layer �� ���ԵǾ����� �ʴ� ���
	// �θ� ������Ʈ�� Layer�� �����Ѵ�.
	if (-1 == _pChildObj->m_iLayerIdx)
	{
		_pChildObj->m_iLayerIdx = m_iLayerIdx;
	}
}

bool CGameObject::IsAncestor(CGameObject * _pObj)
{
	CGameObject* pParent = m_pParentObj;

	while (nullptr != pParent)
	{
		if (pParent == _pObj)
			return true;

		pParent = pParent->m_pParentObj;
	}

	return false;
}

void CGameObject::ClearParent(CGameObject* _pNextParent)
{
	// �θ� ���� ���
	if (nullptr == m_pParentObj)
	{
		// ���� �θ� ������ ���
		if (_pNextParent)
		{
			_pNextParent->m_vecChild.push_back(this);
			m_pParentObj = _pNextParent;
		}
		else // �θ� ����, ���� �θ� ����
			return;

		// �ֻ��� �θ� ������Ʈ,
		// ���� �θ� ������ (Layer ParentList ���� ����)
		if (-1 != m_iLayerIdx)
		{
			CLayer* pCurLayer = CSceneMgr::GetInst()->GetCurScene()->GetLayer(m_iLayerIdx);
			pCurLayer->CheckParentObj();
		}
	}
	else // �θ� �ִ°��
	{
		// �θ�� ���� �ڽ��� ����
		vector<CGameObject*>::iterator iter = m_pParentObj->m_vecChild.begin();
		for (size_t i = 0; iter != m_pParentObj->m_vecChild.end(); ++iter, ++i)
		{
			if ((*iter) == m_pParentObj->m_vecChild[i])
			{
				m_pParentObj->m_vecChild.erase(iter);
				break;
			}
		}

		// ���� �θ� ������ ��� �ش� �θ�� ����.
		if (_pNextParent)
		{
			_pNextParent->m_vecChild.push_back(this);
			m_pParentObj = _pNextParent;
		}
		else // �ֻ��� ������Ʈ�� ��
		{
			m_pParentObj = nullptr;

			if (!m_bDead)
			{
				CLayer* pCurLayer = CSceneMgr::GetInst()->GetCurScene()->GetLayer(m_iLayerIdx);
				m_iLayerIdx = -1;
				pCurLayer->AddGameObject(this);
			}
		}
	}
}

void CGameObject::SetDead()
{
	m_bDead = true;

	for (size_t i = 0; i < m_vecChild.size(); ++i)
	{
		m_vecChild[i]->SetDead();
	}
}

void CGameObject::AddComponent(CComponent * _pCom)
{
	COMPONENT_TYPE eType = _pCom->GetComponentType();

	if (COMPONENT_TYPE::SCRIPT == eType)
	{
		m_vecScript.push_back((CScript*)_pCom);
	}
	else
	{
		m_arrCom[(UINT)eType] = _pCom;
	}

	_pCom->SetGameObject(this);
}

void CGameObject::awake()
{
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (nullptr != m_arrCom[i])
			m_arrCom[i]->awake();
	}

	for (size_t i = 0; i < m_vecChild.size(); ++i)
	{
		if (m_vecChild[i]->IsActive())
			m_vecChild[i]->awake();
	}

	for (size_t i = 0; i < m_vecScript.size(); ++i)
	{
		m_vecScript[i]->awake();
	}
}

void CGameObject::start()
{
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (nullptr != m_arrCom[i])
			m_arrCom[i]->start();
	}

	for (size_t i = 0; i < m_vecChild.size(); ++i)
	{
		if (m_vecChild[i]->IsActive())
			m_vecChild[i]->start();
	}

	for (size_t i = 0; i < m_vecScript.size(); ++i)
	{
		m_vecScript[i]->start();
	}
}

void CGameObject::update()
{
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (nullptr != m_arrCom[i])
			m_arrCom[i]->update();
	}

	for (size_t i = 0; i < m_vecChild.size(); ++i)
	{
		if (m_vecChild[i]->IsActive())
			m_vecChild[i]->update();
	}

	for (size_t i = 0; i < m_vecScript.size(); ++i)
	{
		m_vecScript[i]->update();
	}
}

void CGameObject::lateupdate()
{
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (nullptr != m_arrCom[i])
			m_arrCom[i]->lateupdate();
	}

	for (size_t i = 0; i < m_vecChild.size(); ++i)
	{
		if (m_vecChild[i]->IsActive())
			m_vecChild[i]->lateupdate();
	}

	for (size_t i = 0; i < m_vecScript.size(); ++i)
	{
		m_vecScript[i]->lateupdate();
	}
}

void CGameObject::finalupdate()
{
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (nullptr != m_arrCom[i])
			m_arrCom[i]->finalupdate();
	}

	for (size_t i = 0; i < m_vecChild.size(); ++i)
	{
		if (m_vecChild[i]->IsActive())
			m_vecChild[i]->finalupdate();
	}
}

void CGameObject::RegisterToLayer()
{
	// layer �� ���
	assert(-1 != m_iLayerIdx);

	CLayer* pCurLayer = CSceneMgr::GetInst()->GetCurScene()->GetLayer(m_iLayerIdx);
	pCurLayer->RegisterObj(this);

	for (size_t i = 0; i < m_vecChild.size(); ++i)
	{
		m_vecChild[i]->RegisterToLayer();
	}
}


void CGameObject::enable()
{
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (nullptr != m_arrCom[i] && m_arrCom[i]->IsActive())
			m_arrCom[i]->enable();
	}

	for (size_t i = 0; i < m_vecChild.size(); ++i)
	{
		m_vecChild[i]->enable();
	}

	for (size_t i = 0; i < m_vecScript.size(); ++i)
	{
		if (m_vecScript[i]->IsActive())
			m_vecScript[i]->enable();
	}
}

void CGameObject::disable()
{
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (nullptr != m_arrCom[i] && m_arrCom[i]->IsActive())
			m_arrCom[i]->disable();
	}

	for (size_t i = 0; i < m_vecChild.size(); ++i)
	{
		m_vecChild[i]->disable();
	}

	for (size_t i = 0; i < m_vecScript.size(); ++i)
	{
		if (m_vecScript[i]->IsActive())
			m_vecScript[i]->disable();
	}
}

void CGameObject::SetActive(bool _bTrue)
{
	if (m_bActive)
	{
		if (!_bTrue)
		{
			// ��Ȱ��ȭ
			tEvent event = {};
			event.eType = EVENT_TYPE::DEACTIVATE_GAMEOBJECT;
			event.wParam = (DWORD_PTR)this;

			CEventMgr::GetInst()->AddEvent(event);
		}
	}
	else
	{
		if (_bTrue)
		{
			// Ȱ��ȭ
			// ��Ȱ��ȭ
			tEvent event = {};
			event.eType = EVENT_TYPE::ACTIVATE_GAMEOBJECT;
			event.wParam = (DWORD_PTR)this;

			CEventMgr::GetInst()->AddEvent(event);
		}
	}
}
