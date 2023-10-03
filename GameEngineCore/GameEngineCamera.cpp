#include "PrecompileHeader.h"
#include "GameEngineRenderer.h"
#include "GameEngineCamera.h"
#include <GameEnginePlatform/GameEngineInput.h>
#include <GameEnginePlatform/GameEngineWindow.h>
#include "GameEngineDevice.h"
#include "GameEngineRenderer.h"
#include "GameEngineRenderTarget.h"
#include "GameEngineMaterial.h"
#include "GameEnginePixelShader.h"
#include "GameEngineFBXRenderer.h"
#include "GameEngineLight.h"

GameEngineCamera::GameEngineCamera()
{
}

GameEngineCamera::~GameEngineCamera()
{
}


void GameEngineCamera::Start()
{
	if (false == GameEngineInput::IsKey("CamMoveLeft"))
	{
		GameEngineInput::CreateKey("CamMoveLeft", 'A');
		GameEngineInput::CreateKey("CamMoveRight", 'D');
		GameEngineInput::CreateKey("CamMoveUp", 'Q');
		GameEngineInput::CreateKey("CamMoveDown", 'E');
		GameEngineInput::CreateKey("CamMoveForward", 'W');
		GameEngineInput::CreateKey("CamMoveBack", 'S');

		GameEngineInput::CreateKey("RotY+", VK_NUMPAD1);
		GameEngineInput::CreateKey("RotY-", VK_NUMPAD2);
		GameEngineInput::CreateKey("RotZ+", VK_NUMPAD4);
		GameEngineInput::CreateKey("RotZ-", VK_NUMPAD5);
		GameEngineInput::CreateKey("RotX+", VK_NUMPAD7);
		GameEngineInput::CreateKey("RotX-", VK_NUMPAD8);

		GameEngineInput::CreateKey("CamRot", VK_RBUTTON);
		GameEngineInput::CreateKey("SpeedBoost", VK_LSHIFT);
		GameEngineInput::CreateKey("FreeCameraSwitch", VK_F1);
		GameEngineInput::CreateKey("ProjectionModeChange", 'O');
		GameEngineInput::AddIgnoreKeys("FreeCameraSwitch");
	}

	// float _Width, float _Height, float _Left, float _Right, float _ZMin = 0.0f, float _ZMax = 1.0f

	ViewPortData.TopLeftX = 0;
	ViewPortData.TopLeftY = 0;
	ViewPortData.Width = GameEngineWindow::GetScreenSize().x;
	ViewPortData.Height = GameEngineWindow::GetScreenSize().y;
	ViewPortData.MinDepth = 0.0f;
	ViewPortData.MaxDepth = 1.0f;

	Width = ViewPortData.Width;
	Height = ViewPortData.Height;

	//사용할 렌더타겟을 생성하는 것이 아닌 추후 사용할 렌더타겟을 생성하기 위한 더미 타겟
	AllRenderTarget			= GameEngineRenderTarget::CreateDummy();
	CamTarget				= GameEngineRenderTarget::CreateDummy();
	DeferredLightTarget		= GameEngineRenderTarget::CreateDummy();
	CamForwardTarget		= GameEngineRenderTarget::CreateDummy();
	CamDeferrdTarget		= GameEngineRenderTarget::CreateDummy();
	CamAlphaTarget			= GameEngineRenderTarget::CreateDummy();
	DeferredPostLightTarget = GameEngineRenderTarget::CreateDummy();

}

void GameEngineCamera::InitCameraRenderTarget()
{
	//렌더 타겟을 실제 생성하는 곳

	AllRenderTarget->AddNewTexture(DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, GameEngineWindow::GetScreenSize(), float4::ZERONULL);
	AllRenderTarget->AddNewTexture(DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, GameEngineWindow::GetScreenSize(), float4::ZERONULL);
	AllRenderTarget->AddNewTexture(DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, GameEngineWindow::GetScreenSize(), float4::ZERONULL);
	AllRenderTarget->AddNewTexture(DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, GameEngineWindow::GetScreenSize(), float4::ZERONULL);
	AllRenderTarget->AddNewTexture(DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, GameEngineWindow::GetScreenSize(), float4::ZERONULL);
	AllRenderTarget->AddNewTexture(DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, GameEngineWindow::GetScreenSize(), float4::ZERONULL);
	AllRenderTarget->AddNewTexture(DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, GameEngineWindow::GetScreenSize(), float4::ZERONULL);
	AllRenderTarget->AddNewTexture(DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, GameEngineWindow::GetScreenSize(), float4::ZERONULL);
	AllRenderTarget->CreateDepthTexture();

	CamTarget->AddNewTexture(DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, GameEngineWindow::GetScreenSize(), float4::ZERONULL);

	DeferredLightTarget->AddNewTexture(DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, GameEngineWindow::GetScreenSize(), float4::ZERONULL);// 디퓨즈
	DeferredLightTarget->AddNewTexture(DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, GameEngineWindow::GetScreenSize(), float4::ZERONULL); // 스펙큘러
	DeferredLightTarget->AddNewTexture(DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, GameEngineWindow::GetScreenSize(), float4::ZERONULL); // 앰비언트
	DeferredLightTarget->AddNewTexture(DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, GameEngineWindow::GetScreenSize(), float4::ZERONULL); // 전체
	DeferredLightTarget->AddNewTexture(DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, GameEngineWindow::GetScreenSize(), float4::ZERONULL); // 포인트라이트

	CamForwardTarget->AddNewTexture(DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, GameEngineWindow::GetScreenSize(), float4::ZERONULL);
	CamDeferrdTarget->AddNewTexture(DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, GameEngineWindow::GetScreenSize(), float4::ZERONULL);
	CamAlphaTarget->AddNewTexture(DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, GameEngineWindow::GetScreenSize(), float4::ZERONULL);

	DeferredPostLightTarget->AddNewTexture(DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, GameEngineWindow::GetScreenSize(), float4::ZERONULL);
	DeferredPostLightTarget->AddNewTexture(DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, GameEngineWindow::GetScreenSize(), float4::ZERONULL); 
	DeferredPostLightTarget->AddNewTexture(DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, GameEngineWindow::GetScreenSize(), float4::ZERONULL);

	CalLightUnit.SetMesh("FullRect");
	CalLightUnit.SetMaterial("DeferredCalLight");

	LightDatas& Data = GetLevel()->LightDataObject;
	AllPointLight& PointLight = GetLevel()->PointLights;

	CalLightUnit.ShaderResHelper.SetConstantBufferLink("LightDatas", Data);
	CalLightUnit.ShaderResHelper.SetConstantBufferLink("AllPointLight", PointLight);

	CalLightUnit.ShaderResHelper.SetTexture("PositionTex", AllRenderTarget->GetTexture(2));
	CalLightUnit.ShaderResHelper.SetTexture("NormalTex", AllRenderTarget->GetTexture(3));

	LightPostUnit.SetMesh("FullRect");
	LightPostUnit.SetMaterial("DeferredPostLight");

	LightPostUnit.ShaderResHelper.SetTexture("DifLightTex", DeferredLightTarget->GetTexture(0));
	LightPostUnit.ShaderResHelper.SetTexture("SpcLightTex", DeferredLightTarget->GetTexture(1));
	LightPostUnit.ShaderResHelper.SetTexture("AmbLightTex", DeferredLightTarget->GetTexture(2));
	LightPostUnit.ShaderResHelper.SetTexture("ShadowTestTex", DeferredLightTarget->GetTexture(3));

	DefferdMergeUnit.SetMesh("FullRect");
	DefferdMergeUnit.SetMaterial("DeferredMerge");
	DefferdMergeUnit.ShaderResHelper.SetTexture("DifColor", AllRenderTarget->GetTexture(1));
	DefferdMergeUnit.ShaderResHelper.SetTexture("DifLight", DeferredPostLightTarget->GetTexture(0));
	DefferdMergeUnit.ShaderResHelper.SetTexture("SpcLight", DeferredPostLightTarget->GetTexture(1));
	DefferdMergeUnit.ShaderResHelper.SetTexture("AmbLight", DeferredPostLightTarget->GetTexture(2));
	DefferdMergeUnit.ShaderResHelper.SetTexture("PointLightTex", DeferredLightTarget->GetTexture(4));
}

void GameEngineCamera::ReleaseCameraRenderTarget()
{
	//사용한 렌더타겟을 삭제하는곳
	// 쉐이더 헬퍼가 사용하는 텍스쳐도 매번 생성되버리니 삭제함

	CalLightUnit.ShaderResHelper.ReleaseAllSetter();
	LightPostUnit.ShaderResHelper.ReleaseAllSetter();
	DefferdMergeUnit.ShaderResHelper.ReleaseAllSetter();

	AllRenderTarget->ReleaseTexture();
	CamTarget->ReleaseTexture();
	DeferredLightTarget->ReleaseTexture();
	CamForwardTarget->ReleaseTexture();
	CamDeferrdTarget->ReleaseTexture();
	CamAlphaTarget->ReleaseTexture();
	DeferredPostLightTarget->ReleaseTexture();
}

void GameEngineCamera::FreeCameraSwitch()
{
	if (GetLevel()->GetMainCamera().get() != this)
	{
		return;
	}

	FreeCamera = !FreeCamera;

	if (true == FreeCamera)
	{
		OldData = GetTransform()->GetTransDataRef();
	}
	else {
		GetTransform()->SetTransformData(OldData);
	}
}

void GameEngineCamera::Update(float _DeltaTime)
{
	if (true == GameEngineInput::IsDown("ProjectionModeChange"))
	{
		switch (ProjectionType)
		{
		case CameraType::None:
			break;
		case CameraType::Perspective:
			ProjectionType = CameraType::Orthogonal;
			break;
		case CameraType::Orthogonal:
			ProjectionType = CameraType::Perspective;
			break;
		default:
			break;
		}
	}

	//if (true == GameEngineInput::IsDown("FreeCameraSwitch"))
	//{
	//	if (GetLevel()->GetMainCamera().get() != this)
	//	{
	//		return;
	//	}

	//	FreeCamera = !FreeCamera;

	//	if (true == FreeCamera)
	//	{
	//		OldData = GetTransform()->GetTransDataRef();
	//	}
	//	else {
	//		GetTransform()->SetTransformData(OldData);
	//	}
	//}

	if (true == FreeCamera)
	{
		float RotSpeed = 180.0f;

		float Speed = 200.0f;

		if (true == GameEngineInput::IsPress("SpeedBoost"))
		{
			Speed = 1000.0f;
		}

		if (true == GameEngineInput::IsPress("CamMoveLeft"))
		{
			GetTransform()->AddLocalPosition(GetTransform()->GetWorldLeftVector() * Speed * _DeltaTime);
		}
		if (true == GameEngineInput::IsPress("CamMoveRight"))
		{
			GetTransform()->AddLocalPosition(GetTransform()->GetWorldRightVector() * Speed * _DeltaTime);
		}
		if (true == GameEngineInput::IsPress("CamMoveUp"))
		{
			GetTransform()->AddLocalPosition(GetTransform()->GetWorldUpVector() * Speed * _DeltaTime);
		}
		if (true == GameEngineInput::IsPress("CamMoveDown"))
		{
			GetTransform()->AddLocalPosition(GetTransform()->GetWorldDownVector() * Speed * _DeltaTime);
		}
		if (true == GameEngineInput::IsPress("CamMoveForward"))
		{
			GetTransform()->AddLocalPosition(GetTransform()->GetWorldForwardVector() * Speed * _DeltaTime);
		}
		if (true == GameEngineInput::IsPress("CamMoveBack"))
		{
			GetTransform()->AddLocalPosition(GetTransform()->GetWorldBackVector() * Speed * _DeltaTime);
		}

		if (true == GameEngineInput::IsPress("CamRot"))
		{
			float4 Dir = GameEngineInput::GetMouseDirectionNormal();

			float4 RotMouseDir;
			RotMouseDir.x = Dir.y;
			RotMouseDir.y = Dir.x;

			GetTransform()->AddWorldRotation(RotMouseDir);
		}

	}

}

void GameEngineCamera::ViewPortSetting()
{
	GameEngineDevice::GetContext()->RSSetViewports(1, &ViewPortData);
}

void GameEngineCamera::Setting()
{
	// 랜더타겟 1개1개마다 뷰포트를 세팅해줄수 있다.
	CamForwardTarget->Clear();
	CamForwardTarget->Setting();
}

void GameEngineCamera::Render(float _DeltaTime)
{
	AllRenderTarget->Clear();
	{
		std::map<int, std::list<std::shared_ptr<GameEngineRenderer>>>::iterator RenderGroupStartIter = Renderers.begin();
		std::map<int, std::list<std::shared_ptr<GameEngineRenderer>>>::iterator RenderGroupEndIter = Renderers.end();

		for (; RenderGroupStartIter != RenderGroupEndIter; ++RenderGroupStartIter)
		{
			std::list<std::shared_ptr<GameEngineRenderer>>& RenderGroup = RenderGroupStartIter->second;

			int Order = RenderGroupStartIter->first;
			std::map<int, SortType>::iterator SortIter = SortValues.find(Order);

			if (SortIter != SortValues.end() && SortIter->second != SortType::None)
			{
				if (SortIter->second == SortType::ZSort)
				{
					for (std::shared_ptr<GameEngineRenderer>& Render : RenderGroup)
					{
						Render->CalSortZ(this);
					}

					// 퀵소트 내일
					RenderGroup.sort([](std::shared_ptr<GameEngineRenderer>& _Left, std::shared_ptr<GameEngineRenderer>& _Right)
						{
							return _Left->CalZ > _Right->CalZ;
						});
				}

				// 정렬을 하겠다는 뜻으로 본다.
			}

			std::list<std::shared_ptr<GameEngineRenderer>>::iterator StartRenderer = RenderGroup.begin();
			std::list<std::shared_ptr<GameEngineRenderer>>::iterator EndRenderer = RenderGroup.end();

			float ScaleTime = _DeltaTime * GameEngineTime::GlobalTime.GetRenderOrderTimeScale(RenderGroupStartIter->first);

			for (; StartRenderer != EndRenderer; ++StartRenderer)
			{
				std::shared_ptr<GameEngineRenderer>& Render = *StartRenderer;

				if (false == Render->IsUpdate())
				{
					continue;
				}

				if (true == Render->IsCameraCulling && false == IsView(Render->GetTransform()->GetTransDataRef()))
				{
					continue;
				}

				Render->RenderTransformUpdate(this);
				Render->RenderBaseValueUpdate(ScaleTime);
				// Render->Render(ScaleTime);
			}
		}
	}

	AllRenderTarget->Setting();
	DeferredLightTarget->Clear();

	{
		for (std::pair<const RenderPath, std::map<int, std::list<std::shared_ptr<class GameEngineRenderUnit>>>>& Path : Units)
		{
			std::map<int, std::list<std::shared_ptr<class GameEngineRenderUnit>>>& UnitPath = Path.second;

			std::map<int, std::list<std::shared_ptr<GameEngineRenderUnit>>>::iterator RenderGroupStartIter = UnitPath.begin();
			std::map<int, std::list<std::shared_ptr<GameEngineRenderUnit>>>::iterator RenderGroupEndIter = UnitPath.end();


			for (; RenderGroupStartIter != RenderGroupEndIter; ++RenderGroupStartIter)
			{
				std::list<std::shared_ptr<GameEngineRenderUnit>>& RenderGroup = RenderGroupStartIter->second;

				std::list<std::shared_ptr<GameEngineRenderUnit>>::iterator StartRenderer = RenderGroup.begin();
				std::list<std::shared_ptr<GameEngineRenderUnit>>::iterator EndRenderer = RenderGroup.end();

				float ScaleTime = _DeltaTime * GameEngineTime::GlobalTime.GetRenderOrderTimeScale(RenderGroupStartIter->first);
				//CameraTransformUpdate();
				for (; StartRenderer != EndRenderer; ++StartRenderer)
				{
					std::shared_ptr<GameEngineRenderUnit>& Render = *StartRenderer;

					if (false == Render->IsUpdate())
					{
						continue;
					}

					if (false == Render->GetRenderer()->IsUpdate())
					{
						continue;
					}
					std::shared_ptr<GameEngineFBXRenderer> FbxRenderer = Render->GetRenderer()->DynamicThis<GameEngineFBXRenderer>();
					if (FbxRenderer !=nullptr && Render->GetUnitPos()!= float4::ZERONULL)					{
						if (false == IsView(Render->GetUnitPos(), Render->GetUnitScale()))
						{
							continue;
						}						
					}

					Render->Render(_DeltaTime);
				}
			}
		}

		GameEngineRenderUnit AniUnit;

		// 여기에서 이미 그림자를 그려야하는 애들은 다 그려져 있어야 합니다.
		for (std::shared_ptr<GameEngineLight> Light : GetLevel()->AllLight)
		{
			Light->GetShadowTarget()->Setting();

			for (std::pair<const RenderPath, std::map<int, std::list<std::shared_ptr<class GameEngineRenderUnit>>>>& Path : Units)
			{
				std::map<int, std::list<std::shared_ptr<class GameEngineRenderUnit>>>& UnitPath = Path.second;

				std::map<int, std::list<std::shared_ptr<GameEngineRenderUnit>>>::iterator RenderGroupStartIter = UnitPath.begin();
				std::map<int, std::list<std::shared_ptr<GameEngineRenderUnit>>>::iterator RenderGroupEndIter = UnitPath.end();

				for (; RenderGroupStartIter != RenderGroupEndIter; ++RenderGroupStartIter)
				{
					std::list<std::shared_ptr<GameEngineRenderUnit>>& RenderGroup = RenderGroupStartIter->second;
					std::list<std::shared_ptr<GameEngineRenderUnit>>::iterator StartRenderer = RenderGroup.begin();
					std::list<std::shared_ptr<GameEngineRenderUnit>>::iterator EndRenderer = RenderGroup.end();

					float ScaleTime = _DeltaTime * GameEngineTime::GlobalTime.GetRenderOrderTimeScale(RenderGroupStartIter->first);

					for (; StartRenderer != EndRenderer; ++StartRenderer)
					{
						std::shared_ptr<GameEngineRenderUnit>& Render = *StartRenderer;

						if (false == Render->IsUpdate())
						{
							continue;
						}

						if (false == Render->GetRenderer()->IsUpdate())
						{
							continue;
						}

						if (false == Render->IsShadow)
						{
							continue;
						}

						Render->GetRenderer()->GetTransform()->SetCameraMatrix(Light->GetLightData().LightViewMatrix, Light->GetLightData().LightProjectionMatrix);
						Render->ShadowSetting();
						std::shared_ptr<GameEngineMaterial> Pipe = GameEngineMaterial::Find("Shadow");
						Pipe->VertexShader();
						Pipe->Rasterizer();
						Pipe->PixelShader();
						Pipe->OutputMerger();
						Render->Draw();
					}
				}
			}
		}

		GameEngineRenderTarget::Reset();
		
		DeferredLightTarget->Setting();
		// 빛이 1개라면 잘 동작할 것이다.

		GetLevel()->LightDataObject.LightCount = 0;
		for (std::shared_ptr<GameEngineLight> Light : GetLevel()->AllLight)
		{
			CalLightUnit.ShaderResHelper.SetTexture("ShadowTex", Light->GetShadowTarget()->GetTexture(0));
			CalLightUnit.Render(_DeltaTime);
			++GetLevel()->LightDataObject.LightCount;
		}

		// 빛계산이 끝나고 디퍼드 머지가 되기전에
		// 한번 빛을 수정하고 들어갈 것이다.
		DeferredLightTarget->Effect(_DeltaTime);

		DeferredPostLightTarget->Clear();
		DeferredPostLightTarget->Setting();
		LightPostUnit.Render(_DeltaTime);

		CamDeferrdTarget->Clear();
		CamDeferrdTarget->Setting();
		DefferdMergeUnit.Render(_DeltaTime);

		CamForwardTarget->Clear();
		CamForwardTarget->Merge(AllRenderTarget, 0);

		CamAlphaTarget->Clear();
		CamAlphaTarget->Merge(AllRenderTarget, 6);

		CamTarget->Clear();
		CamTarget->Merge(CamForwardTarget);
		CamTarget->Merge(CamDeferrdTarget);
		CamTarget->Merge(CamAlphaTarget);
	}

}

void GameEngineCamera::CameraTransformUpdate()
{
	// 뷰행렬을 만들기 위해서는 이 2개의 행렬이 필요하다.
	float4 EyeDir = GetTransform()->GetLocalForwardVector();
	float4 EyeUp = GetTransform()->GetLocalUpVector();
	float4 EyePos = GetTransform()->GetLocalPosition();

	View.LookToLH(EyePos, EyeDir, EyeUp);

	switch (ProjectionType)
	{
	case CameraType::None:
	{
		MsgAssert("카메라 투영이 설정되지 않았습니다.");
		break;
	}
	case CameraType::Perspective:
	{
		Projection.PerspectiveFovLH(FOV, Width / Height, Near, Far);

		float4 Dir = GetTransform()->GetLocalForwardVector();
		float4 WorldPos = GetTransform()->GetWorldPosition();
		//WorldPos.y = -WorldPos.y;

		Frustum.CreateFromMatrix(Frustum,Projection);

		Frustum.Orientation = GetTransform()->GetWorldRotation().EulerDegToQuaternion().DirectFloat4;
		Frustum.Origin = WorldPos.DirectFloat3;
		Frustum.Near = Near;
		Frustum.Far = Far;

		//Frustum.Near = Near;
		//Frustum.Far = Far;
		//Frustum.LeftSlope = -(FOV * GameEngineMath::DegToRad) * 0.7f;
		//Frustum.RightSlope = (FOV * GameEngineMath::DegToRad) * 0.7f;
		//Frustum.TopSlope = (FOV / (Width / Height) * GameEngineMath::DegToRad) * 0.7f;
		//Frustum.BottomSlope = -(FOV / (Width / Height) * GameEngineMath::DegToRad) * 0.7f;

		break;
	}
	case CameraType::Orthogonal:
		Projection.OrthographicLH(Width * ZoomRatio, Height * ZoomRatio, Near, Far);
		break;
	default:
		break;
	}

	ViewPort.ViewPort(GameEngineWindow::GetScreenSize().x, GameEngineWindow::GetScreenSize().y, 0.0f, 0.0f);


	float4 WorldPos = GetTransform()->GetWorldPosition();
	float4 Dir = GetTransform()->GetLocalForwardVector();
	Box.Center = (WorldPos + (Dir * Far * 0.5f)).DirectFloat3;
	Box.Extents.z = Far * 0.6f;
	Box.Extents.x = Width * 0.6f;
	Box.Extents.y = Height * 0.6f;
	Box.Orientation = GetTransform()->GetWorldQuaternion().DirectFloat4;

}


void GameEngineCamera::PushRenderer(std::shared_ptr<GameEngineRenderer> _Render)
{
	if (nullptr == _Render)
	{
		MsgAssert("랜더러가 nullptr 입니다");
		return;
	}

	_Render->RenderCamera = this;

	Renderers[_Render->GetOrder()].push_back(_Render);
}

void GameEngineCamera::PushRenderUnit(std::shared_ptr<GameEngineRenderUnit> _Unit, RenderPath _Path /*= RenderPath::None*/)
{
	if (nullptr == _Unit->GetRenderer())
	{
		MsgAssert("부모가 없는 랜더유니트입니다");
		return;
	}

	int Order = _Unit->GetRenderer()->GetOrder();

	RenderPath Path = _Unit->Material->GetPixelShader()->GetRenderPath();

	if (_Path != RenderPath::None)
	{
		Path = _Path;
	}

	Units[Path][Order].push_back(_Unit);
}

bool GameEngineCamera::IsView(const TransformData& _TransData)
{
	if (true == IsFreeCamera())
	{
		//return true;
	}

	// Width, Height, Near, Far;

	switch (ProjectionType)
	{
	case CameraType::None:
	{
		MsgAssert("카메라 투영이 설정되지 않았습니다.");
		break;
	}
	case CameraType::Perspective:
	{
		DirectX::BoundingSphere Sphere;
		Sphere.Center = _TransData.WorldPosition.DirectFloat3;
		Sphere.Radius = _TransData.WorldScale.MaxFloat() * 0.5f;

		bool IsCal = Frustum.Intersects(Sphere);


		
		return IsCal;

		break;
	}
	case CameraType::Orthogonal:
	{

		DirectX::BoundingSphere Sphere;
		Sphere.Center = _TransData.WorldPosition.DirectFloat3;
		Sphere.Radius = _TransData.WorldScale.MaxFloat() * 0.5f;

		bool IsCal = Box.Intersects(Sphere);

		return IsCal;
		break;
	}
	default:
		break;
	}

	return false;
}

bool GameEngineCamera::IsView(const float4& _Pos, const float4& _Scale)
{

	if (true == IsFreeCamera())
	{
		//return true;
	}

	// Width, Height, Near, Far;

	switch (ProjectionType)
	{
	case CameraType::None:
	{
		MsgAssert("카메라 투영이 설정되지 않았습니다.");
		break;
	}
	case CameraType::Perspective:
	{
		DirectX::BoundingSphere Sphere;
		Sphere.Center = _Pos.DirectFloat3;
		Sphere.Radius = _Scale.MaxFloat() * 0.5f;
		
		DirectX::ContainmentType Type = Frustum.Contains(Sphere);
		
		if (Type == DirectX::ContainmentType::DISJOINT)
		{
			int a = 0;
		}

		bool IsCal = Frustum.Intersects(Sphere);

		return IsCal;

		break;
	}
	case CameraType::Orthogonal:
	{

		DirectX::BoundingSphere Sphere;
		Sphere.Center = _Pos.DirectFloat3;
		Sphere.Radius = _Scale.MaxFloat() * 0.5f;

		bool IsCal = Box.Intersects(Sphere);

		return IsCal;
		break;
	}
	default:
		break;
	}

	return false;
}

void GameEngineCamera::Release()
{

	{
		for (std::pair<const RenderPath, std::map<int, std::list<std::shared_ptr<class GameEngineRenderUnit>>>>& Path : Units)
		{
			std::map<int, std::list<std::shared_ptr<class GameEngineRenderUnit>>>& UnitPath = Path.second;

			std::map<int, std::list<std::shared_ptr<GameEngineRenderUnit>>>::iterator RenderGroupStartIter = UnitPath.begin();
			std::map<int, std::list<std::shared_ptr<GameEngineRenderUnit>>>::iterator RenderGroupEndIter = UnitPath.end();


			for (; RenderGroupStartIter != RenderGroupEndIter; ++RenderGroupStartIter)
			{
				std::list<std::shared_ptr<GameEngineRenderUnit>>& RenderGroup = RenderGroupStartIter->second;

				std::list<std::shared_ptr<GameEngineRenderUnit>>::iterator StartRenderUnit = RenderGroup.begin();
				std::list<std::shared_ptr<GameEngineRenderUnit>>::iterator EndRenderUnit = RenderGroup.end();

				float ScaleTime = GameEngineTime::GlobalTime.GetRenderOrderTimeScale(RenderGroupStartIter->first);

				for (; StartRenderUnit != EndRenderUnit; /*++StartRenderer*/)
				{
					std::shared_ptr<GameEngineRenderUnit>& Render = *StartRenderUnit;

					if (false == Render->GetRenderer()->IsDeath())
					{
						++StartRenderUnit;
						continue;
					}

					StartRenderUnit = RenderGroup.erase(StartRenderUnit);
				}
			}
		}
	}

	std::map<int, std::list<std::shared_ptr<GameEngineRenderer>>>::iterator RenderGroupStartIter = Renderers.begin();
	std::map<int, std::list<std::shared_ptr<GameEngineRenderer>>>::iterator RenderGroupEndIter = Renderers.end();

	for (; RenderGroupStartIter != RenderGroupEndIter; ++RenderGroupStartIter)
	{
		std::list<std::shared_ptr<GameEngineRenderer>>& RenderGroup = RenderGroupStartIter->second;

		std::list<std::shared_ptr<GameEngineRenderer>>::iterator StartRenderer = RenderGroup.begin();
		std::list<std::shared_ptr<GameEngineRenderer>>::iterator EndRenderer = RenderGroup.end();

		for (; StartRenderer != EndRenderer;)
		{
			std::shared_ptr<GameEngineRenderer>& Render = *StartRenderer;

			if (false == Render->IsDeath())
			{
				++StartRenderer;
				continue;
			}

			StartRenderer = RenderGroup.erase(StartRenderer);

		}
	}
}