#pragma once

// Ȯ��
enum class MONSTER_TYPE { MONSTER1, MONSTER2, MONSTER3, MONSTER4, BOSS_MONSTER };

enum class KEYINPUT { UP, DOWN, LEFT, RIGHT, JUMP, /* + ����Ű */ };

enum class PLAYER_TYPE { WARRIOR, SNIPER };	// ĳ���� Ÿ�� ���� : ���Ÿ�

// ���� �� ����
enum class ENUM_STATUS { ST_FREE, ST_ALLOC, ST_ACTIVE, ST_SLEEP };

// ��Ȯ��
enum class ENUMOP { OP_RECV, OP_SEND, OP_ACCEPT, OP_RAMDON_MOVE, OP_NOTIFY, OP_NPC };