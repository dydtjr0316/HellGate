#ifndef _FIRE
#define _FIRE

#include "value.fx"
#include "func.fx"



// ==========================
// fire shader  
// Deferred = true
// BlendState : false

// 노이즈
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

    //  행렬 계산 떄문에 위치벡터 4단위
    output.vLocalPos.w = 1.0f;

    //  월드, 뷰, 투영 행렬 정점 계사
    output.vLocalPos = mul(_in.vPos, g_matWorld);
    output.vLocalPos = mul(output.vLocalPos, g_matView);
    output.vLocalPos = mul(output.vLocalPos, g_matProj);
    
    //  픽셀 쉐이더 텍스쳐 좌표 저장
    output.vUV = _in.vUV;

    //  첫 번째 스케일 및 위쪽 스크롤링 속도 값을 사용, 첫 번째 노이즈 텍스쳐 텍스쳐좌표 계산
    output.vUV1 = (_in.vUV * g_vec4_1.x);
    output.vUV1.y = output.vUV1.y + (g_float_0 * g_vec4_0.x);

    //  두 번째 스케일 및 위쪽 스크롤링 속도 값을 사용, 두 번째 노이즈 텍스쳐 텍스쳐좌표 계산
    output.vUV2 = (_in.vUV * g_vec4_1.y);
    output.vUV2.y = output.vUV2.y + (g_float_0 * g_vec4_0.y);

    //  세 번째 스케일 및 위쪽 스크롤링 속도 값을 사용, 세 번째 노이즈 텍스쳐 텍스쳐좌표 계산
    output.vUV3 = (_in.vUV * g_vec4_1.z);
    output.vUV3.y = output.vUV3.y + (g_float_0 * g_vec4_0.z);

    return output;
}

// ===================================
// g_vec2_0 : 왜곡1
// g_vec2_1 : 왜곡2
// g_vec2_2 : 왜곡3
// g_float_2 : 왜곡 스케일;
// g_float_3 : 왜곡 bias


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

    //  3개의 서로 다른 텍스쳐 좌표를 사용하여 동일한 노이즈 텍스쳐를 샘플링하여 3개의 다른 노이즈 스케일
    noise1 = g_tex_1.Sample(g_sam_3, _in.tex1);
    noise2 = g_tex_1.Sample(g_sam_3, _in.tex2);
    noise3 = g_tex_1.Sample(g_sam_3, _in.tex3);

    // (0,1) -> (-1, +1)
    noise1 = (noise1 - 0.5f) * 2.0f;
    noise2 = (noise1 - 0.5f) * 2.0f;
    noise3 = (noise1 - 0.5f) * 2.0f;

    // 3개 다른 왜곡 x,y 값으로 3개의 노이즈 x,y 좌표 왜곡
    noise1.xy = noise1.xy * g_vec2_0.xy;
    noise2.xy = noise2.xy * g_vec2_1.xy;
    noise3.xy = noise3.xy * g_vec2_2.xy;

    finalNoise = noise1 + noise2 + noise3;

    //  왜곡 스케일 및 바이어스 값에 의해 입력 텍스쳐 y좌표 왜곡
    //  위쪽 효과에서 불꽃이 깜박 거리는 텍스쳐를 위로 움직이면 섭동? 이 강해짐
    perturb = ((1.0f - _in.tex.y) * g_float_2 + g_float_3);

    //  이제 화재 색상 텍스쳐를 샘플링하는 데 사용할 교란되고 왜곡 된 텍스쳐 샘플링 좌표를 만든다.
    noiseCoords.xy = (finalNoise.xy * perturb) + _in.tex.xy;

    //  섭동되는 왜곡 된 텍스쳐 샘플링 좌표를 사용하여 화재 텍스쳐에 색상 샘플링
    fireColor = g_tex_0.Sample(g_sam_4, noiseCoords.xy);

    // 교란되고 왜곡 된 텍스처 샘플링 좌표를 사용하여 알파 텍스처에서 알파 값을 샘플링합니다.
    // 이것은 불의 투명도에 사용됩니다.
    // 화스랩 상태를 감싸는 대신 클램프 샘플 상태를 사용하여 화염을 감싸는 것을 방지합니다.
    alphaColor = g_tex_2.Sample(g_sam_4, noiseCoords.xy);

    //  fire의 알파 블렌딩을 불안정하고 왜곡되게 설정
    fireColor.a = alphaColor;

    return fireColor;

}
#endif