#ifndef _FIRE
#define _FIRE

#include "value.fx"
#include "func.fx"



// ==========================
// fire shader  
// Deferred = true
// BlendState : false

// ������
// g_float_0 : frametime
// g_float_1 : padding
// g_vec4_0 : scrollspeeds
// g_vec4_1 : scales


// g_tex_0 : fire texture
// g_tex_1 : noise texture
// g_tex_2 : alpha texture
// ==========================
struct VS_INPUT
{
    float4 vPos : POSITION;
    float2 vUV : TEXCOORD;
};

struct VS_OUTPUT
{
    float4 vLocalPos : SV_POSITION;
    float2 vUV : TEXCOORD;
    float2 vUV1 : TEXCOORD1;
    float2 vUV2 : TEXCOORD2;
    float2 vUV3 : TEXCOORD3;
};

VS_OUTPUT VS_Fire(VS_INPUT _in)
{
    VS_OUTPUT output;

    //  ��� ��� ������ ��ġ���� 4����
    output.vLocalPos.w = 1.0f;

    //  ����, ��, ���� ��� ���� ���
    output.vLocalPos = mul(_in.vPos, g_matWorld);
    output.vLocalPos = mul(output.vLocalPos, g_matView);
    output.vLocalPos = mul(output.vLocalPos, g_matProj);
    
    //  �ȼ� ���̴� �ؽ��� ��ǥ ����
    output.vUV = _in.vUV;

    //  ù ��° ������ �� ���� ��ũ�Ѹ� �ӵ� ���� ���, ù ��° ������ �ؽ��� �ؽ�����ǥ ���
    output.vUV1 = (_in.vUV * g_vec4_1.x);
    output.vUV1.y = output.vUV1.y + (g_float_0 * g_vec4_0.x);

    //  �� ��° ������ �� ���� ��ũ�Ѹ� �ӵ� ���� ���, �� ��° ������ �ؽ��� �ؽ�����ǥ ���
    output.vUV2 = (_in.vUV * g_vec4_1.y);
    output.vUV2.y = output.vUV2.y + (g_float_0 * g_vec4_0.y);

    //  �� ��° ������ �� ���� ��ũ�Ѹ� �ӵ� ���� ���, �� ��° ������ �ؽ��� �ؽ�����ǥ ���
    output.vUV3 = (_in.vUV * g_vec4_1.z);
    output.vUV3.y = output.vUV3.y + (g_float_0 * g_vec4_0.z);

    return output;
}

// ===================================
// g_vec2_0 : �ְ�1
// g_vec2_1 : �ְ�2
// g_vec2_2 : �ְ�3
// g_float_2 : �ְ� ������;
// g_float_3 : �ְ� bias


struct PS_INPUT
{
    float4 vPos : SV_POSITION; 
    float2 tex : TEXCOORD0;
    float2 tex1 : TEXCOORD1;
    float2 tex2 : TEXCOORD2;
    float2 tex3 : TEXCOORD3;
};

float4 PS_Fire(PS_INPUT _in) : SV_TARGET
{
    float4 noise1;
    float4 noise2;
    float4 noise3;
    float4 finalNoise;
    float perturb;
    float2 noiseCoords;
    float4 fireColor;
    float4 alphaColor;

    //  3���� ���� �ٸ� �ؽ��� ��ǥ�� ����Ͽ� ������ ������ �ؽ��ĸ� ���ø��Ͽ� 3���� �ٸ� ������ ������
    noise1 = g_tex_1.Sample(g_sam_3, _in.tex1);
    noise2 = g_tex_1.Sample(g_sam_3, _in.tex2);
    noise3 = g_tex_1.Sample(g_sam_3, _in.tex3);

    // (0,1) -> (-1, +1)
    noise1 = (noise1 - 0.5f) * 2.0f;
    noise2 = (noise1 - 0.5f) * 2.0f;
    noise3 = (noise1 - 0.5f) * 2.0f;

    // 3�� �ٸ� �ְ� x,y ������ 3���� ������ x,y ��ǥ �ְ�
    noise1.xy = noise1.xy * g_vec2_0.xy;
    noise2.xy = noise2.xy * g_vec2_1.xy;
    noise3.xy = noise3.xy * g_vec2_2.xy;

    finalNoise = noise1 + noise2 + noise3;

    //  �ְ� ������ �� ���̾ ���� ���� �Է� �ؽ��� y��ǥ �ְ�
    //  ���� ȿ������ �Ҳ��� ���� �Ÿ��� �ؽ��ĸ� ���� �����̸� ����? �� ������
    perturb = ((1.0f - _in.tex.y) * g_float_2 + g_float_3);

    //  ���� ȭ�� ���� �ؽ��ĸ� ���ø��ϴ� �� ����� �����ǰ� �ְ� �� �ؽ��� ���ø� ��ǥ�� �����.
    noiseCoords.xy = (finalNoise.xy * perturb) + _in.tex.xy;

    //  �����Ǵ� �ְ� �� �ؽ��� ���ø� ��ǥ�� ����Ͽ� ȭ�� �ؽ��Ŀ� ���� ���ø�
    fireColor = g_tex_0.Sample(g_sam_4, noiseCoords.xy);

    // �����ǰ� �ְ� �� �ؽ�ó ���ø� ��ǥ�� ����Ͽ� ���� �ؽ�ó���� ���� ���� ���ø��մϴ�.
    // �̰��� ���� ������ ���˴ϴ�.
    // ȭ���� ���¸� ���δ� ��� Ŭ���� ���� ���¸� ����Ͽ� ȭ���� ���δ� ���� �����մϴ�.
    alphaColor = g_tex_2.Sample(g_sam_4, noiseCoords.xy);

    //  fire�� ���� ������ �Ҿ����ϰ� �ְ�ǰ� ����
    fireColor.a = alphaColor;

    return fireColor;

}
#endif