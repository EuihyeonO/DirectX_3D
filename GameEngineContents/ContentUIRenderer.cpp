#include "PrecompileHeader.h"
#include "ContentUIRenderer.h"

ContentUIRenderer::ContentUIRenderer()
{
}

ContentUIRenderer::~ContentUIRenderer()
{
}

void ContentUIRenderer::Start()
{
	PushCameraRender(100);

	std::shared_ptr<GameEngineRenderUnit> Unit = CreateRenderUnit();

	Unit->SetMesh("Rect");
	Unit->SetMaterial("Content2DTexture");


	AtlasData.x = 0.0f;
	AtlasData.y = 0.0f;
	AtlasData.z = 1.0f;
	AtlasData.w = 1.0f;

	ColorOptionValue.MulColor = float4::ONE;
	ColorOptionValue.PlusColor = float4::ZERONULL;

	GetShaderResHelper().SetConstantBufferLink("ColorOption", ColorOptionValue);
	GetShaderResHelper().SetConstantBufferLink("UVData", UVData);
}