#pragma once
#include <GameEngineCore/GameEngineRenderTarget.h>

class GlowEffect : public GameEnginePostProcess
{

public:

	GlowEffect();
	~GlowEffect();

	GlowEffect(const GlowEffect& _Other) = delete;
	GlowEffect(GlowEffect&& _Other) noexcept = delete;
	GlowEffect& operator=(const GlowEffect& _Other) = delete;
	GlowEffect& operator=(GlowEffect&& _Other) noexcept = delete;

	void SetLevel(std::weak_ptr<GameEngineLevel> _Level)
	{
		EffectedLevel = _Level;
	}

	std::shared_ptr<GameEngineRenderTarget> DetectMaskTarget = nullptr;
	std::shared_ptr<GameEngineRenderTarget> BlurTarget = nullptr;
	std::shared_ptr<GameEngineRenderTarget> DoubleBlurTarget = nullptr;

	void SetIntensity(float _Intensity)
	{
		//Intensity.x = _Intensity;
		Intensity.y = _Intensity;
		//Intensity.z = _Intensity;
		//Intensity.w = _Intensity;
	}

	void Init(std::weak_ptr<GameEngineLevel> _Level, float _Intensity)
	{
		SetLevel(_Level);
		SetIntensity(_Intensity);
	}

protected:
	void Start(GameEngineRenderTarget* _Target) override;
	void Effect(GameEngineRenderTarget* _Target, float _DeltaTime) override;
private:

	std::weak_ptr<GameEngineLevel> EffectedLevel;

	GameEngineRenderUnit DetectMaskUnit;
	GameEngineRenderUnit BlurMergeUnit;
	GameEngineRenderUnit BlurUnit;
	GameEngineRenderUnit ColorMerge;

	float4 Intensity = { 0.0f, 0.0f };
};

