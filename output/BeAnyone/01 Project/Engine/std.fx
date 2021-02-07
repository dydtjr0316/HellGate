
cbuffer GLOBAL_MATRIX1 : register(b0)
{
    float4 vOffset;
};

cbuffer GLOBAL_MATRIX2 : register(b1)
{
    float4 vOffset1;
};

cbuffer GLOBAL_MATRIX3 : register(b2)
{
    float4 vOffset2;
};

cbuffer GLOBAL_MATRIX4 : register(b3)
{
    float4 vOffset3;
};

cbuffer GLOBAL_DATA : register(b5)
{
    float fDT;
    float fAccTime;
    float fWidth;
    float fHeight;
};

struct VS_INPUT
{
    float3 vPos : POSITION; // sementic (지시자) 정점 Layout 과 연동       
    float4 vColor : COLOR;
};

struct VS_OUTPUT
{
    float4 vOutPos : SV_Position;
    float4 vOutColor : COLOR;
};

// ==================
// Test Vertex Shader
// ==================
VS_OUTPUT VS_Test(VS_INPUT _input)
{
    VS_OUTPUT output = (VS_OUTPUT)0;

    output.vOutPos = float4(_input.vPos, 1.f);
    output.vOutPos.x += vOffset.x;

    output.vOutColor = _input.vColor;

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

//return float4(0.f, 0.f, 1.f * fRatio, 1.f);
return _input.vOutColor;
}