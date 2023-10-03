#include "ContentTransform.fx"
#include "ContentAnimation.fx"
#include "ContentLight.fx"
#include "ContentRenderBaseValue.fx"
#include "ContentFunction.fx"

struct Input
{
    float4 POSITION : POSITION;
    float4 TEXCOORD : TEXCOORD;
    float4 NORMAL : NORMAL;
    float4 TANGENT : TANGENT;
    float4 BINORMAL : BINORMAL;
    float4 BLENDWEIGHT : BLENDWEIGHT;
    int4 BLENDINDICES : BLENDINDICES;
};

struct Output
{
    float4 POSITION : SV_POSITION;
    float4 VIEWPOSITION : POSITION;
    float4 WVPPOSITION : POSITION5;
    float4 TEXCOORD : TEXCOORD;
    float4 NORMAL : NORMAL;
};


Output ContentMeshDeferred_VS(Input _Input)
{
    Output NewOutPut = (Output) 0;
    
    float4 InputPos = _Input.POSITION;
    InputPos.w = 1.0f;
    
    float4 InputNormal = _Input.NORMAL;
    InputNormal.w = 0.0f;
    
    NewOutPut.POSITION = mul(InputPos, WorldViewProjectionMatrix);
    NewOutPut.TEXCOORD = _Input.TEXCOORD;
    NewOutPut.WVPPOSITION = NewOutPut.POSITION;
    
    NewOutPut.VIEWPOSITION = mul(InputPos, WorldView);
    NewOutPut.NORMAL = mul(InputNormal, WorldView);
    
    return NewOutPut;
}

Texture2D DiffuseTexture : register(t0);
Texture2D MaskTexture : register(t1);

SamplerState ENGINEBASE : register(s0);

struct DeferredOutPut
{
    float4 DifTarget  : SV_Target1;
    float4 PosTarget  : SV_Target2;
    float4 NorTarget  : SV_Target3;
    float4 BlurTarget : SV_Target7;
};


DeferredOutPut ContentAMeshDeferred_PS(Output _Input)
{
    DeferredOutPut NewOutPut = (DeferredOutPut) 0;

    //UV값 변경
    _Input.TEXCOORD.xy *= MulUV;
    _Input.TEXCOORD.xy += AddUV;
    
    float4 Color = DiffuseTexture.Sample(ENGINEBASE, _Input.TEXCOORD.xy);
    
    //텍스쳐 색상 변경
    Color *= MulColor;
    Color += AddColor;
    
    if (Color.a <= 0.0f)
    {
        clip(-1);
    }
    
    //Fade
    if (Delta > 0.0f)
    {
        Color *= Fading(MaskTexture, ENGINEBASE, _Input.TEXCOORD.xy);
    }
    
    NewOutPut.DifTarget = Color;
    NewOutPut.PosTarget = _Input.VIEWPOSITION;
    _Input.NORMAL.a = 1.0f;
    NewOutPut.NorTarget = _Input.NORMAL;
    
    return NewOutPut;
}

