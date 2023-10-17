#include "PreCompileHeader.h"
#include "PlayerAttackBomb.h"

#include "ContentFBXRenderer.h"
#include "PhysXSphereComponent.h"

#include "GrayScaleEffect.h"

PlayerAttackBomb::PlayerAttackBomb() 
{
}

PlayerAttackBomb::~PlayerAttackBomb() 
{
}

void PlayerAttackBomb::Start()
{
	// Render
	AttackRenderer = CreateComponent< ContentFBXRenderer>();
	AttackRenderer->SetFBXMesh("SphereDefault.fbx", "ContentMeshDeffered");
	AttackRenderer->GetTransform()->SetLocalScale(PLAYER_ATT_BOMB_RENDER_SCALE);
	// PhysX
	CreatePhysXAttComp<PhysXSphereComponent>(PLAYER_ATT_BOMB_PHYSX_SCALE, PhysXFilterGroup::PlayerSkill);

	AttackRenderer->SetGlowToUnit(0, 0);
	AttackRenderer->SetBlurColor(float4{1.0f, 0.1f, 0.2f} * 2.5f);

	SetDeathFunction([this]
		{
			std::weak_ptr<GrayScaleEffect> Effect = GetLevel()->GetLastTarget()->CreateEffect<GrayScaleEffect>();
			GameEngineLevel* Level = GetLevel();
			Level->TimeEvent.AddEvent(0.2f, [Level, Effect](GameEngineTimeEvent::TimeEvent&, GameEngineTimeEvent*) {Level->GetLastTarget()->ReleaseEffect(Effect.lock()); });
		}
	);
}

void PlayerAttackBomb::Update(float _DeltaTime)
{
	AttackBase::Update(_DeltaTime);
}
