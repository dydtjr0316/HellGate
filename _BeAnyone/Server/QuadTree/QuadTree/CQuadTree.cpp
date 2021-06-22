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
			////���ڸ��� �θ��� ���ð� �ڷ� �Ѱ��ִ� �κ��� �ʿ���

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
	// �ش� �÷��̾� id�� ������ġ�� ��带 ã�´� 
	// // �ڽĳ�尡 ���� ����� �÷��̾� ��ǥ�� ���ϴ°�
	
	if (!m_bisDivide && boundary.contains(p))		// player p�� �����ϴ� ������ ����ΰ�?
	{
		// ��忡�� �ش� �÷��̾ ����
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
	// if �ش��尡 ���� �θ��忡 �ִ� ��� �ڽĳ���� �÷��̾� ������ 4������ �Ǵ�
	if (m_pParent->GetChild().size() < MAX_PLAYER_IN_NODE)
	{

	}
	 
	// 4���̻��̸� �н�
	// 4�����϶�� 4���̻��� �θ��带 ã�� �� ���� ���ȣ��


}
bool CQuadTree::IsSameObject(const uShort& p1, const uShort& p2)
{
	if (p1 == p2)return true;
	else return false;
}
void CQuadTree::sub_Divide()
{
	CRectangle childRect[CHILD_NODE_SIZE];//�뼮
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
		// ���⼭ �θ��ִ� �÷��̾����� ��ǥ�� ���� �ڽ����� �Ѱ��ְ� 
		// �θ����� player������ �����ϴ� �ڵ尡 �־����
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
	//  ����Ʈ�� �θ� �ڽ� ���� �ٲٸ鼭 �̺κ� �ȹٲ㵵 �Ǵ��� Ȯ���� �� ��
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

