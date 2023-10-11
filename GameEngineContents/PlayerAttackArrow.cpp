#include "PreCompileHeader.h"
#include "PlayerAttackArrow.h"

#include "ContentFBXRenderer.h"
#include "PhysXCapsuleComponent.h"

PlayerAttackArrow::PlayerAttackArrow()
{
}

PlayerAttackArrow::~PlayerAttackArrow()
{
}

void PlayerAttackArrow::Start()
{
	// Render
	AttackRenderer = CreateComponent< ContentFBXRenderer>();
	AttackRenderer->SetFBXMesh("ARROW_MESH.fbx", "ContentMeshDeffered");
	AttackRenderer->GetTransform()->SetLocalScale(PLAYER_ATT_ARROW_RENDER_SCALE);
	AttackRenderer->GetTransform()->SetLocalRotation(PLAYER_ATT_ARROW_RENDER_ROT);
	// PhysX
	CreatePhysXAttComp<PhysXCapsuleComponent>(PLAYER_ATT_ARROW_PHYSX_SCALE);
	PhysXComp->SetShapeAxis(0,float4::LEFT);

	AttackRenderer->SetGlowToUnit(0, 0);
}

void PlayerAttackArrow::Update(float _DeltaTime)
{
	PlayerAttackBase::Update(_DeltaTime);
}

