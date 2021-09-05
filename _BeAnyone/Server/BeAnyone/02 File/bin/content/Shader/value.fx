#ifndef _VALUE
#define _VALUE

// struct
struct tLightColor
{
    float4 vDiff;
    float4 vSpec;
    float4 vAmb;
};

struct tLightInfo
{
    tLightColor tCol;
    float4      vLightPos;
    float4      vLightDir;
    int         iLightType;
    float       fRange;
    float       fAngle;
    int         ipadding;
};

// constant register 
cbuffer TRANSFORM_MATRIX : register(b0)
{
    row_major matrix g_matWorld;
    row_major matrix g_matView;
    row_major matrix g_matProj;
    row_major matrix g_matWV;
    row_major matrix g_matWVP;

    row_major matrix g_matWorldInv;
    row_major matrix g_matViewInv;
    row_major matrix g_matProjInv;
};

cbuffer MATERIAL_PARAM : register(b1)
{
    float4 g_vDiff;
    float4 g_vSpec;
    float4 g_vEmv;

    int g_int_0;
    int g_int_1;
    int g_int_2;
    int g_int_3;

    float g_float_0;
    float g_float_1;
    float g_float_2;
    float g_float_3;

    float2 g_vec2_0;
    float2 g_vec2_1;
    float2 g_vec2_2;
    float2 g_vec2_3;

    float4 g_vec4_0;
    float4 g_vec4_1;
    float4 g_vec4_2;
    float4 g_vec4_3;

    row_major float4x4 g_mat_0;
    row_major float4x4 g_mat_1;
    row_major float4x4 g_mat_2;
    row_major float4x4 g_mat_3;

    int tex_0;
    int tex_1;
    int tex_2;
    int tex_3;
};

cbuffer GLOBAL_MATRIX3 : register(b2)
{
    float4 vOffset2;
};

//cbuffer GLOBAL_MATRIX4 : register(b3)
//{
//    float4 vOffset3;
//};

cbuffer LIGHT : register(b3)
{
    tLightInfo      g_Light[100];
    int             g_iLightCount;
    int3            paddingLight;
};

cbuffer GLOBAL : register(b4)
{
    float2  g_vResolution;
    float   g_fDT;
    float   g_fAccTime;
}

Texture2D g_tex_0 : register(t0);
Texture2D g_tex_1 : register(t1);
Texture2D g_tex_2 : register(t2);
Texture2D g_tex_3 : register(t3);
Texture2D g_tex_4 : register(t4);
Texture2D g_tex_5 : register(t5);
Texture2D g_tex_6 : register(t6);

StructuredBuffer<Matrix> g_arrFinalBoneMat : register(t7);

Texture2DArray g_arrtex_0 : register(t8);
Texture2DArray g_arrtex_1 : register(t9);
Texture2DArray g_arrtex_2 : register(t10);
Texture2DArray g_arrtex_3 : register(t11);


// Sampler
SamplerState g_sam_0 : register(s0);              // anisotrophic
SamplerState g_sam_1 : register(s1);              // point
SamplerComparisonState g_sam_2 : register(s2);    // PCF
SamplerState g_sam_3 : register(s3);              // fire 1
SamplerState g_sam_4 : register(s4);              // fire 2



#endif