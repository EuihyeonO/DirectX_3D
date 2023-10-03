#include "PrecompileHeader.h"
#include "GameEngineLevel.h"
#include "GameEngineActor.h"
#include "GameEngineNetActor.h"
#include "GameEngineCamera.h"
#include "GameEngineGUI.h"
#include "GameEngineCollision.h"
#include "GameEngineDebug3D.h"
#include "GameEngineLight.h"
#include <GameEngineCore/GameEngineLight.h>
#include <GameEnginePlatform/GameEngineInput.h>

#include <GameEngineContents/PhysXActor.h>

bool GameEngineLevel::IsDebugRender = false;

GameEngineLevel::GameEngineLevel()
{
	LevelCameraInit();

	if (false == GameEngineInput::IsKey("FreeCameraSwitch"))
	{
		GameEngineInput::CreateKey("FreeCameraSwitch", VK_F1);
	}
	if (false == GameEngineInput::IsKey("DebugSwitch"))
	{
		GameEngineInput::CreateKey("DebugSwitch", VK_F7);
	}

}

void GameEngineLevel::LevelCameraInit()
{
	MainCamera = CreateNewCamera(0);

	std::shared_ptr<GameEngineCamera> UICamera = CreateNewCamera(100);
	UICamera->SetProjectionType(CameraType::Orthogonal);

	if (nullptr == LastTarget)
	{
		LastTarget = GameEngineRenderTarget::CreateDummy();
	}

	if (nullptr == FXAATarget)
	{
		FXAATarget = GameEngineRenderTarget::CreateDummy();
	}
}

GameEngineLevel::~GameEngineLevel()
{
}



void GameEngineLevel::Start()
{
}

void GameEngineLevel::ActorUpdate(float _DeltaTime)
{
	if (true == GameEngineInput::IsDown("FreeCameraSwitch"))
	{
		MainCamera->FreeCameraSwitch();
		// GameEngineInput::CreateKey("FreeCameraSwitch", VK_F1);
	}
	if (true == GameEngineInput::IsDown("DebugSwitch"))
	{
		IsDebugSwitch();
	}


	if (true == MainCamera->IsFreeCamera())
	{
		MainCamera->Update(_DeltaTime);

		GameEngineInput::AllReset();
	}

	{
		// 이건 나중에 만들어질 랜더러의 랜더가 다 끝나고 되는 랜더가 될겁니다.
		std::map<int, std::list<std::shared_ptr<GameEngineActor>>>::iterator GroupStartIter = Actors.begin();
		std::map<int, std::list<std::shared_ptr<GameEngineActor>>>::iterator GroupEndIter = Actors.end();

		for (; GroupStartIter != GroupEndIter; ++GroupStartIter)
		{
			std::list<std::shared_ptr<GameEngineActor>>& ActorList = GroupStartIter->second;

			std::list<std::shared_ptr<GameEngineActor>>::iterator ActorStart = ActorList.begin();
			std::list<std::shared_ptr<GameEngineActor>>::iterator ActorEnd = ActorList.end();

			float ScaleTime = _DeltaTime * GameEngineTime::GlobalTime.GetUpdateOrderTimeScale(GroupStartIter->first);

			for (; ActorStart != ActorEnd; ++ActorStart)
			{
				std::shared_ptr<GameEngineActor>& Actor = *ActorStart;

				Actor->AllAccTime(ScaleTime);
				Actor->AllUpdate(ScaleTime);
			}
		}
	}
}

void GameEngineLevel::ActorLevelChangeStart()
{

	{
		// 이건 나중에 만들어질 랜더러의 랜더가 다 끝나고 되는 랜더가 될겁니다.
		std::map<int, std::list<std::shared_ptr<GameEngineActor>>>::iterator GroupStartIter = Actors.begin();
		std::map<int, std::list<std::shared_ptr<GameEngineActor>>>::iterator GroupEndIter = Actors.end();

		for (; GroupStartIter != GroupEndIter; ++GroupStartIter)
		{
			std::list<std::shared_ptr<GameEngineActor>>& ActorList = GroupStartIter->second;

			std::list<std::shared_ptr<GameEngineActor>>::iterator ActorStart = ActorList.begin();
			std::list<std::shared_ptr<GameEngineActor>>::iterator ActorEnd = ActorList.end();

			for (; ActorStart != ActorEnd; ++ActorStart)
			{
				std::shared_ptr<GameEngineActor>& Actor = *ActorStart;

				Actor->AllLevelChangeStart();
			}
		}
	}
}
void GameEngineLevel::ActorLevelChangeEnd()
{

	{
		// 이건 나중에 만들어질 랜더러의 랜더가 다 끝나고 되는 랜더가 될겁니다.
		std::map<int, std::list<std::shared_ptr<GameEngineActor>>>::iterator GroupStartIter = Actors.begin();
		std::map<int, std::list<std::shared_ptr<GameEngineActor>>>::iterator GroupEndIter = Actors.end();

		for (; GroupStartIter != GroupEndIter; ++GroupStartIter)
		{
			std::list<std::shared_ptr<GameEngineActor>>& ActorList = GroupStartIter->second;

			std::list<std::shared_ptr<GameEngineActor>>::iterator ActorStart = ActorList.begin();
			std::list<std::shared_ptr<GameEngineActor>>::iterator ActorEnd = ActorList.end();

			for (; ActorStart != ActorEnd; ++ActorStart)
			{
				std::shared_ptr<GameEngineActor>& Actor = *ActorStart;

				Actor->AllLevelChangeEnd();
			}
		}
	}
}
//
//void GameEngineLevel::CollisionDebugRender(GameEngineCamera* _Camera, float _Delta)
//{
//
//	//{
//	//	std::map<int, std::list<std::shared_ptr<GameEngineCollision>>>::iterator GroupStartIter = Collisions.begin();
//	//	std::map<int, std::list<std::shared_ptr<GameEngineCollision>>>::iterator GroupEndIter = Collisions.end();
//
//	//	for (; GroupStartIter != GroupEndIter; ++GroupStartIter)
//	//	{
//	//		std::list<std::shared_ptr<GameEngineCollision>>& ObjectList = GroupStartIter->second;
//
//	//		std::list<std::shared_ptr<GameEngineCollision>>::iterator ObjectStart = ObjectList.begin();
//	//		std::list<std::shared_ptr<GameEngineCollision>>::iterator ObjectEnd = ObjectList.end();
//
//	//		for (; ObjectStart != ObjectEnd; ++ObjectStart)
//	//		{
//	//			std::shared_ptr<GameEngineCollision> CollisionObject = (*ObjectStart);
//
//	//			if (nullptr == CollisionObject)
//	//			{
//	//				continue;
//	//			}
//
//	//			if (false == CollisionObject->IsUpdate())
//	//			{
//	//				continue;
//	//			}
//
//	//			if (CollisionObject->DebugCamera != _Camera)
//	//			{
//	//				continue;
//	//			}
//
//	//			// CollisionObject->DebugRender(_Delta);
//	//		}
//	//	}
//	//}
//}

void GameEngineLevel::ActorRelease()
{
	//// 랜더러를 릴리즈 한다.
	for (std::pair<int, std::shared_ptr<GameEngineCamera>> Pair : Cameras)
	{
		std::shared_ptr<GameEngineCamera> Cam = Pair.second;
		Cam->Release();
	}

	{
		std::map<int, std::list<std::shared_ptr<GameEngineCollision>>>::iterator GroupStartIter = Collisions.begin();
		std::map<int, std::list<std::shared_ptr<GameEngineCollision>>>::iterator GroupEndIter = Collisions.end();

		for (; GroupStartIter != GroupEndIter; ++GroupStartIter)
		{
			std::list<std::shared_ptr<GameEngineCollision>>& ObjectList = GroupStartIter->second;

			std::list<std::shared_ptr<GameEngineCollision>>::iterator ObjectStart = ObjectList.begin();
			std::list<std::shared_ptr<GameEngineCollision>>::iterator ObjectEnd = ObjectList.end();

			for (; ObjectStart != ObjectEnd; )
			{
				std::shared_ptr<GameEngineCollision> RelaseObject = (*ObjectStart);

				if (nullptr != RelaseObject && false == RelaseObject->IsDeath())
				{
					++ObjectStart;
					continue;
				}

				ObjectStart = ObjectList.erase(ObjectStart);
			}
		}
	}


	{
		std::map<int, std::list<std::shared_ptr<GameEngineActor>>>::iterator GroupStartIter = Actors.begin();
		std::map<int, std::list<std::shared_ptr<GameEngineActor>>>::iterator GroupEndIter = Actors.end();

		for (; GroupStartIter != GroupEndIter; ++GroupStartIter)
		{
			std::list<std::shared_ptr<GameEngineActor>>& ActorList = GroupStartIter->second;

			std::list<std::shared_ptr<GameEngineActor>>::iterator ActorStart = ActorList.begin();
			std::list<std::shared_ptr<GameEngineActor>>::iterator ActorEnd = ActorList.end();

			for (; ActorStart != ActorEnd; )
			{
				std::shared_ptr<GameEngineActor> RelaseActor = (*ActorStart);
				
				RelaseActor->AllDestroy();

				if (nullptr != RelaseActor && false == RelaseActor->IsDeath())
				{
					RelaseActor->AllRelease();
					++ActorStart;
					continue;
				}
				std::shared_ptr<GameEngineLight> ReleaseLight = RelaseActor->DynamicThis<GameEngineLight>();
				if (nullptr != ReleaseLight)
				{
					std::list<std::shared_ptr<GameEngineLight>>::iterator LightStart = AllLight.begin();
					std::list<std::shared_ptr<GameEngineLight>>::iterator LightEnd = AllLight.end();
					for (; LightStart != LightEnd;)
					{
						if (*LightStart == ReleaseLight)
						{
							ReleaseLight->ReleaseShadowRenderTarget();
							LightStart = AllLight.erase(LightStart);
							break;
						}
						++LightStart;
					}
				}
				RelaseActor->Release();
				ActorStart = ActorList.erase(ActorStart);
			}
		}
	}

}

void GameEngineLevel::Update(float _DeltaTime)
{

}

void GameEngineLevel::Render(float _DeltaTime)
{
	if (true == IsDebugRender)
	{
		std::map<int, std::list<std::shared_ptr<GameEngineCollision>>>::iterator GroupStartIter = Collisions.begin();
		std::map<int, std::list<std::shared_ptr<GameEngineCollision>>>::iterator GroupEndIter = Collisions.end();

		for (; GroupStartIter != GroupEndIter; ++GroupStartIter)
		{
			std::list<std::shared_ptr<GameEngineCollision>>& ObjectList = GroupStartIter->second;

			std::list<std::shared_ptr<GameEngineCollision>>::iterator ObjectStart = ObjectList.begin();
			std::list<std::shared_ptr<GameEngineCollision>>::iterator ObjectEnd = ObjectList.end();

			for (; ObjectStart != ObjectEnd; ++ObjectStart)
			{
				std::shared_ptr<GameEngineCollision> CollisionObject = (*ObjectStart);

				if (nullptr == CollisionObject)
				{
					continue;
				}

				if (false == CollisionObject->IsUpdate())
				{
					continue;
				}

				CollisionObject->DebugRender(_DeltaTime);
			}
		}
	}

	for (std::shared_ptr<GameEngineLight> Light : AllLight)
	{
		Light->GetShadowTarget()->Clear();
	}

	for (std::pair<int, std::shared_ptr<GameEngineCamera>> Pair : Cameras)
	{

		std::shared_ptr<GameEngineCamera> Cam = Pair.second;

		LightDataObject.LightCount = 0;
		for (std::shared_ptr<GameEngineLight> Light : AllLight)
		{
			Light->LightUpdate(Cam.get(), _DeltaTime);
			LightDataObject.AllLight[LightDataObject.LightCount] = Light->GetLightData();
			++LightDataObject.LightCount;
		}

		Cam->CameraTransformUpdate();
		PointLights.ViewInverse = GetMainCamera()->GetView().InverseReturn();
		Cam->ViewPortSetting();
		Cam->Render(_DeltaTime);


		if (false == IsDebugRender)
		{
			continue;
		}

		GameEngineDebug::DebugRender(Cam.get(), _DeltaTime);

		// CollisionDebugRender(Cam.get(), _DeltaTime);
	}

	LastTarget->Clear();

	for (std::pair<int, std::shared_ptr<GameEngineCamera>> Pair : Cameras)
	{
		std::shared_ptr<GameEngineCamera> Camera = Pair.second;
		std::shared_ptr<GameEngineRenderTarget> Target = Camera->CamTarget;

		LastTarget->Merge(Target);
	}

	LastTarget->Effect(_DeltaTime);

	LastTarget->Setting();

	FXAATarget->Clear();
	FXAATarget->Setting();
	
	
	FXAAUnit.Render(0.0f);

	GameEngineDevice::GetBackBufferTarget()->Merge(FXAATarget);

	static bool GUIRender = true;

	if (true == GameEngineInput::IsDown("GUISwitch"))
	{
		GUIRender = !GUIRender;

		if (false == GUIRender)
		{
			GameEngineGUI::Release();
		}
		else {
			GameEngineGUI::Initialize();
		}
	}

	if (true == GUIRender)
	{
		GameEngineGUI::Render(GetSharedThis(), _DeltaTime);
	}

}

void GameEngineLevel::ActorInit(std::shared_ptr<GameEngineActor> _Actor, int _Order, GameEngineLevel* _Level)
{
	_Actor->Level = this;
	_Actor->SetOrder(_Order);
	_Actor->Start();

	// Actors[_Order].push_back(_Actor);
}

void GameEngineLevel::PushCollision(std::shared_ptr<GameEngineCollision> _Collision)
{
	Collisions[_Collision->GetOrder()].push_back(_Collision);
}

void GameEngineLevel::LevelChangeStart()
{

}
void GameEngineLevel::LevelChangeEnd()
{

}


void GameEngineLevel::PushCameraRenderer(std::shared_ptr<GameEngineRenderer> _Renderer, int _CameraOrder)
{
	std::shared_ptr<GameEngineCamera> FindCamera = GetCamera(_CameraOrder);

	if (nullptr == FindCamera)
	{
		MsgAssert("존재하지 않는 카메라에 랜더러를 넣을수는 없습니다.");
		return;
	}

	FindCamera->PushRenderer(_Renderer);
}

std::shared_ptr<GameEngineCamera> GameEngineLevel::CreateNewCamera(int _Order)
{
	if (Cameras.find(_Order) != Cameras.end())
	{
		MsgAssert("이미 존재하는 오더의 카메라를 중복 생성하려했습니다 Order : " + std::to_string(_Order));
		return nullptr;
	}

	std::shared_ptr<GameEngineCamera> NewCamera = CreateActor<GameEngineCamera>();
	Cameras.insert(std::make_pair(_Order, NewCamera));

	return NewCamera;
}

std::shared_ptr<GameEngineCamera> GameEngineLevel::GetCamera(int _CameraOrder)
{
	std::map<int, std::shared_ptr<GameEngineCamera>>::iterator FindIter = Cameras.find(_CameraOrder);

	if (FindIter == Cameras.end())
	{
		return nullptr;
	}

	std::shared_ptr<GameEngineCamera> Camera = FindIter->second;

	return Camera;
}

void GameEngineLevel::TextureUnLoad(GameEngineLevel* _NextLevel)
{
	for (const std::pair<std::string, std::string>& Pair : LoadEndPath)
	{
		if (nullptr != _NextLevel && true == _NextLevel->TexturePath.contains(Pair.first))
		{
			continue;
		}

		GameEngineTexture::UnLoad(Pair.first);
		TexturePath.insert(std::make_pair(Pair.first, Pair.second));
	}

	LoadEndPath.clear();
}

void GameEngineLevel::TextureReLoad(GameEngineLevel* _PrevLevel)
{
	for (const std::pair<std::string, std::string>& Pair : TexturePath)
	{
		if (nullptr != _PrevLevel && true == _PrevLevel->TexturePath.contains(Pair.first))
		{
			continue;
		}

		GameEngineTexture::ReLoad(Pair.second, Pair.first);
		LoadEndPath.insert(std::make_pair(Pair.first, Pair.second));
	}

	TexturePath.clear();
}

void GameEngineLevel::InitLevelRenderTarget()
{
	std::map<int, std::shared_ptr<GameEngineCamera>>::iterator BeginCamIter = Cameras.begin();
	for (; BeginCamIter != Cameras.end(); ++BeginCamIter)
	{
		BeginCamIter->second->InitCameraRenderTarget();
	}

	LastTarget->AddNewTexture(DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, GameEngineWindow::GetScreenSize(), float4::ZERONULL);

	FXAATarget->AddNewTexture(DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, GameEngineWindow::GetScreenSize(), float4::ZERONULL);
	FXAAUnit.SetMesh("FullRect");
	FXAAUnit.SetMaterial("FXAA");

	FXAAUnit.ShaderResHelper.SetTexture("ScreenTexture", LastTarget->GetTexture(0));
	
}

void GameEngineLevel::ReleaseLevelRenderTarget()
{
	std::map<int, std::shared_ptr<GameEngineCamera>>::iterator BeginCamIter = Cameras.begin();
	for (; BeginCamIter != Cameras.end(); ++BeginCamIter)
	{
		BeginCamIter->second->ReleaseCameraRenderTarget();
	}

	LastTarget->ReleaseTexture();

	FXAATarget->ReleaseTexture();

	FXAAUnit.ShaderResHelper.ReleaseAllSetter();
}

void GameEngineLevel::AllActorDestroy()
{
	DestroyCamera();
	{
		// 이건 나중에 만들어질 랜더러의 랜더가 다 끝나고 되는 랜더가 될겁니다.
		std::map<int, std::list<std::shared_ptr<GameEngineActor>>>::iterator GroupStartIter = Actors.begin();
		std::map<int, std::list<std::shared_ptr<GameEngineActor>>>::iterator GroupEndIter = Actors.end();

		for (; GroupStartIter != GroupEndIter; ++GroupStartIter)
		{
			std::list<std::shared_ptr<GameEngineActor>>& ActorList = GroupStartIter->second;

			std::list<std::shared_ptr<GameEngineActor>>::iterator ActorStart = ActorList.begin();
			std::list<std::shared_ptr<GameEngineActor>>::iterator ActorEnd = ActorList.end();

			for (; ActorStart != ActorEnd; ++ActorStart)
			{
				std::shared_ptr<GameEngineActor>& Actor = *ActorStart;
				Actor->Death();
			}
		}

		ActorRelease();
	}
	LevelCameraInit();
}

void GameEngineLevel::DestroyCamera()
{
	for (std::pair<int, std::shared_ptr<GameEngineCamera>> _Cam : Cameras)
	{
		_Cam.second->Renderers.clear();
		_Cam.second->ReleaseCameraRenderTarget();
	}
	Cameras.clear();
}



void GameEngineLevel::PushLight(std::shared_ptr<GameEngineLight> _Light)
{
	// 라이트가 지워질때 어떻게 하지?
	//LightData& LightInst = LightDataObject.AllLight[LightDataObject.LightCount];

	//_Light->LightDataPtr = &LightInst;
	//++LightDataObject.LightCount;
	AllLight.push_back(_Light);
}
//void GameEngineLevel::SendActorPacket(float _DeltaTime)
//{
//	// 모든 Actor가 업데이트 된 후에 SendPacket을 하기 위함이다.
//	{
//		std::map<int, std::list<std::shared_ptr<GameEngineActor>>>::iterator GroupStartIter = Actors.begin();
//		std::map<int, std::list<std::shared_ptr<GameEngineActor>>>::iterator GroupEndIter = Actors.end();
//
//		for (; GroupStartIter != GroupEndIter; ++GroupStartIter)
//		{
//			std::list<std::shared_ptr<GameEngineActor>>& ActorList = GroupStartIter->second;
//
//			std::list<std::shared_ptr<GameEngineActor>>::iterator ActorStart = ActorList.begin();
//			std::list<std::shared_ptr<GameEngineActor>>::iterator ActorEnd = ActorList.end();
//
//			for (; ActorStart != ActorEnd; ++ActorStart)
//			{
//				// 이 액터가 GameEngineNetObject로 캐스팅이 가능하면
//				std::shared_ptr< GameEngineNetActor> CurActor = (*ActorStart)->DynamicThis< GameEngineNetActor>();
//				if (nullptr != CurActor)
//				{
//					CurActor->SendNetPacket(_DeltaTime);
//				}
//			}
//		}
//	}
//}

