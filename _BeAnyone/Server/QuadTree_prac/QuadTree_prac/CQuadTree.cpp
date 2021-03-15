#include "stdafx.h"
#include "CQuadTree.h"
#include "CQuad.h"
#include "CPlayer.h"

CQuadTree::CQuadTree()
{
	m_pRoot = new CQuad(0, 0, WINDOW::WIDTH, WINDOW::HEIGHT);
}

void CQuadTree::update(const CQuad* root)
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
					root->GetInfo()[it.first] = it.second;
					// Player class 내용 작성 후 시작 

				}
			}
		}
	}



}
