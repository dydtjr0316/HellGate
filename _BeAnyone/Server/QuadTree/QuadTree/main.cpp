#include "stdafx.h"
#include "CQuadTree.h"
#include "CPlayer.h"
#include "CRectangle.h"
#include "CMediator.h"
uShort id = 0;
CMediator g_Medi;
void Enter() { cout << endl; }
void Print(CQuadTree* qt);
int main()
{
	system_clock::time_point start = system_clock::now();
	{
		/*CQuadTree* qt = new CQuadTree(CRectangle(3200.f, 3200.f, 3200.f, 3200.f), MAX_PLAYER_IN_NODE);
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
		unordered_set<uShort> m_vpPlayers = qt->search(look);

		for (auto& obj : m_vpPlayers)
		{
			cout << "player[" << obj->GetID() << "]  ->" << obj->GetX() << ", " << obj->GetZ() << endl;
		}*/
	}
	CQuadTree* qt = new CQuadTree(CRectangle(3200.f, 3200.f, 3200.f, 3200.f), MAX_PLAYER_IN_NODE);
	CPlayer* p;
	{
		p = new CPlayer(id, 100.f, 100.f);
		qt->insert(p);
		g_Medi.Add(p, id++);
		p = new CPlayer(id, 6000.f, 6000.f);
		qt->insert(p);
		g_Medi.Add(p, id++);
		p = new CPlayer(id, 6000.f, 1000.f);
		qt->insert(p);
		g_Medi.Add(p, id++);
		p = new CPlayer(id, 1000.f, 6000.f);
		qt->insert(p);
		g_Medi.Add(p, id++);
		qt->PrintQuadTree();
	}
	int i = 0;
	float PlayerX, PlayerY;
	CPlayer* pTemp;
	unordered_set<uShort> m_vpPlayers;
	CRectangle temp_look;
	float nodeX, nodeY;
	float sectorX, sectorY;
	uShort searchid;
	while (true)
	{
		cout << "1: Insert Player" << endl;
		cout << "2: Delete Player" << endl;
		cout << "3: Search Sector" << endl;

		cout << "User Input : ";
		cin >> i;
		switch (i)
		{
		case 1:
			cout << "Input Player Points(x, y)" << endl;
			cin >> PlayerX >> PlayerY;
			pTemp = new CPlayer(id, PlayerX, PlayerY);
			g_Medi.Add(pTemp, id++);
			qt->insert(pTemp);
			qt->PrintQuadTree();
			break;
		case 2:
			cout << "Insert id : ";
			cin >> searchid;
			if (g_Medi.Find(searchid) == nullptr)
			{
				cout << "없는 아이디 입니다." << endl;
				break;
			}
			qt->Delete(g_Medi.Find(searchid));
			g_Medi.Delete_Obj(searchid);
			qt->PrintQuadTree();
			break;
		case 3:
			cout << "Insert Points & Sector size" << endl;
			cin >> nodeX >> nodeY >> sectorX >> sectorY;
			temp_look = CRectangle(nodeX, nodeY, sectorX, sectorY);
			m_vpPlayers = qt->search(temp_look);
			for (auto& obj : m_vpPlayers)
			{
				cout << "player[" << g_Medi.Find(obj)->GetID() << "]  ->" << g_Medi.Find(obj)->GetX() << ", " << g_Medi.Find(obj)->GetZ() << endl;
			}
			break;
		default:
			cout << "default" << endl;
			break;
		}
		system("pause");
		system("cls");
	}


	system_clock::time_point end = system_clock::now();
	nanoseconds OperTime = duration_cast<nanoseconds>(end - start);
	cout << "총 수행 시간 : " << OperTime.count() << endl;
}

void Print(CQuadTree* qt)
{
	/*cout << "----------Root의 데이터-------------" << endl;
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
	cout << "-----------------------------------" << endl;*/
}
