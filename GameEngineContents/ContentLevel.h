#pragma once
#include "PhysXLevel.h"

enum class ContentLevelType
{
	NONE,
	OfficeLevel,
	FortressLevel,
	BossFrogLevel,
	OldCrowLevel,
};

class ContentLevel : public PhysXLevel
{
public:
	ContentLevel();
	~ContentLevel();

	ContentLevel(const ContentLevel& _Other) = delete;
	ContentLevel(ContentLevel&& _Other) noexcept = delete;
	ContentLevel& operator=(const ContentLevel& _Other) = delete;
	ContentLevel& operator=(ContentLevel&& _Other) noexcept = delete;

	inline ContentLevelType GetContentLevelType() const
	{
		return m_eLevelType;
	}

	//ī�޶�� �÷��̾��� �Ÿ�


	inline void SetContentLevelType(ContentLevelType _Type)
	{
		if (_Type == ContentLevelType::NONE)
		{
			MsgAssert("����������Ÿ���� NONE �Դϴ�. �� ������ Start ���� Ÿ���� �����ϼ���.");
			return;
		}

		m_eLevelType = _Type;
	}

	std::shared_ptr<GameEngineActor> GetPivotActor()
	{
		return PivotActor;
	}

	void GraphicUpdate();

	bool isGamma = true;
	bool isHDR = true;
	bool isFXAA = true;

protected:
	void LevelInit(float4 _BlurSize);
	void CreateUI();
	void SetPostPrecessEffect(float4 _BlurSize);
	void CreateIMGUIDebugRenderTarget();
private:

	void GammaUpdate();
	void HDRUpdate();
	void FXAAUpdate();

	void CreatePivotActor();

	std::shared_ptr<class GammaCorrection> Gamma = nullptr;
	std::shared_ptr<class FXAA> AntiAliasing = nullptr;
	std::shared_ptr<class ToneMapping> ToneMappingEffect = nullptr;
	ContentLevelType m_eLevelType = ContentLevelType::NONE;

	std::shared_ptr<GameEngineActor> PivotActor = nullptr;

	bool PrevisGamma = true;
	bool PrevisHDR = true;
	bool PrevisFXAA = true;
};
