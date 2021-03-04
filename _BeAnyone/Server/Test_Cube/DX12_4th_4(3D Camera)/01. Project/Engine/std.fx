#include "value.fx"

#ifndef _STD
#define _STD

struct VS_INPUT
{
    float3 vPos : POSITION; // sementic (������) ���� Layout �� ����       
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
    VS_OUTPUT output = (VS_OUTPUT) 0;      
    
    //float4 vWorldPos = mul(float4(_input.vPos, 1.f), g_matWorld);
    //float4 vViewPos = mul(vWorldPos, g_matView);
    //float4 vProjPos = mul(vViewPos, g_matProj);
    
    output.vOutPos = mul(float4(_input.vPos, 1.f), g_matWVP);
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
    float fRatio = _input.vOutPos.x / 1280.f;

    if(g_int_0 == 1)
        return float4(1.f, 0.2f, 0.2f, 1.f);
    if (g_int_0 == 2)
        return float4(0.2f, 0.2f, 1.f, 1.f);
    
    return g_tex_0.Sample(g_sam_0, _input.vUV);
}


struct VS_COL_INPUT
{
    float3 vPos : POSITION; // sementic (������) ���� Layout �� ����       
    float4 vColor : COLOR;
};

struct VS_COL_OUTPUT
{
    float4 vOutPos : SV_Position;
    float4 vOutColor : COLOR;
};

// ==================
// Vertex Color Shader
// ==================
VS_COL_OUTPUT VS_Color(VS_COL_INPUT _input)
{
    VS_COL_OUTPUT output = (VS_COL_OUTPUT) 0;
    
    // ������ǥ�踦 ��ȯ�� ������ float4 4��° w ��ҿ� 1.f �� �־��ش�.
    float4 vWorldPos = mul(float4(_input.vPos, 1.f), g_matWorld);
    float4 vViewPos = mul(vWorldPos, g_matView);
    float4 vProjPos = mul(vViewPos, g_matProj);

    output.vOutPos = vProjPos;
    output.vOutColor = _input.vColor;

    return output;
}

float4 PS_Color(VS_COL_OUTPUT _input) : SV_Target
{
    return _input.vOutColor;
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
    TEX_OUTPUT output = (TEX_OUTPUT) 0;
    
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


// =================
// Collider2D Shader
// =================
TEX_OUTPUT VS_Collider2D(TEX_INPUT _input)
{
    TEX_OUTPUT output = (TEX_OUTPUT) 0;
    
    // ������ǥ�踦 ��ȯ�� ������ float4 4��° w ��ҿ� 1.f �� �־��ش�.
    float4 vWorldPos = mul(float4(_input.vPos, 1.f), g_matWorld);
    float4 vViewPos = mul(vWorldPos, g_matView);
    float4 vProjPos = mul(vViewPos, g_matProj);

    output.vOutPos = vProjPos;
    output.vUV = _input.vUV;

    return output;
}

float4 PS_Collider2D(TEX_OUTPUT _input) : SV_Target
{
    if (g_int_0)
        return float4(1.f, 0.2f, 0.f, 1.f);
    else
        return float4(0.f, 1.f, 0.f, 1.f);
}

// ==================
// Standard 2D Shader
// g_tex_0 : Default Texture
// g_tex_3 : Animation Texture
// alpha blend : true
// ==================
struct VTX_INPUT
{
    float3 vPos : POSITION;
    float2 vUV : TEXCOORD;
};

struct VTX_OUTPUT
{
    float4 vPos : SV_POSITION;
    float2 vUV : TEXCOORD;
    float3 vWorldPos : POSITION;
};

VTX_OUTPUT VS_Std2D(VTX_INPUT _input)
{
    VTX_OUTPUT output = (VTX_OUTPUT) 0.f;

    float4 vPosition = (float4) 0.f;

    if (IsAnim2D)
    {
        vPosition = mul(float4(_input.vPos, 1.f), g_matWorld);
        vPosition.xy += g_vOffset;
        vPosition = mul(vPosition, g_matView);
        vPosition = mul(vPosition, g_matProj);
    }
    else
    {
        vPosition = mul(float4(_input.vPos, 1.f), g_matWVP);
    }
       
    output.vPos = vPosition;
    output.vUV = _input.vUV;

    float3 vWorldPos = mul(float4(_input.vPos, 1.f), g_matWorld).xyz;
    vWorldPos.z = 0.f;
    output.vWorldPos = vWorldPos;

    return output;
}

float4 PS_Std2D(VTX_OUTPUT _input) : SV_Target
{
    float4 vColor = (float4) 0.f;
    return float4(1.f, 0.f, 0.f, 1.f);
    if (IsAnim2D)
    {
        float2 vUV = g_vLT + (g_vLen * _input.vUV);
        vColor = g_tex_anim2D.Sample(g_sam_0, vUV);
    }
    else
    {
        vColor = g_tex_0.Sample(g_sam_0, _input.vUV);
    }

    float3 vLightPow = float3(0.f, 0.f, 0.f);
    for (int i = 0; i < g_iLight2DCount; ++i)
    {
        float fDist = distance(_input.vWorldPos.xy, g_Light2D[i].vLightPos.xy);
        float fRatio = saturate((g_Light2D[i].fRange - fDist) / g_Light2D[i].fRange);
        vLightPow += fRatio * g_Light2D[i].vLightColor.rgb;
    }

    vColor.rgb *= vLightPow;
    
    return vColor;
}

// ==================
// 2D Shadow Shader
// g_tex_0 : Default Texture
// g_tex_3 : Animation Texture
// alpha blend : true
// ==================
VTX_OUTPUT VS_2DShadow(VTX_INPUT _input)
{
    VTX_OUTPUT output = (VTX_OUTPUT) 0.f;

    if (_input.vPos.y == 0.5f)
    {
        _input.vPos.x += 0.8f;
        _input.vPos.y -= 0.4f;
    }

    output.vPos = mul(float4(_input.vPos, 1.f), g_matWVP);
    output.vUV = _input.vUV;

    return output;
}

float4 PS_2DShadow(VTX_OUTPUT _input) : SV_Target
{
    float4 vColor = (float4) 0.f;
    
    if (IsAnim2D)
    {
        float2 vUV = g_vLT + (g_vLen * _input.vUV);
        vColor = g_tex_anim2D.Sample(g_sam_0, vUV);
    }
    else
    {
        vColor = g_tex_0.Sample(g_sam_0, _input.vUV);
    }
    return float4(0.1f, 0.1f, 0.1f, vColor.a);
}


#endif