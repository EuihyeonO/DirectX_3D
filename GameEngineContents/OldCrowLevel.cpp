#include "PrecompileHeader.h"
#include "OldCrowLevel.h"

#include "Player.h"
#include "Boss_OldCrow.h"
#include "Map_Emptyplain.h"
#include "PhysXCapsuleComponent.h"
#include "PhysXBoxComponent.h"
#include "PhysXControllerComponent.h"
#include "FadeEffect.h"

#include "ShortCutDoor.h"
#include "OfficeLevel.h"

#include "PlayerInfoWindow.h"

OldCrowLevel::OldCrowLevel()
{
}

OldCrowLevel::~OldCrowLevel()
{
}

void OldCrowLevel::Start()
{
	SetContentLevelType(ContentLevelType::OldCrowLevel);
	InitKey();

}

void OldCrowLevel::Update(float _DeltaTime)
{
	if (false == m_bIsClear)
	{
		ClearCheck();
	}

	GraphicUpdate();

}

void OldCrowLevel::LevelChangeStart()
{
	LevelInit({ 3200, 1800, 800, 450 });
	CreateScene();

	GetMainCamera()->SetProjectionType(CameraType::Perspective);

	
	Create_Light();
	Create_Map();
	Create_Player();
	Create_OldCrow();
	PlayerInfoWindow::PlayerGUI->On();
}

void OldCrowLevel::LevelChangeEnd()
{
	PlayerInfoWindow::PlayerGUI->Off();
	AllActorDestroy();
}

void OldCrowLevel::Create_Light()
{
	std::shared_ptr<GameEngineLight> Light = CreateActor<GameEngineLight>();
	Light->GetTransform()->SetLocalRotation(float4{30, 180, 0 }); 
	Light->GetTransform()->AddLocalPosition({ 0.0f, 0.0f, 4000.0f });
}

void OldCrowLevel::Create_Map()
{
	m_pMap = CreateActor<Map_Emptyplain>();
}

void OldCrowLevel::Create_Player()
{
	std::shared_ptr<Player> Obj = CreateActor<Player>();
	float4 Pos = Obj->GetTransform()->GetWorldPosition();
	Set_PlayerStartPos();
}

void OldCrowLevel::Create_OldCrow()
{
	m_pBoss = CreateActor<Boss_OldCrow>();
	m_pBoss.lock()->GetPhysXComponent()->SetWorldPosWithParent(float4{0, 0, 1500}, float4{0, 180, 0});
}

void OldCrowLevel::ClearCheck()
{
	bool Clear = m_pBoss.lock()->DeathCheck();
	if (true == Clear)
	{
		m_bIsClear = true;
		std::shared_ptr<ShortCutDoor> Door = CreateActor<ShortCutDoor>();
		Door->GetPhysXComponent()->SetWorldPosWithParent(m_pBoss.lock()->GetTransform()->GetWorldPosition() + float4{ 0 , 0 , 1500 });
		Door->SetTriggerFunction([=]
			{
				std::shared_ptr<GameEngineLevel> NextLevel = GameEngineCore::ChangeLevel("OfficeLevel");
				std::shared_ptr<OfficeLevel> Level = NextLevel->DynamicThis<OfficeLevel>();
				if (nullptr == Level)
				{
					MsgAssert("레벨의 다이나믹캐스트에 실패했습니다.");
					return;
				}

				Level->SetPrevLevelType(PrevLevelType::OldCrowLevel);
			});
	}
}

void OldCrowLevel::InitKey()
{
}

void OldCrowLevel::Set_PlayerStartPos()
{
	if (nullptr == Player::MainPlayer)
	{
		MsgAssert("Player 가 nullptr 입니다.");
		return;
	}

	std::shared_ptr<PhysXControllerComponent> Comp = Player::MainPlayer->GetPhysXComponent();

	if (nullptr == Comp)
	{
		MsgAssert("Player 의 PhysXComponent가 nullptr 입니다.");
		return;
	}

	Comp->SetWorldPosWithParent(m_StartPos);
}
