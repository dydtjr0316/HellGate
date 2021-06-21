#include "stdafx.h"
#include "CQuadTree.h"
#include "CPlayer.h"
#include "CRectangle.h"
uShort id = 0;
void Enter() { cout << endl; }
void Print(CQuadTree* qt);
int main()
{
	CQuadTree* qt = new CQuadTree(CRectangle(3200.f, 3200.f, 3200.f, 3200.f), 4);
	srand((unsigned int)time(NULL));
	cout << "----------insert start-------------" << endl;
	for (int i = 0; i < 10; i++)
	{
		CPlayer *p = new CPlayer(id, (float)(rand() % 6400), (float)(rand() % 6400));
		id++;
		cout << p->GetX() << ", " << p->GetZ() << endl;
		qt->insert(p);
	}
	cout << "----------insert end-------------" << endl;

	Print(qt);

	Enter();
	cout << "----------search-------------" << endl;
	CRectangle look(4000.f, 4000.f, 1000.f, 1000.f);
	vector<CPlayer*> m_vpPlayers = qt->search(look);

	for (auto& obj : m_vpPlayers)
	{
		cout << "player[" << obj->GetID() << "]  ->" << obj->GetX() << ", " << obj->GetZ() << endl;
	}
}

void Print(CQuadTree* qt)
{
	cout << "----------Root의 데이터-------------" << endl;
	if (qt->GetPoint().size() != 0)
		for (auto& obj : qt->GetPoint())
		{
			cout << "player[" << obj->GetID() << "]  ->" << obj->GetX() << ", " << obj->GetZ() << endl;
		}
	cout << "-----------------------------------" << endl;
	
	cout << "------------자식의 데이터---------------" << endl;
	for (auto& obj : qt->GetChild())
	{
		if (obj!=nullptr)
		{
			for (auto& player : obj->GetPoint())
			{
				cout << "player[" <<player->GetID()<< "]  ->" << player->GetX() << ", " << player->GetZ() << endl;
			}
			Enter();
		}
	}
	cout << "-----------------------------------" << endl;
}
