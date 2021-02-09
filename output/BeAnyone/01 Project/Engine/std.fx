
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
    float3 vPos : POSITION; // sementic (������) ���� Layout �� ����       
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

//return float4(0.f, 0.f, 1.f * fRatio, 1.f);
return _input.vOutColor;
}