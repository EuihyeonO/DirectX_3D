#pragma once
#include <GameEngineCore/GameEngineFBXRenderer.h>

class ContentFBXRenderer : public GameEngineFBXRenderer
{

public:

	ContentFBXRenderer();
	~ContentFBXRenderer();

	ContentFBXRenderer(const ContentFBXRenderer& _Other) = delete;
	ContentFBXRenderer(ContentFBXRenderer&& _Other) noexcept = delete;
	ContentFBXRenderer& operator=(const ContentFBXRenderer& _Other) = delete;
	ContentFBXRenderer& operator=(ContentFBXRenderer&& _Other) noexcept = delete;

	void SetAllUnitTexture(const std::string_view& _SettingName, const std::string_view& _ImageName);
	
	void SetFadeMask(const std::string_view& _MaskTextureName = "MaskType2_0.png");
	void SetCrackMask();

	void FadeOut(float _MaxTime, float _DeltaTime);
	void FadeIn(float _MaxTime, float _DeltaTime);

	void SetFBXMesh(const std::string& _MeshName, const std::string _SettingName) override;
	void SetCrackAmount(float _Amount);

	void SetBlurColor(float4 _Color)
	{
		BlurColor = _Color;
	}

protected:
	void Start() override;
	void Render(float _DeltaTime) override;
private:
	void LinkBlurColor();

	float4 BlurColor = {1.0f, 0.1f, 0.2f};
	std::string MaterialName;
};

