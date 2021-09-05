#pragma once
#include "Script.h"

struct NoiseBufferType
{
	float frameTime;
	Vector4 scrollSpeeds;
	Vector4 scales;
	float padding;
};

struct DistortionBufferType
{
	Vector2 distortion1;
	Vector2 distortion2;
	Vector2 distortion3;
	float distortionScale;
	float distortionBias;
};

class CFireScript :
	public CScript
{
private:
	NoiseBufferType			m_NoiseBuffer;
	DistortionBufferType	m_DistortionBuffer;

public:
	void init();
	NoiseBufferType GetNoiseBuffer() { return m_NoiseBuffer; }
	DistortionBufferType GetDistortionBuffer() { return m_DistortionBuffer; }

public:
	virtual void update();
	virtual void OnCollisionEnter(CCollider* _pOther);

public:
	CLONE(CFireScript);

public:
	CFireScript();
	virtual ~CFireScript();
};

