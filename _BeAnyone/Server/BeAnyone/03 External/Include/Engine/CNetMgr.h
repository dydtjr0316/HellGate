#pragma once
class CScene;
class CGameObject;
class CNetMgr
{
public:
	CNetMgr();
	~CNetMgr() {}
public:
	void SetLoginPacket(sc_packet_login_ok* packet);
	sc_packet_login_ok* GetLoginPacket() { return m_loginPacket; }

public:
	void SetObj(CGameObject* obj) {
		m_pObj = new CGameObject;
		m_pObj = obj; 
	}

public:
	void Connect();
	// �Ʒ� �ΰ� ���� ����� const�� reference Ȯ���غ���
	void Recevie_Data();
	void Recevie_ID_Data();
	void ProcessPacket(char* ptr);
	void Process_Data(char* net_buf, size_t& io_byte);

	void Enter_Player(const int& id);


	void Send_Packet(void* _packet);

	void Send_LogIN_Packet();
	void Send_Move_Packet(unsigned const char& dir);
	void Send_Attack_Packet();

	void testX(const float& x);
	float testgetx() { return x; }
	int recvn(SOCKET s, char* buf, int len, int flags);


public:
	//void SetPos(const Vec3& _v3) { m_v3testPos = _v3; }
	//void SetPos(const float& x, const float& y, const float& z) {
	//	Vec3* temp = new Vec3(x, y, z); 
	//	m_v3testPos = static_cast<Vec3>(*temp);
	//	cout << m_v3testPos.x << endl;
	//	cout << m_v3testPos.y << endl;
	//	cout << m_v3testPos.z << endl;
	//}


public:
	void err_quit(const char* msg);
private:
	//CScene* m_pCurScene;
private:
	OVERLAPPED m_overlapped;
	//Vec3 m_v3testPos;

	CGameObject* m_pObj;

	sc_packet_login_ok* m_loginPacket;

	float x;

};
