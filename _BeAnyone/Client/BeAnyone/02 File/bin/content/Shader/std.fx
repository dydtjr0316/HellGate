#include "value.fx"
#include "func.fx"

#ifndef _STD
#define _STD

struct VS_INPUT
{
    float3 vPos : POSITION; // sementic (지시자) 정점 Layout 과 연동       
    float4 vColor : COLOR;
    float2 vUV : TEXCOORD;
    float3 vNormal : NORMAL;
    float3 vTangent : TANGENT;
    float3 vBinormal : BINORMAL;
};

struct VS_OUTPUT
{
    float4 vOutPos : SV_Position;
    float4 vOutColor : COLOR;
    float3 vViewNormal : NORMAL;
    float3 vViewTangent : TANGENT;
    float3 vViewBinormal : BINORMAL;
    float3 vViewPos : POSITION;
    float2 vUV : TEXCOORD;
};

// ==================
// Test Vertex Shader
// ==================
VS_OUTPUT VS_Test(VS_INPUT _input)
{
    VS_OUTPUT output = (VS_OUTPUT)0;

    output.vOutPos = mul(float4(_input.vPos, 1.f), g_matWVP);
    output.vViewPos = mul(float4(_input.vPos, 1.f), g_matWV).xyz;

    output.vViewNormal = normalize(mul(float4(_input.vNormal, 0.f), g_matWV)).xyz;
    output.vViewTangent = normalize(mul(float4(_input.vTangent, 0.f), g_matWV)).xyz;
    output.vViewBinormal = normalize(mul(float4(_input.vBinormal, 0.f), g_matWV)).xyz;

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
    float4 vOutColor = g_tex_0.Sample(g_sam_0, _input.vUV);
    float4 vNormal = g_tex_1.Sample(g_sam_0, _input.vUV);
    vNormal = vNormal * 2.f - 1.f; // 표면 좌표에서의 Normal

    // 표면 좌표계 기준의 Normal 방향을
    // 현재 표면 기준으로 가져올 회전 행렬
    float3x3 matTBN =
    {
        _input.vViewTangent,
        _input.vViewBinormal,
        _input.vViewNormal
    };

    // 표면 좌표 방향에 행렬을 곱해서 View Space 표면으로 가져온다.
    float3 vViewNormal = mul(vNormal.xyz, matTBN);

    tLightColor tCol = (tLightColor)0.f;

    for (int i = 0; i < g_iLightCount; ++i)
    {
        tLightColor tTemp = CalLight(i, vViewNormal, _input.vViewPos);
        tCol.vDiff += tTemp.vDiff;
        tCol.vSpec += tTemp.vSpec;
        tCol.vAmb += tTemp.vAmb;
    }

    vOutColor = vOutColor * tCol.vDiff
                 + tCol.vSpec
                 + tCol.vAmb;

    return vOutColor;
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

struct PS_STD3D_OUTPUT
{
    float4 vTarget0 : SV_Target0; // Diffuse
    float4 vTarget1 : SV_Target1; // Normal
    float4 vTarget2 : SV_Target2; // Position
};

PS_STD3D_OUTPUT PS_Std3D(VS_STD3D_OUTPUT _in)
{
    PS_STD3D_OUTPUT output = (PS_STD3D_OUTPUT)0.f;

    if (tex_0)
        output.vTarget0 = g_tex_0.Sample(g_sam_0, _in.vUV);
    else
        output.vTarget0 = float4(1.f, 0.f, 1.f, 1.f);

    float3 vViewNormal = _in.vViewNormal;
    // 노말맵이 있는경우
    if (tex_1)
    {
        float3 vTSNormal = g_tex_1.Sample(g_sam_0, _in.vUV).xyz;
        vTSNormal.xyz = (vTSNormal.xyz - 0.5f) * 2.f;
        float3x3 matTBN = { _in.vViewTangent
                            , _in.vViewBinormal
                            , _in.vViewNormal };
        vViewNormal = normalize(mul(vTSNormal, matTBN));
    }

    output.vTarget1.xyz = vViewNormal;
    output.vTarget2.xyz = _in.vViewPos;

    return output;
}


// =============
// Skybox Shader
// mesh         : sphere
// rasterizer   : CULL_FRONT
// DepthStencilState : Less_Equal ( 최대깊이 1.f 로 설정하면, Less 비교판정에서 실패할 수 있음 )
// g_tex_0 : Output Texture
// =============
struct VS_SKY_IN
{
    float3 vPos : POSITION;
    float2 vUV : TEXCOORD;
};

struct VS_SKY_OUT
{
    float4 vPosition : SV_POSITION;
    float2 vUV : TEXCOORD;
};

VS_SKY_OUT VS_Skybox(VS_SKY_IN _in)
{
    VS_SKY_OUT output = (VS_SKY_OUT)0.f;

    // skybox 의 월드좌표를 무시하기 위해서, Local 좌표를 활용한다.(원점이 중심 == View Space 기준과 일치)
    float4 vViewPos = mul(float4(_in.vPos, 0.f), g_matView);
    float4 vProjPos = mul(vViewPos, g_matProj);

    // w 값으로 z 값을 나눌것이기 때문에 미리 w 값을 셋팅해두면
    // 어떤 상황에서도 깊이값이 1.f 로 판정된다.
    vProjPos.z = vProjPos.w;

    output.vPosition = vProjPos;
    output.vUV = _in.vUV;

    return output;
}

float4 PS_Skybox(VS_SKY_OUT _in) : SV_Target
{
    float4 vOutColor = g_tex_0.Sample(g_sam_0, _in.vUV);
    return vOutColor;
}
#endif