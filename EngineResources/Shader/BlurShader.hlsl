struct Input
{
    float4 Pos : POSITION;
    float4 UV : TEXCOORD;
};

struct OutPut
{
    // 레스터라이저야 이 포지션이
    // w나눈 다음  뷰포트 곱하고 픽셀 건져낼때 쓸포지션 정보를 내가 보낸거야.
    float4 Pos : SV_Position;
    float4 UV : TEXCOORD;
};

OutPut Blur_VS(Input _Value)
{
    OutPut OutPutValue = (OutPut) 0;
    OutPutValue.Pos = _Value.Pos;
    OutPutValue.UV = _Value.UV;
    
    // 화면 전체 범위
    return OutPutValue;
}

// 대충
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
    // 현재 UV
    float2 PixelUvCenter = _Value.UV.xy;
    float2 StartUV = _Value.UV.xy + (-PixelSize * 3.0f);
    float2 CurUV = StartUV;
    
    float4 ResultColor = (float4) 0.0f;
 
    // 쉐이더 for문 쓰는건 정말 주의해야 합니다.
    // 상수가 아닐시 에러가 뜰때가 있다.
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