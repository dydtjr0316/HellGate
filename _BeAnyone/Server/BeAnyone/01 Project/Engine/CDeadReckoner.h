#pragma once
class CPlayerScript;
class CGameObject;
class CDeadReckoner
{
public:
	CDeadReckoner() {};
	CDeadReckoner(const int& id);
	~CDeadReckoner() {};
public:
	Vector3 GetLocalPos() { return m_v3localPos; }
	float GetRotateY() { return m_fRotateY; }
	float GetSpeed() { return m_fSpeed; }

public:
	void SetLocalPos(const Vector3& local) { m_v3localPos = local; }
	void SetRotateY(const float& rotateY) { m_fRotateY = rotateY; }
	void SetSpeed(const float& speed) { m_fSpeed = speed; }
	void SetDirVec(const Vector3& dir) { m_v3DirVec = dir; }

public:
	void DeadReckoning(CGameObject* obj);
	bool isFollowing();

public:
	//void initSpline(float x[3], float y[3], float a[2], float b[2], float c[2], float d[2]);

private:
	Vector3 m_v3localPos;
	Vector3 m_v3DirVec;
	float m_fRotateY;
	float m_fSpeed;
};

