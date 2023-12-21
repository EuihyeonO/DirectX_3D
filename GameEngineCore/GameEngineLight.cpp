#include "PrecompileHeader.h"
#include "GameEngineLight.h"
#include "GameEngineLevel.h"

// 쉐이더에서 Light.fx를 이용하는 경우 이녀석은 빛을 사용하는 녀석이다
// 자동으로 

GameEngineLight::GameEngineLight() 
{
	LightDataValue.ShadowTargetSizeX = 4096;
	LightDataValue.ShadowTargetSizeY = 4096;
	LightDataValue.LightNear = 0.1f;
	LightDataValue.LightFar = 10000.f;
	ShadowRange.x = 16384.0f;
	ShadowRange.y = 16384.0f;
}

GameEngineLight::~GameEngineLight() 
{
}


void GameEngineLight::Update(float _DeltaTime)
{
	//LightDataValue.LightPos = GetTransform()->GetWorldPosition();
	//LightDataValue.LightDir = GetTransform()->GetLocalForwardVector();
}

void GameEngineLight::Start()
{
	// 크기가 곧 그림자가 맺히는 범위와 디테일을 의미하게 된다.
	ShadowTarget = GameEngineRenderTarget::CreateDummy();
	InitShadowRenderTarget();
	GetLevel()->PushLight(DynamicThis<GameEngineLight>());
}

void GameEngineLight::LightUpdate(GameEngineCamera* _Camera, float _DeltaTime) 
{
	// GetTransform()->SetCameraMatrix(_Camera->GetView(), _Camera->GetProjection());

	LightDataValue.CameraViewInverseMatrix = _Camera->GetView().InverseReturn();

	LightDataValue.LightPos = GetTransform()->GetWorldPosition();
	LightDataValue.LightDir = GetTransform()->GetLocalForwardVector();
	LightDataValue.ViewLightPos = LightDataValue.LightPos * _Camera->GetView();
	LightDataValue.ViewLightDir = LightDataValue.LightDir * _Camera->GetView();
	LightDataValue.ViewLightRevDir = -LightDataValue.ViewLightDir;
	LightDataValue.CameraPosition = _Camera->GetTransform()->GetWorldPosition() * _Camera->GetView();


	LightDataValue.LightViewMatrix.LookToLH(
		GetTransform()->GetWorldPosition(),
		GetTransform()->GetWorldForwardVector(),
		GetTransform()->GetWorldUpVector());

	LightDataValue.LightProjectionMatrix.OrthographicLH(
		ShadowRange.x,
		ShadowRange.y,
		LightDataValue.LightNear,
		LightDataValue.LightFar);

	LightDataValue.LightProjectionInverseMatrix = LightDataValue.LightProjectionMatrix.InverseReturn();

	LightDataValue.LightViewProjectionMatrix = LightDataValue.LightViewMatrix * LightDataValue.LightProjectionMatrix;
}

void GameEngineLight::InitShadowRenderTarget()
{
	ShadowTarget->AddNewTexture(DXGI_FORMAT_R32_FLOAT, { LightDataValue.ShadowTargetSizeX, LightDataValue.ShadowTargetSizeY }, float4::RED);
	ShadowTarget->CreateDepthTexture();
}

void GameEngineLight::ReleaseShadowRenderTarget()
{
	ShadowTarget->ReleaseTexture();
	ShadowTarget = nullptr;
}
