#include <iostream>

using namespace std;

void youngmun();

int main()
{
#ifdef _DEBUG
	cout << "Debug" << endl;
#endif
	cout << "�� �ǳ�" << endl;
}

void youngmun()
{
	cout << "youngmun" << endl;
}