#include "stdafx.h"
#include "CQuadTree.h"
#include "CNode.h"
#include "CRect.h"
#include "CPlayer.h"

CQuadTree::CQuadTree()
{
	m_pRoot = new CNode(Point(3200.f, 3200.f), 3200.f);
}

void CQuadTree::update(CNode* root)
{
	bool flag = true;
	if (root->GetChildern().size() != 0)
	{
		for (auto& obj : root->GetChildern())
		{
			if (obj.GetChildern().size() != 0)
			{
				flag = false;
				break;
			}
		}
	}

	if (flag)
	{
		int sum = 0;
		for (auto& obj : root->GetChildern())
		{
			sum += obj.GetInfo().size();
		}

		if (sum < 10)
		{
			for (auto& obj : root->GetChildern())
			{
				root->GetInfo() = obj.GetInfo();
				//root->GetInfo()->parent 설정 하기 플레이어 스크립트 필요
			}
			root->GetChildern().clear();
		}
	}
	//for (int i = 0; i < root->GetChildern().size(); ++i) { update(&root->GetChildern()[i]); }
	for (auto& obj : root->GetChildern()) { update(&obj); }
}

void CQuadTree::insert(const float& x, const float& y, CNode* root, bool dead)
{
	// 예외처리 필요한가?
	CNode* node = nullptr;
	node = search(x, y, root);

	CRect* rect = node->GetRect();

	if (node->GetInfo().size() >= MAX_PLAYER_IN_SECTOR)
	{
		if (node->GetChildern().size() == 0)
		{
			// 여기 함수로 정리하기 
			// 1
			node->GetChildern().push_back(CNode(rect->GetCenterX()-rect->GetHalfLength(), 
				rect->GetCenterZ() + rect->GetHalfLength(),
				rect->GetHalfLength()));
			// 2
			node->GetChildern().push_back(CNode(rect->GetCenterX() + rect->GetHalfLength(),
				rect->GetCenterZ() + rect->GetHalfLength(),
				rect->GetHalfLength()));
			// 3
			node->GetChildern().push_back(CNode(rect->GetCenterX() - rect->GetHalfLength(),
				rect->GetCenterZ() - rect->GetHalfLength(),
				rect->GetHalfLength()));
			// 4
			node->GetChildern().push_back(CNode(rect->GetCenterX() + rect->GetHalfLength(),
				rect->GetCenterZ() - rect->GetHalfLength(),
				rect->GetHalfLength()));
		}

		for (auto& obj : node->GetChildern())
		{
			obj.SetParent(node);
			obj.SetIteration(node->GetIteration() + 1);
		}

		for (auto& it : node->GetInfo())
		{
			for (auto& obj : node->GetChildern())
			{
				if (obj.GetRect()->ColliderRectPoint(it.second->x, it.second->z))
				{
					insert(it.second->x, it.second->z, &obj, it.second->m_bisDead);
					it.second->clean();
					break;
				}
			}
		}
		node->GetInfo().clear();
	}
	node = nullptr;
}

CNode* CQuadTree::search(const float& x, const float& y, CNode* root)
{
	if (!(root->GetRect()->ColliderRectPoint(x,y)))
	{
		cout << "out of range" << endl;
		return root;
	}
	for (auto& obj : root->GetChildern())
	{
		if (obj.GetRect()->ColliderRectPoint(x, y))
			return search(x, y, &obj);
	}
	return root;
}

