cbuffer MaskInfo : register(b0)
{
    float UV_MaskingValue;
    float3 MaskingColor;
}; 

cbuffer ColorOption : register(b1)
{
    float4 MulColor;
    float4 AddColor;
};

cbuffer UVData : register(b2)
{
    float2 MulUV;
    float2 AddUV;
};

cbuffer FadeInfo : register(b3)
{
    float Delta;
    float3 FadeColor;
};

float4 Fading(Texture2D _MaskTexture, SamplerState _Sampler, float2 _UV)
{
    float4 Color = _MaskTexture.Sample(_Sampler, _UV);

    if (Color.r <= Delta)
    {
        clip(-1);
    }
    
    float2 UVsize = (1.0f / 1600.0f, 1.0f / 900.0f);
    
    float2 BaseUV = _UV;
    float2 StartUV = float2(BaseUV.x - 2.0f * UVsize.x, BaseUV.y - 2.0f * UVsize.y);
    float2 CurUV = StartUV;
    
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            float4 CurFliterColor = _MaskTexture.Sample(_Sampler, CurUV);
            
            if (CurFliterColor.r <= Delta)
            {
                Color.rgb = float3(0.0f, 1.0f, 1.0f);
                break;
            }
    
            CurUV += float2(UVsize.x, 0);
        }
    
        CurUV = float2(StartUV.x, StartUV.y + (i + 1) * UVsize.y);
    }
    
    return Color;
}