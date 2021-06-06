#ifndef _TERRAIN
#define _TERRAIN

#include "value.fx"
#include "func.fx"



// ==========================
// Terrain Shader
// Deferred = true
// BlendState : false

// g_int_1 : Face X 개수
// g_int_2 : Face Z 개수
// g_float_0 : Max Tess Lv
// g_vec2_0 : 높이맵의 해상도
// g_vec4_0 : 카메라 포지션

// g_tex_0 : 출력 텍스쳐
// g_tex_1 : Nomrlai 텍스쳐
// g_tex_2 : 높이맵 텍스쳐
// g_tex_3 : 맵 텍스쳐
// ==========================
struct VS_INPUT
{
    float3 vPos : POSITION;
    float2 vUV : TEXCOORD;

    float3 vTangent : TANGENT;
    float3 vNormal : NORMAL;
    float3 vBinormal : BINORMAL;
};

struct VS_OUTPUT
{
    float3 vLocalPos : POSITION;
    float2 vUV : TEXCOORD;
};

VS_OUTPUT VS_Terrain(VS_INPUT _in)
{
    VS_OUTPUT output = (VS_OUTPUT)0.f;

    output.vLocalPos = _in.vPos;
    output.vUV = _in.vUV;

    return output;
}

struct PatchTess
{
    float fInsideTess : SV_InsideTessFactor;    //  내부 분할 레벨
    float arrEdgeTess[3] : SV_TessFactor;       //  표면 분할 레벨
};

PatchTess PatchConstFunc_terrain(InputPatch<VS_OUTPUT, 3> _input, int _iPatchID : SV_PrimitiveID)
{
    PatchTess output = { 0.f, { 0.f, 0.f, 0.f } };

    float3 vCenterPos = (_input[0].vLocalPos + _input[1].vLocalPos) / 2.f;
    float3 vSidePos = float3(vCenterPos.x - (vCenterPos.x - _input[0].vLocalPos.x), vCenterPos.y, vCenterPos.z);
    float3 vUpDownPos = float3(vCenterPos.x, vCenterPos.y, vCenterPos.z - (vCenterPos.z - _input[1].vLocalPos.z));

    vCenterPos = mul(float4(vCenterPos, 1.f), g_matWorld).xyz;
    vSidePos = mul(float4(vSidePos, 1.f), g_matWorld).xyz;
    vUpDownPos = mul(float4(vUpDownPos, 1.f), g_matWorld).xyz;

    float fCenter = CalTessLevel(g_vec4_0.xyz, vCenterPos, 1000.f, 4000.f, 4.f);    //  1000, 4000, 4
    float fSide = CalTessLevel(g_vec4_0.xyz, vSidePos, 1000.f, 4000.f, 2.f);
    float fUnDown = CalTessLevel(g_vec4_0.xyz, vUpDownPos, 1000.f, 4000.f, 4.f);


    output.arrEdgeTess[0] = fUnDown; // 밑, 윗
    output.arrEdgeTess[1] = fSide; // 좌, 우  
    output.arrEdgeTess[2] = fCenter; // 빗변
    output.fInsideTess = fCenter;

    return output;
}

[domain("tri")]
//[partitioning("integer")]
[partitioning("fractional_odd")]
[outputcontrolpoints(3)]
[patchconstantfunc("PatchConstFunc_terrain")]
[maxtessfactor(64.f)]
[outputtopology("triangle_cw")]
VS_OUTPUT HS_Terrain(InputPatch<VS_OUTPUT, 3> _input, int _iVtxIdx : SV_OutputControlPointID, int _iPatchID : SV_PrimitiveID)
{
    VS_OUTPUT output = (VS_OUTPUT)0.f;

    output.vLocalPos = _input[_iVtxIdx].vLocalPos;
    output.vUV = _input[_iVtxIdx].vUV;

    return output;
}

struct DS_OUTPUT
{
    float4 vPosition : SV_Position;

    float3 vViewPos : POSITION;
    float3 vViewTangent : TANGENT;
    float3 vViewBinormal : BINORMAL;
    float3 vViewNormal : NORMAL;
    float2 vUV : TEXCOORD;
};

[domain("tri")]
DS_OUTPUT DS_Terrain(const OutputPatch<VS_OUTPUT, 3> _in, float3 _vLocation : SV_DomainLocation, PatchTess _patch)
{
    DS_OUTPUT output = (DS_OUTPUT)0.f;

    float3 vLocalPos = _in[0].vLocalPos * _vLocation[0] + _in[1].vLocalPos * _vLocation[1] + _in[2].vLocalPos * _vLocation[2];
    float2 vUV = _in[0].vUV * _vLocation[0] + _in[1].vUV * _vLocation[1] + _in[2].vUV * _vLocation[2];

    float2 vFullUV = float2(vUV.x / (float)g_int_1, vUV.y / (float)g_int_2);
    float3 vHeight = g_tex_2.SampleLevel(g_sam_0, vFullUV, 0).xyz;
    vLocalPos.y = vHeight.x;

    // 높이맵 픽셀단위 UV 간격
    float2 vUVStep = float2(1.f / g_vec2_0.x, 1.f / g_vec2_0.y);
    float2 vPosStep = float2(g_int_1 * vUVStep.x, g_int_2 * vUVStep.y);

    float fUpHeight = g_tex_2.SampleLevel(g_sam_0, float2(vFullUV.x, vFullUV.y - vUVStep.y), 0).x;
    float fDownHeight = g_tex_2.SampleLevel(g_sam_0, float2(vFullUV.x, vFullUV.y + vUVStep.y), 0).x;
    float fRightHeight = g_tex_2.SampleLevel(g_sam_0, float2(vFullUV.x + vUVStep.x, vFullUV.y), 0).x;
    float fLeftHeight = g_tex_2.SampleLevel(g_sam_0, float2(vFullUV.x - vUVStep.x, vFullUV.y), 0).x;

    float3 vLocalTangent = float3(vLocalPos.x + vPosStep.x, fRightHeight, vLocalPos.z) - float3(vLocalPos.x - vPosStep.x, fLeftHeight, vLocalPos.z);
    float3 vLocalBinormal = float3(vLocalPos.x, fUpHeight, vLocalPos.z + vPosStep.y) - float3(vLocalPos.x, fDownHeight, vLocalPos.z - vPosStep.y);

    output.vPosition = mul(float4(vLocalPos, 1.f), g_matWVP);
    output.vViewPos = mul(float4(vLocalPos, 1.f), g_matWV).xyz;
    output.vViewTangent = normalize(mul(float4(vLocalTangent, 0.f), g_matWV)).xyz;
    output.vViewBinormal = normalize(mul(float4(vLocalBinormal, 0.f), g_matWV)).xyz;
    output.vViewNormal = normalize(cross(output.vViewBinormal, output.vViewTangent));
    output.vUV = vUV;

    if (g_int_0 == 64)
        output.vViewNormal = float3(1.f, 0.f, 0.f);


    return output;
}

struct PS_OUTPUT
{
    float4 vTarget0 : SV_Target0; // Diffuse
    float4 vTarget1 : SV_Target1; // Normal
    float4 vTarget2 : SV_Target2; // Position
};

PS_OUTPUT PS_Terrain(DS_OUTPUT _in)
{
    PS_OUTPUT output = (PS_OUTPUT)0.f;

    float2 vFullUV = float2(_in.vUV.x / (float)g_int_1, _in.vUV.y / (float)g_int_2);

    float3 vViewNormal = _in.vViewNormal;

    // 노말맵이 있는경우
    if (tex_1)
    {
        float3 vTSNormal = g_tex_1.Sample(g_sam_0, _in.vUV).xyz;
        vTSNormal.xyz = (vTSNormal.xyz - 0.5f) * 2.f;
        float3x3 matTBN = { _in.vViewTangent, _in.vViewBinormal, _in.vViewNormal };
        vViewNormal = normalize(mul(vTSNormal, matTBN));
    }
    else
    {
        vViewNormal = _in.vViewNormal;
    }

    //output.vTarget0.xyz = float3(1.f, 1.f, 1.f);
    output.vTarget0.xyz = g_tex_3.Sample(g_sam_0, _in.vUV /*/ (float)g_int_1*/).xyz;
    output.vTarget1.xyz = vViewNormal;
    output.vTarget2.xyz = _in.vViewPos;

    return output;
}
#endif