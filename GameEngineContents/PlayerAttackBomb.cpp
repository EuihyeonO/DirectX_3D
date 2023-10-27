#include "PreCompileHeader.h"
#include "PlayerAttackBomb.h"

#include "ContentFBXRenderer.h"
#include "PhysXSphereComponent.h"

#include "GrayScaleEffect.h"
#include "DustParticle.h"

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
	SetDestTarget(PhysXFilterGroup::MonsterDynamic);

	AttackRenderer->SetGlowToUnit(0, 0);
	AttackRenderer->SetColor(float4{1.0f, 0.1f, 0.2f}, 4.0f);

}
void PlayerAttackBomb::Death()
{
	std::weak_ptr<GrayScaleEffect> Effect = GetLevel()->GetLastTarget()->CreateEffect<GrayScaleEffect>();
	GameEngineLevel* Level = GetLevel();
	Level->TimeEvent.AddEvent(0.2f, [Level, Effect](GameEngineTimeEvent::TimeEvent&, GameEngineTimeEvent*) {Level->GetLastTarget()->ReleaseEffect(Effect.lock()); });

	GameEngineObjectBase::Death();
}


void PlayerAttackBomb::Update(float _DeltaTime)
{
	AttackBase::Update(_DeltaTime);
	CreateParticle(_DeltaTime);
}


void PlayerAttackBomb::CreateParticle(float _DeltaTime)
{
	ParticleCount += _DeltaTime;

	if (ParticleCount >= 0.01f)
	{
		ParticleCount = 0.0f;

		std::shared_ptr<DustParticle> NewParticle = CreateComponent<DustParticle>();
		NewParticle->SetColor({ 1.0f, 0.1f, 0.2f });
		NewParticle->GetTransform()->SetWorldPosition(GetTransform()->GetWorldPosition());
		NewParticle->GetTransform()->SetLocalScale({ 50.0f, 50.0f, 50.0f });
		NewParticle->SetGlow();

	}
}