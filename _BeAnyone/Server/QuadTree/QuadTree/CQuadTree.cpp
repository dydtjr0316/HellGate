#include "stdafx.h"
#include "CQuadTree.h"
#include "CPlayer.h"
#include "CRectangle.h"

CQuadTree::CQuadTree(const CRectangle& _boundary, const int& n)
{
		boundary = _boundary; m_icapacity = n;
}


bool CQuadTree::insert(CPlayer* p)
{
	if (!boundary.contains(p))
		return false;

	if (m_vpPlayers.size() < m_icapacity)
	{
		m_vpPlayers.push_back(p);
	}
	else
	{
		if (!m_bisDivide)
		{
			sub_Divide();
		}

		{
			////이자리에 부모노드 세팅과 자료 넘겨주는 부분이 필요함

		//if (m_pParent != nullptr)
		//{

		//}
		}
		for (auto& obj : m_pChild)
		{
			if (obj->insert(p))return true;
		}
	}
}

void CQuadTree::Delete(CPlayer* p)
{
	// 해당 플레이어 id로 현재위치한 노드를 찾는다 
	// // 자식노드가 없는 노드중 플레이어 좌표를 취하는곳
	
	if (!m_bisDivide && boundary.contains(p))		// player p를 포함하는 최하위 노드인가?
	{
		// 노드에서 해당 플레이어를 뺀다
		for (auto& player : m_vpPlayers)
		{
			if (IsSameObject(player->GetID(), p->GetID())) 
			{
				m_vpPlayers.push_back(player);
				break;
			}
		}
	}
	else
	{
		for (auto& childNode : m_pChild)
		{
			childNode->Delete(p);
		}
	}
	// if 해당노드가 속한 부모노드에 있는 모든 자식노드의 플레이어 갯수가 4개인지 판단
	if (m_pParent->GetChild().size() < MAX_PLAYER_IN_NODE)
	{

	}
	 
	// 4개이상이면 패스
	// 4개이하라면 4개이상인 부모노드를 찾을 때 까지 재귀호출


}
bool CQuadTree::IsSameObject(const uShort& p1, const uShort& p2)
{
	if (p1 == p2)return true;
	else return false;
}
void CQuadTree::sub_Divide()
{
	CRectangle childRect[CHILD_NODE_SIZE];//용석
	childRect[0] = CRectangle(boundary.GetX() + boundary.GetW() / 2, boundary.GetZ() - boundary.GetH() / 2,
		boundary.GetW() / 2, boundary.GetH() / 2);
	childRect[1] = CRectangle(boundary.GetX() - boundary.GetW() / 2, boundary.GetZ() - boundary.GetH() / 2,
		boundary.GetW() / 2, boundary.GetH() / 2);
	childRect[2] = CRectangle(boundary.GetX() + boundary.GetW() / 2, boundary.GetZ() + boundary.GetH() / 2,
		boundary.GetW() / 2, boundary.GetH() / 2);
	childRect[3] = CRectangle(boundary.GetX() - boundary.GetW() / 2, boundary.GetZ() + boundary.GetH() / 2,
		boundary.GetW() / 2, boundary.GetH() / 2);
	for (int i = 0;i< CHILD_NODE_SIZE;i++)
	{
		CQuadTree* temp = new CQuadTree(childRect[i], MAX_PLAYER_IN_NODE);
		m_pChild.push_back(temp);
		temp->setParent(this);
	}
	SubDivideToChild();
	m_bisDivide = true;
}
void CQuadTree::SubDivideToChild()
{
	for (auto& player : m_vpPlayers)
	{
		// 여기서 부모에있던 플레이어정보 좌표에 따라 자식으로 넘겨주고 
		// 부모노드의 player정보는 삭제하는 코드가 있어야함
		for (auto& childNode : m_pChild)
		{
			if (childNode->boundary.contains(player))
			{
				childNode->m_vpPlayers.push_back(player);
				break;
			}
		}
	}
	m_vpPlayers.clear();
}




vector<CPlayer*> CQuadTree::search(CRectangle& range)
{
	//  쿼드트리 부모 자식 구조 바꾸면서 이부분 안바꿔도 되는지 확인해 볼 것
	vector<CPlayer*> found;
	{
		/*if (!boundary.intersects(range))
		return found;
	else
	{
		for (auto& p : m_vpPlayers)
		{
			if (range.contains(p))
				found.push_back(p);
		}
	}*/
	}
	if (boundary.intersects(range))
	{
		for (auto& p : m_vpPlayers)
		{
			if (range.contains(p))
				found.push_back(p);
		}
	}
	if (m_bisDivide)
	{
		for (auto& obj : m_pChild)
		{
			for (auto player : obj->search(range))
			{
				found.push_back(player);
			}
		}
	}

	return found;

}

