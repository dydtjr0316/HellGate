#include "value.fx"
#include "func.fx"

#ifndef _STD
#define _STD

struct VS_INPUT
{
    float3 vPos : POSITION; // sementic (������) ���� Layout �� ����       
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
// �������̴����� ��ȯ�� ������ǥ�� ���ؼ�
// Target ���ۿ��� ȣ�� �Ǿ�� �ϴ� Pixel �� ã�Ƴ���.

// Cull Mode �ɼǿ� ����, ���� �� Toplogy �� �����Ѵ�.
// CULL_BACK(ccw)

// Pixel Shader
// Rasterizer ���� ������ �ȼ��鸶�� ȣ�� �Ǵ� �Լ�
// �������� ��ȯ�� ������ Ÿ�ٿ� ����Ѵ�.
float4 PS_Test(VS_OUTPUT _input) : SV_Target
{
    float4 vOutColor;
    if (tex_0) {
        vOutColor = g_tex_0.Sample(g_sam_0, _input.vUV);
    }
    else {
        vOutColor = _input.vOutColor;
    }

   // float4 vOutColor = _input.vOutColor;
  //  float4 vOutColor = g_tex_0.Sample(g_sam_0, _input.vUV);
    float4 vNormal = g_tex_1.Sample(g_sam_0, _input.vUV);
    vNormal = vNormal * 2.f - 1.f; // ǥ�� ��ǥ������ Normal

    // ǥ�� ��ǥ�� ������ Normal ������
    // ���� ǥ�� �������� ������ ȸ�� ���
    float3x3 matTBN =
    {
        _input.vViewTangent,
        _input.vViewBinormal,
        _input.vViewNormal
    };

    // ǥ�� ��ǥ ���⿡ ����� ���ؼ� View Space ǥ������ �����´�.
    float3 vViewNormal = mul(vNormal.xyz, matTBN);

    tLightColor tCol = (tLightColor)0.f;

    for (int i = 0; i < g_iLightCount; ++i)
    {
        tLightColor tTemp = CalLight(i, vViewNormal, _input.vViewPos);
        tCol.vDiff += tTemp.vDiff;
        tCol.vSpec += tTemp.vSpec;
        tCol.vAmb += tTemp.vAmb;
    }

   // vOutColor = vOutColor * tCol.vDiff
   //              + tCol.vSpec
   //              + tCol.vAmb;

    
    //return tCol.vDiff;
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

    float4 vWeight : BLENDWEIGHT;
    float4 vIndices : BLENDINDICES;
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

    if (g_int_0)
    {
        Skinning(_in.vPos, _in.vTangent
            , _in.vBinormal, _in.vNormal
            , _in.vWeight, _in.vIndices, 0);
    }

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
    // �븻���� �ִ°��
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

// =============================
// Texture Shader
// g_tex_0 : Output Texture
// AlphaBlend = true;
// =============================
struct TEX_INPUT
{
    float3 vPos : POSITION;
    float2 vUV : TEXCOORD;
};

struct TEX_OUTPUT
{
    float4 vOutPos : SV_Position;
    float2 vUV : TEXCOORD;
};

TEX_OUTPUT VS_Tex(TEX_INPUT _input)
{
    TEX_OUTPUT output = (TEX_OUTPUT)0;

    // ������ǥ�踦 ��ȯ�� ������ float4 4��° w ��ҿ� 1.f �� �־��ش�.
    float4 vWorldPos = mul(float4(_input.vPos, 1.f), g_matWorld);
    float4 vViewPos = mul(vWorldPos, g_matView);
    float4 vProjPos = mul(vViewPos, g_matProj);

    output.vOutPos = vProjPos;
    output.vUV = _input.vUV;

    return output;
}

float4 PS_Tex(TEX_OUTPUT _input) : SV_Target
{
    float4 vColor = (float4) 0.f;

    if (tex_0)
        vColor = g_tex_0.Sample(g_sam_1, _input.vUV);
    else
        vColor = float4(1.f, 0.f, 1.f, 1.f);

    return vColor;
}
// =============
// Skybox Shader
// mesh         : sphere
// rasterizer   : CULL_FRONT
// DepthStencilState : Less_Equal ( �ִ���� 1.f �� �����ϸ�, Less ���������� ������ �� ���� )
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

    // skybox �� ������ǥ�� �����ϱ� ���ؼ�, Local ��ǥ�� Ȱ���Ѵ�.(������ �߽� == View Space ���ذ� ��ġ)
    float4 vViewPos = mul(float4(_in.vPos, 0.f), g_matView);
    float4 vProjPos = mul(vViewPos, g_matProj);

    // w ������ z ���� �������̱� ������ �̸� w ���� �����صθ�
    // � ��Ȳ������ ���̰��� 1.f �� �����ȴ�.
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

// ======================
// Grid Shader
// AlphaBlend : true
// g_int_0 : ���� �ܰ�
// g_float_0 : ī�޶� ���̿� ���� �׸����� ����
// g_float_1 : ���� �β�
// g_vec4_0  : ī�޶� ��ġ
// g_vec4_1  : Grid Color
// g_tex_0 : Position Target
// ======================
struct VTX_IN
{
    float3 vPos : POSITION;
    float2 vUV : TEXCOORD;
};

struct VTX_OUT
{
    float4 vPosition : SV_Position;
    float3 vWorldPos : POSITION;
    float2 vUV : TEXCOORD;
    float3 vViewPos : POSITION1;
};

VTX_OUT VS_Grid(VTX_IN _in)
{
    VTX_OUT output = (VTX_OUT)0.f;

    output.vPosition = mul(float4(_in.vPos, 1.f), g_matWVP);
    output.vWorldPos = mul(float4(_in.vPos, 1.f), g_matWorld).xyz;
    output.vViewPos = mul(float4(_in.vPos, 1.f), g_matWV).xyz;
    output.vUV = _in.vUV;

    return output;
}

float4 PS_Grid(VTX_OUT _in) : SV_Target
{
    //float2 vFullUV = float2(_in.vPosition.x / g_vResolution.x, _in.vPosition.y / g_vResolution.y);
    //float3 vTargetViewPos = g_tex_0.Sample(g_sam_0, vFullUV).xyz;

    //float fLen = length(vTargetViewPos);    
    //float fMax = fLen * 0.01f; // �Ÿ����� 10%�� �ִ뺸�������� ����.    

    //float fDepthRatio = 1.f;
    //if (vTargetViewPos.z != 0.f)
    //{        
    //    fDepthRatio = 1.f - saturate((length(_in.vViewPos) - length(vTargetViewPos)) / fMax);
    //}


    // ���� ���� ��ġ���� ī�޶�κ��� �ָ�    
    float fDist = abs(distance(_in.vWorldPos, g_vec4_0.xyz));
    float fThickness = g_float_1 * (fDist / abs(g_vec4_0.y)) * 2.f;

    // ���纸�� ���� �ܰ� ������ ��Ƴ���.
    int iStep = 1;
    for (int i = 0; i < g_int_0 + 1; ++i)
    {
        iStep *= 10;
    }

    if (fDist > 400.f && fDist > abs(g_vec4_0.y) * 7.f)
    {
        // �þ� �־ �Ⱥ���
        clip(-1);
    }

    float fX = abs(_in.vWorldPos.x - (float)((int)_in.vWorldPos.x / iStep) * iStep);
    float fZ = abs(_in.vWorldPos.z - (float)((int)_in.vWorldPos.z / iStep) * iStep);

    if (fThickness / 2.f < fX)
        fX = (float)iStep - fX;
    if (fThickness / 2.f < fZ)
        fZ = (float)iStep - fZ;

    float fXRatio = 0.f;
    float fZRatio = 0.f;
    float fRatio = 0.f;

    if (fX < fThickness / 2.f || fZ < fThickness / 2.f)
    {
        fXRatio = ((fThickness / 2.f) - abs(fX)) / (fThickness / 2.f);
        fZRatio = ((fThickness / 2.f) - abs(fZ)) / (fThickness / 2.f);

        fRatio = 0.f;
        if (fXRatio > fZRatio)
            fRatio = fXRatio;
        else
            fRatio = fZRatio;

        return float4(g_vec4_1.rgb, fRatio); // * fDepthRatio);
    }

    // ���� �ܰ� ����
    iStep = 1;
    for (i = 0; i < g_int_0; ++i)
    {
        iStep *= 10;
    }

    fX = abs(_in.vWorldPos.x - (float)((int)_in.vWorldPos.x / iStep) * iStep);
    fZ = abs(_in.vWorldPos.z - (float)((int)_in.vWorldPos.z / iStep) * iStep);

    if (fThickness / 2.f < fX)
        fX = (float)iStep - fX;
    if (fThickness / 2.f < fZ)
        fZ = (float)iStep - fZ;

    fXRatio = 0.f;
    fZRatio = 0.f;
    fRatio = 0.f;

    if (fX < fThickness / 2.f || fZ < fThickness / 2.f)
    {
        fXRatio = ((fThickness / 2.f) - abs(fX)) / (fThickness / 2.f);
        fZRatio = ((fThickness / 2.f) - abs(fZ)) / (fThickness / 2.f);

        fRatio = 0.f;
        if (fXRatio > fZRatio)
            fRatio = fXRatio;
        else
            fRatio = fZRatio;

        return float4(g_vec4_1.rgb, fRatio * g_float_0); // * fDepthRatio);
    }

    clip(-1);
    return float4(0.f, 0.f, 0.f, 0.f);
}


#endif