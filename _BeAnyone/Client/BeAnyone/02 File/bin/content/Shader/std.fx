#include "value.fx"
#include "func.fx"

#ifndef _STD
#define _STD

struct VS_INPUT
{
    float3 vPos : POSITION; // sementic (지시자) 정점 Layout 과 연동       
    float4 vColor : COLOR;
    float2 vUV : TEXCOORD;
};

struct VS_OUTPUT
{
    float4 vOutPos : SV_Position;
    float4 vOutColor : COLOR;
    float2 vUV : TEXCOORD;
};

// ==================
// Test Vertex Shader
// ==================
VS_OUTPUT VS_Test(VS_INPUT _input)
{
    VS_OUTPUT output = (VS_OUTPUT)0;

    output.vOutPos = mul(float4(_input.vPos, 1.f), g_matWVP);
    output.vOutColor = _input.vColor;

    output.vUV = _input.vUV;

    return output;
}

// Rasterizer 
// 정점쉐이더에서 반환한 투영좌표를 통해서
// Target 버퍼에서 호출 되어야 하는 Pixel 을 찾아낸다.

// Cull Mode 옵션에 따라, 제거 할 Toplogy 를 선별한다.
// CULL_BACK(ccw)

// Pixel Shader
// Rasterizer 에서 검출한 픽셀들마다 호출 되는 함수
// 정점에서 반환한 색상값을 타겟에 출력한다.
float4 PS_Test(VS_OUTPUT _input) : SV_Target
{
    float fRatio = _input.vOutPos.x / 1280.f;

    if (g_int_0 == 1)
        return float4(1.f, 0.2f, 0.2f, 1.f);
    if (g_int_0 == 2)
        return float4(0.2f, 0.2f, 1.f, 1.f);

    return _input.vOutColor;
  //  return g_tex_0.Sample(g_sam_0, _input.vUV);
}

// ==========================
// Std3D Shader
// 
// g_tex_0 : Diffuse Texture
// g_tex_1 : Normalmap Texture
// ==========================
struct VS_STD3D_INPUT
{
    float3 vPos : POSITION;
    float2 vUV : TEXCOORD;

    float3 vTangent : TANGENT;
    float3 vNormal : NORMAL;
    float3 vBinormal : BINORMAL;
};

struct VS_STD3D_OUTPUT
{
    float4 vPosition : SV_Position;

    float3 vViewPos : POSITION;

    float3 vViewTangent : TANGENT;
    float3 vViewNormal : NORMAL;
    float3 vViewBinormal : BINORMAL;

    float2 vUV : TEXCOORD;
};

VS_STD3D_OUTPUT VS_Std3D(VS_STD3D_INPUT _in)
{
    VS_STD3D_OUTPUT output = (VS_STD3D_OUTPUT)0.f;

    output.vPosition = mul(float4(_in.vPos, 1.f), g_matWVP);

    output.vViewPos = mul(float4(_in.vPos, 1.f), g_matWV).xyz;
    output.vViewTangent = normalize(mul(float4(_in.vTangent, 0.f), g_matWV).xyz);
    output.vViewNormal = normalize(mul(float4(_in.vNormal, 0.f), g_matWV).xyz);
    output.vViewBinormal = normalize(mul(float4(_in.vBinormal, 0.f), g_matWV).xyz);
    output.vUV = _in.vUV;
    return output;
}

float4 PS_Std3D(VS_STD3D_OUTPUT _in) : SV_Target
{
    float4 vOutColor = float4(1.f, 0.f, 1.f, 1.f);

    if (tex_0)
    {
        vOutColor = g_tex_0.Sample(g_sam_0, _in.vUV);
    }

    float3 vViewNormal = _in.vViewNormal;
    // 노말맵이 있는경우
    if (tex_1)
    {
        float3 vTSNormal = g_tex_1.Sample(g_sam_0, _in.vUV).xyz;
        vTSNormal.xyz = (vTSNormal.xyz - 0.5f) * 2.f;
        float3x3 matTBN = { _in.vViewTangent, _in.vViewBinormal, _in.vViewNormal};
        vViewNormal = normalize(mul(vTSNormal, matTBN));
    }

    tLightColor tCol = (tLightColor)0.f;

    for (int i = 0; i < g_iLightCount; ++i)
    {
        tLightColor tCurCol = CalLight(i, vViewNormal, _in.vViewPos);
        tCol.vDiff += tCurCol.vDiff;
        tCol.vSpec += tCurCol.vSpec;
        tCol.vAmb += tCurCol.vAmb;
    }

    vOutColor.xyz = (tCol.vDiff.xyz * vOutColor.xyz)
                  + tCol.vSpec.xyz
                  + tCol.vAmb.xyz * vOutColor.xyz;

    return vOutColor;
}

#endif