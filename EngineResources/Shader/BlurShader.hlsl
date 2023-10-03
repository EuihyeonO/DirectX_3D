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
    float4 UV : TEXCOORD;
};

OutPut Blur_VS(Input _Value)
{
    OutPut OutPutValue = (OutPut) 0;
    OutPutValue.Pos = _Value.Pos;
    OutPutValue.UV = _Value.UV;
    
    // ȭ�� ��ü ����
    return OutPutValue;
}

// ����
static float Gau[7][7] =
{
    { 0.000f, 0.000f, 0.001f, 0.001f, 0.001f, 0.000f, 0.000f },
    { 0.000f, 0.002f, 0.012f, 0.002f, 0.012f, 0.002f, 0.000f },
    { 0.001f, 0.012f, 0.068f, 0.109f, 0.068f, 0.012f, 0.001f },
    { 0.001f, 0.020f, 0.109f, 0.172f, 0.109f, 0.020f, 0.001f },
    { 0.001f, 0.012f, 0.068f, 0.109f, 0.068f, 0.012f, 0.001f },
    { 0.000f, 0.002f, 0.012f, 0.020f, 0.012f, 0.002f, 0.000f },
    { 0.000f, 0.000f, 0.001f, 0.001f, 0.001f, 0.000f, 0.000f },
};

Texture2D DiffuseTex : register(t0);
SamplerState ENGINEBASE : register(s0);

float4 Blur_PS(OutPut _Value) : SV_Target0
{
    float2 PixelSize = float2(1.0f / 180.0f, 1.0f / 180);
    // ���� UV
    float2 PixelUvCenter = _Value.UV.xy;
    float2 StartUV = _Value.UV.xy + (-PixelSize * 3.0f);
    float2 CurUV = StartUV;
    
    float4 ResultColor = (float4) 0.0f;
 
    // ���̴� for�� ���°� ���� �����ؾ� �մϴ�.
    // ����� �ƴҽ� ������ �㶧�� �ִ�.
    for (int y = 0; y < 7; ++y)
    {
        for (int x = 0; x < 7; ++x)
        {
            ResultColor += DiffuseTex.Sample(ENGINEBASE, CurUV.xy) * Gau[y][x];
            
            
            
            CurUV.x += PixelSize.x;
        }
        
        CurUV.x = StartUV.x;
        CurUV.y += PixelSize.y;
    }
    
    if (ResultColor.a <= 0.0f)
    {
        clip(-1);
    }
    ResultColor *= float4(1.0f, 0.0f, 1.0f, 1.0f);
    
    return ResultColor;
}