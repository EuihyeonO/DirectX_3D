#pragma once
#include <GameEngineCore/GameEngineUIRenderer.h>

class ContentUIRenderer : public GameEngineUIRenderer
{

public:

	ContentUIRenderer();
	~ContentUIRenderer();

	ContentUIRenderer(const ContentUIRenderer& _Other) = delete;
	ContentUIRenderer(ContentUIRenderer&& _Other) noexcept = delete;
	ContentUIRenderer& operator=(const ContentUIRenderer& _Other) = delete;
	ContentUIRenderer& operator=(ContentUIRenderer&& _Other) noexcept = delete;
	
	void SetUVData(float4 _Data)
	{
		UVData = _Data;
	}
protected:
	void Start() override;

private:
	float4 UVData = { 1, 1, 0, 0 };
};

