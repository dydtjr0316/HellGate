#pragma once
#include "Component.h"

#include "Frustum.h"

enum class PROJ_TYPE
{
	ORTHGRAPHIC,
	PERSPECTIVE,
};

class CCamera :
	public CComponent
{
private:	
	CFrustum	m_frustum;

	float		m_fNear; // View Space ���� �þ� ����
	float		m_fFar;  // View Space ���� �þ� ����

	float		m_fFOV;  // ���� ���� ��, �þ߰�
	float		m_fScale; // �������� ��, ���� ����
	   
	Matrix		m_matView;		// �����
	Matrix		m_matViewInv;	// �俪���
	Matrix		m_matProj;		// �������
	Matrix		m_matProjInv;	// ���������

	PROJ_TYPE	m_eProjType;
	UINT		m_iLayerCheck; // ī�޶� ��� Layer ��Ʈ��

	 
public:	
	virtual void finalupdate();
	void render();

public:
	void SetProjType(PROJ_TYPE _eType) { m_eProjType = _eType; Changed(); }
	PROJ_TYPE GetProjType() { return m_eProjType; }

	void SetFar(float _far) { m_fFar = _far; Changed();}
	void SetScale(float _fScale) { m_fScale = _fScale; if (m_fScale < 0.01f) m_fScale = 0.01f; Changed(); }
	void SetFOV(float _fAngle) { m_fFOV = _fAngle; Changed();}

	float GetFar() { return m_fFar; }
	float GetScale() { return m_fScale; }
	float GetFOV() { return m_fFOV; }

	void SetLayerCheck(int _iLayerIdx, bool _bAdd)
	{
		assert(_iLayerIdx != -1);
		if (_bAdd)
			m_iLayerCheck |= (1 << _iLayerIdx);
		else
			m_iLayerCheck &= ~(1 << _iLayerIdx);
	}
	void SetLayerAllCheck(){m_iLayerCheck = 0xfffffff;}
	void SetLayerAllRelease() { m_iLayerCheck = 0; }


	const Matrix& GetViewMat() { return m_matView; }
	const Matrix& GetViewMatInv() { return m_matViewInv; }
	const Matrix& GetProjMat() { return m_matProj; }
	const Matrix& GetProjMatInv() { return m_matProjInv; }

public:
	CLONE(CCamera);
	virtual void SaveToScene(FILE* _pFile);
	virtual void LoadFromScene(FILE* _pFile);

	
public:
	CCamera();
	virtual ~CCamera();
};

