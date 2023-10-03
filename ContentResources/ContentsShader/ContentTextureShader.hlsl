#include "../../EngineResources/Shader/Transform.fx"

// � ������ ������ ����ü�� ������ �մϴ�.
// ��� �������̰� ��� ���̰�
// �̸� �������
struct Input
{
    float4 Pos : POSITION;
    float4 UV : TEXCOORD;
};

struct OutPut
{
    // �����Ͷ������� �� ��������
    // w���� ����  ����Ʈ ���ϰ� �ȼ� �������� �������� ������ ���� �����ž�.
    float4 Pos : SV_Position;
    float4 UV : TEXCOORD0;
    float4 ClipUV : TEXCOORD1;
};

cbuffer UVData : register(b4)
{
    float MaxUV_x;
    float MAXUV_y;
    float AddTimeToUV_x;
    float AddTimeToUV_y;
}

// �������������

OutPut ContentTexture_VS(Input _Value)
{
    OutPut OutPutValue = (OutPut) 0;
	
    _Value.Pos.w = 1.0f;
    OutPutValue.Pos = mul(_Value.Pos, WorldViewProjectionMatrix);   
    
    OutPutValue.UV.x = _Value.UV.x * MaxUV_x + AddTimeToUV_x;
    OutPutValue.UV.y = _Value.UV.y * MAXUV_y + AddTimeToUV_y;
        
    return OutPutValue;
}

cbuffer ColorOption : register(b0)
{
    float4 MulColor;
    float4 PlusColor;
}

Texture2D DiffuseTex : register(t0);
SamplerState ENGINEBASE : register(s0);

struct OutColor
{
    float4 Color0;
};

float4 ContentTexture_PS(OutPut _Value) : SV_Target0
{

    float4 Color = DiffuseTex.Sample(ENGINEBASE, _Value.UV.xy);
    
    Color *= MulColor;
    Color += PlusColor;
    
    Color = saturate(Color);
    
    return Color;
}