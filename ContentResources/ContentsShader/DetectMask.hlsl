struct Input
{
    float4 Pos : POSITION;
    float4 UV : TEXCOORD;
};

struct OutPut
{
    float4 Pos : SV_Position;
    float4 UV : TEXCOORD;
};

OutPut DetectMask_VS(Input _Value)
{
    OutPut OutPutValue = (OutPut) 0;
    OutPutValue.Pos = _Value.Pos;
    OutPutValue.UV = _Value.UV;
    
    return OutPutValue;
}

Texture2D DifLightTexture : register(t0);
Texture2D SpcLightTexture : register(t1);
Texture2D AmbLightTexture : register(t2);
Texture2D DiffuseTexture : register(t3);
Texture2D BlurTexture : register(t4);

SamplerState POINTSAMPLER : register(s0);

struct OutPutTarget
{
    float4 DiffuseTarget : SV_Target0;
    float4 DifLightTarget : SV_Target1;
    float4 SpcLightTarget : SV_Target2;
    float4 AmbLightTarget : SV_Target3;
};

cbuffer Intensity : register(b0)
{
    float4 Intensity;
};
    
OutPutTarget DetectMask_PS(OutPut _Value)
{
    float4 BlurColor = BlurTexture.Sample(POINTSAMPLER, _Value.UV.xy);
    
    if (BlurColor.a <= 0.0f)
    {
        clip(-1);
    }
    
    float4 DiffuseColor = DiffuseTexture.Sample(POINTSAMPLER, _Value.UV.xy);
    
    if (BlurColor.r != DiffuseColor.r || BlurColor.g != DiffuseColor.g || BlurColor.b != DiffuseColor.b)
    {
        clip(-1);
    }
    
    float4 DifLight = DifLightTexture.Sample(POINTSAMPLER, _Value.UV.xy);
    float4 SpcLight = SpcLightTexture.Sample(POINTSAMPLER, _Value.UV.xy);
    float4 AmbLight = AmbLightTexture.Sample(POINTSAMPLER, _Value.UV.xy);
   
    OutPutTarget OutPut = (OutPutTarget) 0.0f;
    
    OutPut.DiffuseTarget = DiffuseColor;
    
    DifLight *= DiffuseColor;
    SpcLight *= DiffuseColor;
    AmbLight *= DiffuseColor;
    
    OutPut.DifLightTarget = DifLight * Intensity.x;
    OutPut.SpcLightTarget = SpcLight * Intensity.y;
    OutPut.AmbLightTarget = AmbLight * Intensity.z;

    return OutPut;
}


