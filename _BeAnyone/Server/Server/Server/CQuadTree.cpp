#include "stdafx.h"
#include "CQuadTree.h"
#include "CGameObject.h"
#include "CBoundary.h"


void CQuadTree::PrintQuadTree()
{
	cout << "------------------" << endl;
	cout << "DEPTH : " << m_iDepth << endl;
	cout << "------------------" << endl;
	if (!m_bisDivide)
	{
		for (auto& obj : m_vpPlayers)
		{
			CGameObject* temp = Netmgr.GetMediatorMgr()->Find(obj);
			cout << "ID : " << temp->GetID() << "\tPOS : (" << temp->GetLocalPosVector().x << ", " 
				<< temp->GetLocalPosVector().z << ")" << endl;
			cout << "------------------" << endl;

		}
	}
	else
	{
		cout << "자식이 있는 노드 입니다" << endl << endl;
		for (auto& obj : m_pChild)
		{
			obj->PrintQuadTree();
		}
	}
}

CQuadTree::CQuadTree(const CBoundary& _m_boundary, const int& n)
{
	m_boundary = _m_boundary; m_icapacity = n;
}


bool CQuadTree::Insert(CGameObject* p)
{
	if (!m_boundary.contains(p))
		return false;

	if (!m_bisDivide)
	{
		if (m_vpPlayers.size() < m_icapacity)	// leaf node 일때
		{
			m_vpPlayers.emplace(p->GetID());
		}
		else
		{
			Sub_Divide();
			for (auto& obj : m_pChild)
				if (obj->Insert(p))return true;
		}
	}
	else
	{
		for (auto& obj : m_pChild)
			if (obj->Insert(p))return true;
	}
	//cout << endl;
	//cout << "Insert**********************" << endl;
	//PrintQuadTree();
	//return true;// 여기서 true 반환하는거 괜찮은지 보기
}

void CQuadTree::Delete(CGameObject* p)
{
	// 해당 플레이어 id로 현재위치한 노드를 찾는다 
	// // 자식노드가 없는 노드중 플레이어 좌표를 취하는곳
	int cnt = 0;
	this;

	if (!m_bisDivide && m_boundary.contains(p))		// player p를 포함하는 리프 노드인가?
	{
		//cout << "Test 출력" << endl;
		//cout << m_boundary.GetX() << " - " << m_boundary.GetZ() << " - "
		//	<< m_boundary.GetW() << " - " << m_boundary.GetH() << endl;
		// 노드에서 해당 플레이어를 뺀다
		for (auto& playerID : m_vpPlayers)
		{
			if (IsSameObject(playerID, p->GetID()))
			{
				m_vpPlayers.erase(playerID);
				break;
			}
		}
		// if 해당노드가 속한 부모노드에 있는 모든 자식노드의 플레이어 갯수가 4개인지 판단


		// 4개이상이면 패스
		// 4개이하라면 4개이상인 부모노드를 찾을 때 까지 재귀호출
		for (auto& obj : m_pParent->GetChild())
		{
			cnt += obj->GetPoint().size();
		}
		if (cnt <= 4)
		{
			for (auto& obj : m_pParent->GetChild())
			{
				for (auto& sub : obj->GetPoint())	//0629 이근처 부분 뒤죽박죽임 m_vplayer를쓸거면 그거만 쓰는걸로 정리하기 존나 헷갈리네 ㅅㅂ
				{
					m_pParent->GetPoint().emplace(sub);
					if (m_pParent->m_bisDivide)
						m_pParent->m_bisDivide = false;
				}
			}

		}
		//m_pParent->m_bisDivide = false;
	}
	else
	{
		//if (m_pChild.size() ==0)return;
		for (auto& childNode : m_pChild)
		{
			childNode->Delete(p);
		}

	}
	//cout << endl;
	//cout << "Delete********************" << endl;
	//PrintQuadTree();
}
bool CQuadTree::IsSameObject(const uShort& p1, const uShort& p2)
{
	if (p1 == p2)return true;
	else return false;
}
void CQuadTree::Sub_Divide()
{
	CBoundary childRect[CHILD_NODE_SIZE];//용석
	childRect[0] = CBoundary(m_boundary.GetX() + m_boundary.GetW() / 2, m_boundary.GetZ() - m_boundary.GetH() / 2,
		m_boundary.GetW() / 2, m_boundary.GetH() / 2);
	childRect[1] = CBoundary(m_boundary.GetX() - m_boundary.GetW() / 2, m_boundary.GetZ() - m_boundary.GetH() / 2,
		m_boundary.GetW() / 2, m_boundary.GetH() / 2);
	childRect[2] = CBoundary(m_boundary.GetX() + m_boundary.GetW() / 2, m_boundary.GetZ() + m_boundary.GetH() / 2,
		m_boundary.GetW() / 2, m_boundary.GetH() / 2);
	childRect[3] = CBoundary(m_boundary.GetX() - m_boundary.GetW() / 2, m_boundary.GetZ() + m_boundary.GetH() / 2,
		m_boundary.GetW() / 2, m_boundary.GetH() / 2);
	for (int i = 0; i < CHILD_NODE_SIZE; i++)
	{
		CQuadTree* temp = new CQuadTree(childRect[i], MAX_PLAYER_IN_NODE);
		temp->SetParent(this);
		m_pChild.push_back(temp);
	}
	for (auto& obj : m_pChild)
	{
		obj->m_iDepth = this->m_iDepth + 1;
	}
	m_bisDivide = true;
	SubDivideToChild();
}
void CQuadTree::SubDivideToChild()
{
	for (auto& player : m_vpPlayers)
	{
		// 여기서 부모에있던 플레이어정보 좌표에 따라 자식으로 넘겨주고 
		// 부모노드의 player정보는 삭제하는 코드가 있어야함
		for (auto& childNode : m_pChild)
		{
			if (childNode->m_boundary.contains(Netmgr.GetMediatorMgr()->Find(player)))
			{
				/*m_vpPlayers.erase(player);*/
				childNode->m_vpPlayers.emplace(player);
				break;
			}
		}
	}
	if (m_bisDivide)
		m_vpPlayers.clear();
}




unordered_set<uShort> CQuadTree::search(const CBoundary& range)
{
	//  쿼드트리 부모 자식 구조 바꾸면서 이부분 안바꿔도 되는지 확인해 볼 것
	unordered_set<uShort> found;
	{
		/*if (!m_boundary.intersects(range))
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
	CBoundary temp = range;
	if (m_boundary.intersects(temp))
	{
		for (auto& p : m_vpPlayers)
		{
			if (temp.contains(Netmgr.GetMediatorMgr()->Find(p)))
				found.emplace(p);
		}
	}
	if (m_bisDivide)
	{
		for (auto& obj : m_pChild)
		{
			for (auto player : obj->search(range))
			{
				found.emplace(player);
			}
		}
	}
	cout << "Search****************************" << endl;
	for (auto& obj : found)
	{
		cout <<"ID : "<< obj << endl;
	}
	cout << "---------------------------" << endl;

	return found;

}

