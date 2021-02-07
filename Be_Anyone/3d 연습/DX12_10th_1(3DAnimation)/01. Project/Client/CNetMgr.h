#pragma once
struct TestPos {
	float x;
	float y;
	float z;
	
	TestPos() { x = 0.f, y = 0.f, z = 0.f; }
	TestPos(const TestPos& tPos) { x = tPos.x, y = tPos.y, z = tPos.z; }
	TestPos(const float& _x, const float& _y, const float& _z) { x = _x, y = _y, z = _z; }

}typedef Pos;
class CNetMgr
{
public:
	CNetMgr() {}
	~CNetMgr() {}
public:
	void Connect();
	// 아래 두개 문제 생기면 const랑 reference 확인해보기
	void Recevie_Data();
	void ProcessPacket(char* ptr);
	void Process_Data(char* net_buf, size_t& io_byte);

	void Send_Packet(void* _packet);
	void Send_LogIN_Packet();
	void Send_Move_Packet(unsigned const char &dir);
	void Send_Attack_Packet();

public:
	/*void SetPos(const Vec3& _v3) { m_v3testPos = _v3; }
	void SetPos(const float& x, const float& y, const float& z) { m_v3testPos(x, y, z); }*/


public:
	void err_quit(const char* msg);
private:
	OVERLAPPED m_overlapped;
	//Vec3 m_v3testPos;
	Pos m_Pos;

};

