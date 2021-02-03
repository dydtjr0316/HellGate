#pragma once

// �ش� �����ӿ����� ��ΰ� ���� Ű�����ؼ� ������ ���°��� �������� �Ѵ�.
// ���� ���� ����, Ű�� �ַη� �� ����
enum class KEY_TYPE
{
	KEY_UP,
	KEY_DOWN,
	KEY_LEFT,
	KEY_RIGHT,

	KEY_SPACE,
	KEY_ENTER,
	KEY_LSHIFT,
	KEY_LCTRL,
	KEY_LALT,
	KEY_ESC,

	KEY_Q,
	KEY_W,
	KEY_S,
	KEY_A,
	KEY_D,
	KEY_R,
	KEY_Z,

	KEY_LBTN,
	KEY_RBTN,

	KEY_NUM0,
	KEY_NUM1,
	KEY_NUM2,
	KEY_NUM3,
	KEY_NUM4,
	KEY_NUM5,
	KEY_NUM6,
	KEY_NUM7,
	KEY_NUM8,
	KEY_NUM9,

	KEY_END,
};

enum class KEY_STATE
{
	STATE_NONE,		// �ƹ��ϵ� ����.
	STATE_TAB,		// Ű�� ó�� ���� ����
	STATE_HOLD,		// ������ �ִ� ����
	STATE_AWAY,		// Ű�� �� ����
};

struct tKeyState
{
	KEY_STATE	eState;
	bool		bPrevPress;
};

class CKeyMgr
{
	SINGLE( CKeyMgr );
private:
	vector<tKeyState>	m_vecKey;

public:
	void init();
	void update();
};

