#pragma once
#include "Component.h"

#include "Ptr.h"
#include "Texture.h"
#include "Material.h"
#include "Mesh.h"

class CStructuredBuffer;


class CAnimator3D : public CComponent
{
private:
	const vector<tMTBone>* m_pVecBones;
	const vector<tMTAnimClip>* m_pVecClip;

	vector<float>				m_vecClipUpdateTime;
	vector<Matrix>				m_vecFinalBoneMat; // �ؽ��Ŀ� ������ ���� ������� ���� �� ��
	int							m_iFrameCount; // 30
	double						m_dCurTime;
	int							m_iCurClip; // Ŭ�� �ε���	
	int							m_iFrameIdx; // Ŭ���� ���� ������

	Ptr<CMaterial>				m_pBoneMtrl; // BoneData Update Compute Shader
	CStructuredBuffer*			m_pBoneFinalMat;  // Ư�� �������� ���� ���
	CStructuredBuffer*			m_pBoneSwordFinalMat;  // Ư�� �������� ���� ���(����)
	bool						m_bFinalMatUpdate; // ������� ���� ���࿩��

	bool						m_bAniUse; // animator ����� ���� �� ����

public:
	void SetBones(const vector<tMTBone>* _vecBones) { m_pVecBones = _vecBones; m_vecFinalBoneMat.resize(m_pVecBones->size()); }
	void SetAnimClip(const vector<tMTAnimClip>* _vecAnimClip);
	void UpdateData();
	void SetClipTime(int _iClipIdx, float _fTime) { m_vecClipUpdateTime[_iClipIdx] = _fTime; }

	tMTAnimClip GetAnimClip(int _idx) { return m_pVecClip->at(_idx); }

	// vector<Matrix>& GetFinalBoneMat() { return m_vecFinalBoneMat; }// �� ��
	CStructuredBuffer* GetFinalBoneMat() { return m_pBoneFinalMat; }
	CStructuredBuffer* GetSwordFinalBoneMat() { return m_pBoneSwordFinalMat; }

	// animator ������ ����
	void SetAniUse(bool _aniUse) { m_bAniUse = _aniUse; }
	bool GetAniUse() { return m_bAniUse; }

private:
	void check_mesh(Ptr<CMesh> _pMesh);


public:
	virtual void awake() {};
	virtual void start() {};
	virtual void update();
	virtual void lateupdate();
	virtual void finalupdate();

public:
	virtual void SaveToScene(FILE* _pFile);
	virtual void LoadFromScene(FILE* _pFile);
	CAnimator3D* Clone() { return new CAnimator3D(*this); }

public:
	CAnimator3D();
	virtual ~CAnimator3D();
};

