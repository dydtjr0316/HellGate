#ifndef _FUNC
#define _FUNC

#include "value.fx"

tLightColor CalLight(int _iLightIdx, float3 _vViewNormal, float3 _vViewPos)
{
	tLightColor tCol = (tLightColor)0.f;

	float3 vViewLightDir = (float3) 0.f;
	float fDiffPow = 0.f;
	float fSpecPow = 0.f;
	float fRatio = 1.f;

	// Directional LIght
	if (g_Light[_iLightIdx].iLightType == 0)
	{
		// 광원의 방향
		vViewLightDir = normalize(mul(float4(g_Light[_iLightIdx].vLightDir.xyz, 0.f), g_matView).xyz);
		fDiffPow = saturate(dot(-vViewLightDir, _vViewNormal));
	}
	// Point Light
	else if (g_Light[_iLightIdx].iLightType == 1)
	{
		float3 vViewLightPos = mul(float4(g_Light[_iLightIdx].vLightPos.xyz, 1.f), g_matView).xyz;
		vViewLightDir = normalize(_vViewPos - vViewLightPos);

		fDiffPow = saturate(dot(-vViewLightDir, _vViewNormal));

		// Ratio 계산
		float fDistance = distance(_vViewPos, vViewLightPos);
		if (0.f == g_Light[_iLightIdx].fRange)
			fRatio = 0.f;
		else
			fRatio = saturate(1.f - fDistance / g_Light[_iLightIdx].fRange);
	}
	// Spot Light
	else
	{

	}

	// 반사 방향
	float3 vReflect = normalize(vViewLightDir + 2 * (dot(-vViewLightDir, _vViewNormal) * _vViewNormal));
	float3 vEye = normalize(_vViewPos);
	fSpecPow = saturate(dot(-vEye, vReflect));
	fSpecPow = pow(fSpecPow, 20);



	tCol.vDiff = fDiffPow * g_Light[_iLightIdx].tCol.vDiff * fRatio;
	tCol.vSpec = fSpecPow * g_Light[_iLightIdx].tCol.vSpec * fRatio;
	tCol.vAmb = g_Light[_iLightIdx].tCol.vAmb;

	return tCol;
}

float CalTessLevel(in float3 _vWorldCamPos, float3 _vPatchPos, float _fMin, float _fMax, float _fMaxLv) {
	float fLen = length(_vPatchPos - _vWorldCamPos);

	float fLevel = (_fMaxLv - 1.f) * ((1.f - saturate((fLen - _fMin) / (_fMax - _fMin))));
	if (fLevel == _fMaxLv - 1.f)
	{
		fLevel += 1.f;
	}

	fLevel = pow(2, fLevel);

	return fLevel;
}

#endif