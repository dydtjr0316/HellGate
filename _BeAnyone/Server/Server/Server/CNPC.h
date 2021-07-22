#pragma once
class CNPC : public CGameObject
{
public:
	CNPC() {}
	~CNPC() {}
public:
	NPC_AUTOMOVE_DIR& GetAutoDir() { return m_eDir; }

public:
	void SetAutoDir(NPC_AUTOMOVE_DIR dir) { m_eDir = dir; }

private:
	NPC_AUTOMOVE_DIR m_eDir;

};

