#pragma once
#include "../Client/ServerHeader.h"

class CScene;
class CGameObject;
class CToolCamScript;

class CNetMgr
{
public:
	CNetMgr();
	~CNetMgr() { delete m_pObj; /*delete m_pCamObj;*/ }
public:		// obj객체 설정부
	void SetObj(CGameObject* obj) { m_pObj = new CGameObject; m_pObj = obj; }
	//void SetCamObj(CToolCamScript* obj) { m_pCamObj = new CToolCamScript; m_pCamObj = obj; }
	void SetCamObj(CGameObject* obj) { m_pCamObj = new CGameObject; m_pCamObj = obj; }
public:		// 네트워크 연결부
	void Connect();
public:		// 패킷 처리부
	void Recevie_Data();
	void ProcessPacket(char* ptr);
	void Process_Data(char* net_buf, size_t& io_byte);
	void Enter_Player(const int& id);
public:		// 패킷 전송부
	void Send_Packet(void* _packet);
	void Send_LogIN_Packet();
	void Send_Move_Packet(unsigned const char& dir);// 아래꺼 완성하면 지워도됨 : 용석
	void Send_Move_Packet(unsigned const char& dir, const Vector3& local, const Vector3& dirVector);
	void Send_Rotate_Packet(unsigned const char& dir, const Vector2& drag, const Vector3& rotate, const float& dt);
	void Send_Rotate_Packet(unsigned const char& dir, const Vector2& drag, const Vector3& rotate);
	void Send_Attack_Packet();
public:
	void err_quit(const char* msg);
private:
	OVERLAPPED m_overlapped;
	CGameObject* m_pObj;
	CGameObject* m_pCamObj;
	//CToolCamScript* m_pCamObj;
};

