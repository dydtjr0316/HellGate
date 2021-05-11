#pragma once

struct tResolution
{
	float fWidth;
	float fHeight;
};

// 정점 구조체
struct VTX
{
	Vector3 vPos;
	Vector4 vColor;
	Vector2 vUV;

	Vector3 vTangent;
	Vector3 vNormal;
	Vector3 vBinormal;

	// animation
	Vector4 vWeights;		// 가중치
	Vector4 vIndices;		// 본 인덱스
};

struct tTransform
{
	Matrix matWorld;
	Matrix matView;
	Matrix matProj;

	Matrix matWV;
	Matrix matWVP;

	Matrix matWorldInv;
	Matrix matViewInv;
	Matrix matProjInv;
};

extern tTransform g_transform;

struct tMtrlParam
{
	Vector4			m_vDiff; // 색상 계수
	Vector4			m_vSpec; // 반사광 계수
	Vector4			m_vEmv;  // 발광 계수

	int				m_arrInt[(UINT)SHADER_PARAM::INT_END - (UINT)SHADER_PARAM::INT_0];
	float			m_arrFloat[(UINT)SHADER_PARAM::FLOAT_END - (UINT)SHADER_PARAM::FLOAT_0];
	Vector2			m_arrVec2[(UINT)SHADER_PARAM::VEC2_END - (UINT)SHADER_PARAM::VEC2_0];
	Vector4			m_arrVec4[(UINT)SHADER_PARAM::VEC4_END - (UINT)SHADER_PARAM::VEC4_0];
	Matrix			m_arrMat[(UINT)SHADER_PARAM::MATRIX_END - (UINT)SHADER_PARAM::MATRIX_0];

	int				m_iArrTex[(UINT)SHADER_PARAM::TEX_END - (UINT)SHADER_PARAM::TEX_0];
};

struct tLightColor
{
	Vector4 vDiff;
	Vector4 vSpec;
	Vector4 vAmb;
};

struct tLight
{
	tLightColor tColor;
	Vector4		vLightPos;
	Vector4		vLightDir;
	int			iLightType;
	float		fRange;
	float		fAngle;
	int			iPadding;
};

struct tLightInfo
{
	tLight arrLight[100];
	UINT	 iCurCount;
	Vector3     vPadding;
};


//================
// Struct of FBX 
//=================
typedef struct _tagMtrlInfo
{
	Vector4	vMtrlDiff;
	Vector4	vMtrlSpec;
	Vector4	vMtrlAmb;
	Vector4	vMtrlEmiv;
	_tagMtrlInfo()
		: vMtrlDiff(1.f, 1.f, 1.f, 1.f)
		, vMtrlSpec(1.f, 1.f, 1.f, 1.f)
		, vMtrlAmb(1.f, 1.f, 1.f, 1.f)
		, vMtrlEmiv(1.f, 1.f, 1.f, 1.f)
	{}

}tMtrlInfo;

typedef struct _tagFbxMat
{
	tMtrlInfo	tMtrl;
	wstring     strMtrlName;
	wstring     strDiff;
	wstring		strNormal;
	wstring		strSpec;
}tFbxMaterial;

typedef struct _tagWeightsAndIndices
{
	int		iBoneIdx;
	double	dWeight;
}tWeightsAndIndices;

typedef struct _tagContainer
{
	wstring								strName;
	vector<Vector3>						vecPos;
	vector<Vector3>						vecTangent;
	vector<Vector3>						vecBinormal;
	vector<Vector3>						vecNormal;
	vector<Vector2>						vecUV;

	vector<Vector4>						vecIndices;
	vector<Vector4>						vecWeights;

	vector<vector<UINT>>				vecIdx;
	vector<tFbxMaterial>				vecMtrl;

	// Animation 관련 정보
	bool								bAnimation;
	vector<vector<tWeightsAndIndices>>	vecWI;

	void Resize(UINT _iSize)
	{
		vecPos.resize(_iSize);
		vecTangent.resize(_iSize);
		vecBinormal.resize(_iSize);
		vecNormal.resize(_iSize);
		vecUV.resize(_iSize);
		vecIndices.resize(_iSize);
		vecWeights.resize(_iSize);
		vecWI.resize(_iSize);
	}

}tContainer;

// Animation
struct tFrameTrans
{
	Vector4		vScale;
	Vector4		qRot;
	Vector4		vTranslate;
};

struct tMTKeyFrame
{
	double		dTime;
	int			iFrame;
	Vector3		vTranslate;
	Vector3		vScale;
	Vector4		qRot;
};

struct tMTBone
{
	wstring				strBoneName;
	int					iDepth;
	int					iParentIndx;
	Matrix				matOffset;	// Offset 행렬(뼈 -> 루트 까지의 행렬)
	Matrix				matBone;   // 이거 안씀
	vector<tMTKeyFrame>	vecKeyFrame;
};

struct tMTAnimClip
{
	wstring		strAnimName;
	int			iStartFrame;
	int			iEndFrame;
	int			iFrameLength;

	double		dStartTime;
	double		dEndTime;
	double		dTimeLength;
	float		fUpdateTime; // 이거 안씀

	FbxTime::EMode eMode;
};
