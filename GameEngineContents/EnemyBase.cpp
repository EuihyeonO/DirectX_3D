#include "PreCompileHeader.h"
#include "EnemyBase.h"
#include "Player.h"
#include "PhysXCapsuleComponent.h"
#include "PhysXControllerComponent.h"

EnemyBase::EnemyBase()
{
}

EnemyBase::~EnemyBase()
{
}

void EnemyBase::Start()
{
	InitAnimation();
	if (nullptr != EnemyRenderer)
	{
		SetFSMFUNC();
	}
}

void EnemyBase::Update(float _DetltaTime)
{
	if (true == m_bEffect)
	{
		EnemyRenderer->FadeIn(1.0f, _DetltaTime);
	}

	if (GetTransform()->GetWorldPosition().y < -300.0f)
	{
		m_iEnemyHP =0;
		return;
	}
}

bool EnemyBase::InRangePlayer(float _Range)
{
	if (nullptr == Player::MainPlayer)
	{
		return false;
	}

	float4 PlayerPos = Player::MainPlayer->GetTransform()->GetWorldPosition();
	PlayerPos.y = 0;
	float4 EnemyPos = GetTransform()->GetWorldPosition();
	EnemyPos.y = 0;
	if ((PlayerPos - EnemyPos).Size() < _Range)
	{
		return true;
	}
	return false;
}
float4 EnemyBase::GetPlayerPosition()
{
	return Player::MainPlayer->GetTransform()->GetWorldPosition();
}

float4 EnemyBase::GetPlayerDir()
{
	float4 PlayerPos = Player::MainPlayer->GetTransform()->GetWorldPosition();
	PlayerPos.y = 0;
	float4 EnemyPos = GetTransform()->GetWorldPosition();
	EnemyPos.y = 0;

	return (PlayerPos - EnemyPos).NormalizeReturn();
}

float4 EnemyBase::GetRotationDegree(const float4& _CurDir)
{
	float4 PlayerDir = GetPlayerDir();
	float4 Rot = float4::ZERO;
	Rot.y = float4::GetAngleVectorToVectorDeg360(PlayerDir, float4::FORWARD);
	return Rot - _CurDir;
}


float4 EnemyBase::AggroDir(std::shared_ptr< PhysXControllerComponent> _Comp, float4 DefaultDir)
{
	float4 PlayerDir = GetPlayerDir();
	_Comp->SetRotation(GetRotationDegree(DefaultDir));
	return PlayerDir;
}


bool EnemyBase::CheckHit()
{
	if (true == CheckCollision(PhysXFilterGroup::PlayerSkill) || true == CheckCollision(PhysXFilterGroup::PlayerBomb))// 플레이어로부터 공격을 받는다면 
	{
		AddPlayerSpellCost();
		GameEngineSound::Play("DefaultHit.mp3");
		--m_iEnemyHP;
		float Crack = static_cast<float>(m_TotalHP-m_iEnemyHP) / m_TotalHP; // 몬스터에 크랙쉐이더 적용 0~1값
		EnemyRenderer->SetCrackAmount(Crack);		
		EnemyRenderer->SetCrackColor({ 0.952f, 0.286f, 0.372f, 1.0f }, 1 + Crack * 10.0f);
		return true;
	}
	return false;
}


void EnemyBase::AddPlayerSpellCost()
{
	Player::MainPlayer->AddSpellCost();
}

bool EnemyBase::DeathCheck()
{
	if (m_iEnemyHP <= 0)
	{
		--m_iEnemyHP;
		return true;
	}
	return false;
}

void EnemyBase::CreateFadeEffect()
{
	m_bEffect = true;
	EnemyRenderer->FadeOut(0.01f, 0.01f);
}

void EnemyBase::FadeInEffect(float _DeltaTime)
{
	EnemyRenderer->FadeIn(1.0f, _DeltaTime);
}
