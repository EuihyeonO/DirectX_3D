#include "PrecompileHeader.h"
#include "Player.h"
#include "ContentFBXRenderer.h"
#include <GameEngineCore/GameEngineFBXRenderer.h>
#include "PhysXTestLevel.h"
#include "PhysXCapsuleComponent.h"

#include "PlayerAttackRange.h"


#define PlayerInitRotation float4{ 90,0,0 }

Player* Player::MainPlayer = nullptr;


Player::Player()
{
	//if (nullptr != MainPlayer)
	//{
	//	MainPlayer->Death();
	//	//MainPlayer = nullptr;
	//}
	MainPlayer = this;
}

Player::~Player()
{
}

#define PLAYER_PHYSX_SCALE float4{0.0f, 75.0f, 50.0f}

void Player::Start()
{
	//init
	InitInputKey();
	InitPlayerAnimation();

	// physx
	{

		m_pCapsuleComp = CreateComponent<PhysXCapsuleComponent>();
		m_pCapsuleComp->SetPhysxMaterial(1.f, 1.f, 0.f);
		m_pCapsuleComp->CreatePhysXActors(PLAYER_PHYSX_SCALE);

		// lever 충돌테스트 
		m_pCapsuleComp->SetFilterData(PhysXFilterGroup::PlayerDynamic, PhysXFilterGroup::LeverTrigger);
	}

	//m_pCapsuleComp->SetDynamicPivot()
	AttackRange = GetLevel()->CreateActor< PlayerAttackRange>();
	AttackRange->Off();

	SetFSMFunc();
	Renderer->ChangeAnimation("IDLE0");
}

void Player::Update(float _DeltaTime)
{
	DirectionUpdate(_DeltaTime);
	FSMObjectBase::Update(_DeltaTime);
	DefaultPhysX();

	// input 사다리타기 추후 trigger로 변경할 예정
	if (true == GameEngineInput::IsDown("PressN"))
	{
		if (PlayerState::IDLE == GetCurState<PlayerState>())
		{
			SetNextState(PlayerState::CLIMB);
		}
		else
		{
			SetNextState(PlayerState::IDLE);
		}
	}
	// state와 상관없이 스킬 변경 가능
	if (PlayerState::SKILL != GetCurState<PlayerState>())
	{
		SetSkill();
	}

	StackDuration -= _DeltaTime;
	if (StackDuration < 0.0f)
	{
		AttackStack = 0;
	}
	// 뭔지? 모름?
	m_pCapsuleComp->GetDynamic()->setMass(65);


	// test
	float4 MyPos = GetTransform()->GetWorldPosition();

}


void Player::CheckDirInput(float _DeltaTime)
{
	float4 Dir = float4::ZERO;
	if (true == GameEngineInput::IsPress("PlayerLeft"))
	{
		Dir += float4::LEFT; // 0 -90 0
	}
	if (true == GameEngineInput::IsPress("PlayerRight"))
	{
		Dir += float4::RIGHT; // 0 90 0
	}
	if (true == GameEngineInput::IsPress("PlayerUp"))
	{
		Dir += float4::FORWARD; // 0 0 0
	}
	if (true == GameEngineInput::IsPress("PlayerDown"))
	{
		Dir += float4::BACK; // 0 180 0
	}

	if (false == Dir.IsZero()) //  방향 입력이 있다면
	{
		SetNextState(PlayerState::WALK);
		//DirectionUpdate(_DeltaTime);
		MoveDir = Dir.NormalizeReturn();
		MoveUpdate(PlayerMoveSpeed);
	}
	else // 방향 입력이 없다면 IDLE
	{
		if (false == GetStateChecker() && GetCurState<PlayerState>() == PlayerState::WALK)
		{
			SetStateCheckerOn();
			//StateChecker = true;
			return;
		}
		else
		{
			SetNextState(PlayerState::IDLE);
		}
	}
}

void Player::CheckStateInput(float _DeltaTime)
{
	if (true == GameEngineInput::IsDown("PlayerRoll")) // roll antmation inter이 필요함
	{
		SetNextState(PlayerState::ROLL);
	}
	else if (true == GameEngineInput::IsPress("PlayerLBUTTON"))
	{
		SetNextState(PlayerState::BASE_ATT);
	}
	else if (true == GameEngineInput::IsPress("PlayerRBUTTON"))
	{
		SetNextState(PlayerState::SKILL);
	}
	else if (true == GameEngineInput::IsPress("PlayerMBUTTON"))
	{
		SetNextState(PlayerState::CHARGE_ATT);
	}
}

void Player::CheckFalling()
{
	// Falling Check
	float4 PlayerGroundPos = GetTransform()->GetWorldPosition();
	float4 CollPoint = float4::ZERO;
	if (true == m_pCapsuleComp->RayCast(PlayerGroundPos, float4::DOWN, CollPoint, 2000.0f))
	{
		float4 PPos = GetTransform()->GetWorldPosition();
		if (PPos.y > CollPoint.y + 60.0f)
		{
			SetNextState(PlayerState::FALLING);
			return;
		}
	}
}
void Player::CheckState(float _DeltaTime)
{
	StateInputDelayTime -= _DeltaTime;
	if (StateInputDelayTime > 0.0f)
	{
		return;
	}
	CheckDirInput(_DeltaTime);
	CheckStateInput(_DeltaTime);
	CheckFalling();
}



void Player::DirectionUpdate(float _DeltaTime)
{
	if (MoveDir == ForwardDir)
	{
		return;
	}
	float4 LerpDir = float4::LerpClamp(ForwardDir, MoveDir, _DeltaTime * 10.0f);
	//float4 NextFRot = float4::LerpClamp(MoveDir, NextDir, _DeltaTime * 10.0f);

	float4 CalRot = float4::ZERO;
	CalRot.y = float4::GetAngleVectorToVectorDeg360(PlayerDefaultDir, LerpDir);
	m_pCapsuleComp->SetRotation(/*PlayerInitRotation*/ -CalRot);
	ForwardDir = LerpDir;
}


void Player::MoveUpdate(float _MoveVec, float4 _Dir)
{
	m_pCapsuleComp->GetDynamic()->setLinearVelocity({ 0,0,0 });
	if (float4::ZERONULL == _Dir)
	{
		m_pCapsuleComp->SetMoveSpeed(MoveDir * _MoveVec);
	}
	else
	{
		m_pCapsuleComp->SetMoveSpeed(_Dir * _MoveVec);
	}
}


void Player::UserUpdate(float _DeltaTime)
{
}

void Player::ServerUpdate(float _DeltaTime)
{
}


void Player::DefaultPhysX()
{
	// physx
	{
		float4 PlayerGroundPos = GetTransform()->GetWorldPosition();
		float4 CollPoint = float4::ZERO;
		if (true == m_pCapsuleComp->RayCast(PlayerGroundPos, float4::DOWN, CollPoint, 2000.0f) && PlayerState::CLIMB != GetCurState< PlayerState>())
		{
			float4 PPos = GetTransform()->GetWorldPosition();
			if (PPos.y > CollPoint.y + 40.0f)
			{
				return;
			}
		}
		if (PlayerState::FALLING == GetCurState< PlayerState>() || PlayerState::BASE_ATT == GetCurState<PlayerState>())
		{
			return;
		}
		MoveUpdate(0.0f);
		//m_pCapsuleComp->GetDynamic()->setLinearVelocity({ 0,0,0 });
		//m_pCapsuleComp->SetMoveSpeed(float4::ZERO);
	}
}


void Player::SetSkill()
{
	if (true == GameEngineInput::IsDown("PlayerSkillArrow"))
	{
		CurSkill = PlayerSkill::ARROW;
	}
	else if (true == GameEngineInput::IsDown("PlayerSkillMagic"))
	{
		CurSkill = PlayerSkill::MAGIC;
	}
	else if (true == GameEngineInput::IsDown("PlayerSkillBomb"))
	{
		CurSkill = PlayerSkill::BOMB;
	}
	else if (true == GameEngineInput::IsDown("PlayerSkillHook"))
	{
		CurSkill = PlayerSkill::HOOK;
	}
}


float4 Player::GetMousDirection()
{
	// Windows Forms to Cartesian Coordinate System
	float4 Mouse2DPos = GameEngineWindow::GetMousePosition() - GameEngineWindow::GetScreenSize().half();
	Mouse2DPos.y = -Mouse2DPos.y;

	// 3D to 2D matrix
	std::shared_ptr<GameEngineCamera> MainCam = GetLevel()->GetMainCamera();
	float4x4 CamViewProjectionMat = MainCam->GetView() * MainCam->GetProjection() * MainCam->GetViewPort();

	float4 Player2DPos = GetTransform()->GetWorldPosition() * CamViewProjectionMat;
	Player2DPos /= Player2DPos.w;
	Player2DPos.w = 1.0f;

	// Windows Forms to Cartesian Coordinate System
	Player2DPos -= GameEngineWindow::GetScreenSize().half();
	Player2DPos.y = -Player2DPos.y;


	float4 NDir = Mouse2DPos - Player2DPos;


	return  float4{ NDir.x, 0, NDir.y }.NormalizeReturn();
}
