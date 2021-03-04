#include "stdafx.h"
#include "Layer.h"

#include "SceneMgr.h"
#include "Scene.h"
#include "GameObject.h"

CLayer::CLayer()
{
}

CLayer::~CLayer()
{
	Safe_Delete_Vector(m_vecParentObj);
}

void CLayer::AddGameObject(CGameObject * _pObject, bool _bMoveAll)
{	
	if (!_pObject->GetParent())
	{
		// �ֻ��� ������Ʈ
		m_vecParentObj.push_back(_pObject);

		// �ٸ� ���̾��� �־�����
		if (-1 != _pObject->GetLayerIdx())
		{
			CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
			pCurScene->GetLayer(_pObject->GetLayerIdx())->RemoveParentObj(_pObject);
		}
	}

	_pObject->SetLayerIdx(m_iLayerIdx);
	
	// �ʺ�켱 Ž��
	list<CGameObject*> queue;

	const vector<CGameObject*>& vecChild = _pObject->GetChild();
	queue.insert(queue.end(), vecChild.begin(), vecChild.end());

	while (!queue.empty())
	{
		CGameObject* pChild = queue.front();
		queue.pop_front();

		const vector<CGameObject*>& vecChild = pChild->GetChild();
		queue.insert(queue.end(), vecChild.begin(), vecChild.end());

		// Layer Index ���� ����
		if (_bMoveAll)
		{
			pChild->SetLayerIdx(m_iLayerIdx);
		}
		else
		{
			if (pChild->GetLayerIdx() == -1)
			{
				pChild->SetLayerIdx(m_iLayerIdx);
			}
		}
	}
}

void CLayer::CheckParentObj()
{
	vector<CGameObject*>::iterator iter = m_vecParentObj.begin();

	for (; iter != m_vecParentObj.end();)
	{
		if ((*iter)->GetParent())
		{
			iter = m_vecParentObj.erase(iter);
		}
		else
		{
			++iter;
		}
	}
}

void CLayer::RemoveParentObj(CGameObject * _pObject)
{
	vector<CGameObject*>::iterator iter = m_vecParentObj.begin();
	for (; iter != m_vecParentObj.end(); ++iter)
	{
		if ((*iter) == _pObject)
		{
			m_vecParentObj.erase(iter);
			return;
		}
	}

	assert(nullptr);
}

void CLayer::awake()
{
	for (UINT i = 0; i < m_vecParentObj.size(); ++i)
	{
		if(m_vecParentObj[i]->IsActive())
			m_vecParentObj[i]->awake();
	}
}

void CLayer::start()
{
	for (UINT i = 0; i < m_vecParentObj.size(); ++i)
	{
		if (m_vecParentObj[i]->IsActive())
			m_vecParentObj[i]->start();
	}
}

void CLayer::update()
{
	for (UINT i = 0; i < m_vecParentObj.size(); ++i)
	{
		if (m_vecParentObj[i]->IsActive())
			m_vecParentObj[i]->update();
	}	
}

void CLayer::lateupdate()
{
	for (UINT i = 0; i < m_vecParentObj.size(); ++i)
	{
		if (m_vecParentObj[i]->IsActive())
			m_vecParentObj[i]->lateupdate();
	}
}

void CLayer::finalupdate()
{
	m_vecLayerObj.clear();

	vector<CGameObject*>::iterator iter = m_vecParentObj.begin();

	for (; iter != m_vecParentObj.end();)
	{
		if ((*iter)->IsActive())
		{
			// ��ü��Ͽ� ��� (���� ���� ������Ʈ ���������� �浹ó�� ������ �ϱ� ���ؼ�)
			(*iter)->finalupdate();
		}

		(*iter)->RegisterToLayer();

		if ((*iter)->IsDead())
			iter = m_vecParentObj.erase(iter);		
		else		
			++iter;
	}
}

void CLayer::render()
{
	//for (UINT i = 0; i < m_vecLayerObj.size(); ++i)
	//{
	//	m_vecLayerObj[i]->render();
	//}
}
