#include "stdafx.h"
#include "CQuadTree.h"
#include "CQuad.h"
#include "CPlayer.h"
#include"CRect.h"

CQuadTree::CQuadTree()
{
	m_pRoot = new CQuad(0, 0, WINDOW::WIDTH, WINDOW::HEIGHT);
}

void CQuadTree::update(CQuad* root)
{
	bool flag = true;
	if (root->GetChildern().size() != 0)							// 노드에 자식이 존재한다면
	{
		for (int i = 0; i < root->GetChildern().size(); ++i)		// 자식수 만큼 
		{
			if (root->GetChildern()[i].GetChildern().size() != 0)	// 자식의 자식 노드에 노드가 존재 한다면
			{
				flag = false;
				break;
			}
		}
	}

	if (flag)	//위에서 flag 연산 자식노드가 자식을 가지고 있지 않다면
	{
		int sum = 0;
		for (int i = 0; i < root->GetChildern().size(); ++i)
		{
			sum += root->GetChildern()[i].GetInfo().size();
			// 자식노드에 플레이어가 몇명인지 계산해서 더하기
		}

		if (sum < 4/*숫자 4는 노드하나에 들어갈 플레이어 수로 지정*/)
		{
			for (int i = 0; i < root->GetChildern().size(); ++i)
			{
				for (auto it : root->GetChildern()[i].GetInfo())
				{
					root->GetInfo()[it.first] = it.second; // Player class 내용 작성 후 시작 
					root->GetInfo()[it.first]->SetParent(root);
				}
			}
			root->GetChildern().clear();
		}
	}

	// 이부분 적용 할때 어떻게 할지 다시 생각해보기
	{
		// ERASING CODE HERE

		/*for (auto it = root->people.begin(); it != root->people.end(); it++) {
			if (it.value().update()) {
				queue.push_back(&it.value());
			}
		}*/
	}	

	for (int i = 0; i < root->GetChildern().size(); ++i) { update(&root->GetChildern()[i]); }
}

void CQuadTree::insert(const int& x,const int& y, CQuad* root, const bool dead)
{
	CQuad* quad = nullptr;
	try
	{
		quad = search(x, y, root);
	}
	catch (const std::exception& e)
	{
		cout << e.what() << endl;
		system("pause");
		return;
	}

	// 모든 rect 변수들 구조체로 묶을생각 한번 해보기
	// 현재는 안 묶여있는 상태
	
	// 이건 인간적으로 해야됨 일단 손가는대로
	
	tRect dest;
	
	dest.set(quad->GetRect()->GetX(), quad->GetRect()->GetY(), quad->GetRect()->GetW(), quad->GetRect()->GetH());



	quad->GetInfo()[CPlayer::m_iCounter] = new CPlayer(x, y);
	quad->GetInfo()[CPlayer::m_iCounter]->SetParent(quad);
	CPlayer::m_iCounter++;



	if (quad->GetIteration() > MAX_ITER)
	{
		quad = nullptr;
		return;
	}

	if (quad->GetInfo().size() >= 4/*한구역에 들어가는 캐릭터 숫자 다시설정해야함*/)
	{
		if (quad->GetChildern().size() == 0)
		{
			quad->GetChildern().push_back(CQuad(dest.m_ix, dest.m_iy, dest.m_iw / 2, dest.m_ih / 2));
			quad->GetChildern().push_back(CQuad(dest.m_ix + (dest.m_iw / 2), dest.m_iy, dest.m_iw / 2, dest.m_ih / 2));
			quad->GetChildern().push_back(CQuad(dest.m_ix, dest.m_iy + (dest.m_ih / 2), dest.m_iw / 2, dest.m_ih / 2));
			quad->GetChildern().push_back(CQuad(dest.m_ix + (dest.m_iw / 2), dest.m_iy + (dest.m_ih / 2), dest.m_iw / 2, dest.m_ih / 2));
		}

		for (unsigned int i = 0; i < quad->GetChildern().size(); i++)
		{
			quad->GetChildern()[i].SetParent(quad);
			quad->GetChildern()[i].SetIteration(quad->GetIteration() + 1);	// 여기 오류날지도? getiter const 해놓은거 오류나면 수정하기 
		}

		for (auto it = quad->GetInfo().begin(); it != quad->GetInfo().end(); it++)
		{
			for (int i = 0; i < quad->GetChildern().size(); i++)
			{
				if (quad->GetChildern()[i].GetRect()->CollidePoint(it->second->GetRect()->GetX(), it->second->GetRect()->GetY()))
				{
					insert(it->second->GetRect()->GetX(), it->second->GetRect()->GetY(), &quad->GetChildern()[i], it->second->GetDead());
					it->second->clean();
					break;
				}
			}
		}
		quad->GetInfo().clear();
	}
	quad = nullptr;

}

CQuad* CQuadTree::search(const int& x, const int& y, CQuad* root)
{

	if (!(root->GetRect()->CollidePoint(x, y)))
	{
		// 뭐하는 코드인가,,,?
		std::string tmp = "Out of range: ";
		std::stringstream buffer; buffer << x;
		tmp += buffer.str() + " ";
		buffer.str("");
		buffer << y;
		tmp += buffer.str() + "  SDL_Rect {";
		buffer.str("");
		buffer << root->GetRect()->GetX();
		tmp += buffer.str() + ", ";
		buffer.str("");
		buffer << root->GetRect()->GetY();
		tmp += buffer.str() + ", ";
		buffer.str("");
		buffer << root->GetRect()->GetW();
		tmp += buffer.str() + ", ";
		buffer.str("");
		buffer << root->GetRect()->GetH();
		tmp += buffer.str() + "}";
		buffer.str("");
		throw std::exception(tmp.c_str());
		return root;
	}

	
	for (int i = 0; i < root->GetChildern().size(); i++)
	{
		if (root->GetChildern()[i].GetRect()->CollidePoint(x, y))
			return search(x, y, &root->GetChildern()[i]);
	}
	return root;
}
