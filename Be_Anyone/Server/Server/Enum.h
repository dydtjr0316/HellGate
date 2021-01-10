#pragma once

// 확정
enum class MONSTER_TYPE { MONSTER1, MONSTER2, MONSTER3, MONSTER4, BOSS_MONSTER };

enum class KEYINPUT { UP, DOWN, LEFT, RIGHT, JUMP, /* + 조작키 */ };

enum class PLAYER_TYPE { WARRIOR, SNIPER };	// 캐릭터 타입 근접 : 원거리

// 수정 후 삭제
enum class ENUM_STATUS { ST_FREE, ST_ALLOC, ST_ACTIVE, ST_SLEEP };

// 미확정
enum class ENUMOP { OP_RECV, OP_SEND, OP_ACCEPT, OP_RAMDON_MOVE, OP_NOTIFY, OP_NPC };