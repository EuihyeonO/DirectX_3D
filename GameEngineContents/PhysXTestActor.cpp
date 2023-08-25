#include "PrecompileHeader.h"
#include "PhysXTestActor.h"

#include <GameEngineCore/GameEngineFBXRenderer.h>

#include "PhysXBoxComponent.h"
#include "PhysXDynamicActorComponent.h"
#include "PhysXCapsuleComponent.h"
#include "PhysXConvexComponent.h"
#include "PhysXTriangleComponent.h"
#include "PhysXSphereComponent.h"

#include "PhysXTestLevel.h"


PhysXTestActor::PhysXTestActor() 
{
}

PhysXTestActor::~PhysXTestActor() 
{
}

void PhysXTestActor::Start()
{
	std::shared_ptr<GameEngineFBXRenderer> pRenderer = CreateComponent<GameEngineFBXRenderer>();
	pRenderer->SetFBXMesh("Armature.fbx", "MeshTexture");
	pRenderer->GetFBXMesh();
	GetTransform()->SetLocalPosition(float4(0.f, 0.f, 0.f));

	float4 scale = pRenderer->GetMeshScale();
	//pRenderer->GetTransform()->AddLocalPosition(float4(0.f, -scale.hy(), 0.f));
	physx::PxVec3 vscale = physx::PxVec3(scale.x, scale.y, scale.z);

	//m_pConvexComp = CreateComponent <PhysXConvexComponent>();
	//m_pTriangleComp = CreateComponent <PhysXTriangleComponent>();
	//m_pDynamicActorComp = CreateComponent <PhysXDynamicActorComponent>();
	//m_pGeometryComp = CreateComponent<PhysXBoxGeometryComponent>();
	//m_pCapsuleComp = CreateComponent<PhysXCapsuleComponent>();
	m_pSphereComp = CreateComponent<PhysXSphereComponent>();
	if (GetLevel()->DynamicThis<PhysXTestLevel>()!= nullptr)
	{
		std::shared_ptr<PhysXTestLevel> pLevel = GetLevel()->DynamicThis<PhysXTestLevel>();

		m_pSphereComp->SetPhysxMaterial(0.5f, 0.5f, 0.f);
		m_pSphereComp->CreatePhysXActors(pLevel->GetScene(), pLevel->GetPhysics(), vscale);
		m_pSphereComp->GetDynamic()->setMass(0.65f);
		m_pSphereComp->TurnOnSpeedLimit();
		
		//m_pCapsuleComp->SetPhysxMaterial(1.5f, 0.1f, 1.f);
		//m_pCapsuleComp->CreatePhysXActors(pLevel->m_pScene, pLevel->m_pPhysics,vscale);
		//m_pConvexComp->CreatePhysXActors("Armature.fbx", pLevel->m_pScene, pLevel->m_pPhysics, pLevel->m_pCooking, true, vscale);
		//m_pTriangleComp->CreatePhysXActors("Armature.fbx", pLevel->m_pScene, pLevel->m_pPhysics, pLevel->m_pCooking,true ,vscale);
		//m_pDynamicActorComp->CreatePhysXActors(pLevel->m_pScene, pLevel->m_pPhysics, vscale);

		//_pCapsuleComp->GetDynamic()->setMass(1.f);
		//_pCapsuleComp->TurnOnSpeedLimit();
		//m_pGeometryComp->SetGravity(true);
		//m_pGeometryComp->SetRestitution(2.f);
		//m_pGeometryComp->CreatePhysXActors(pLevel->m_pScene, pLevel->m_pPhysics, vscale);
		//m_pGeometryComp->SetPositionSetFromParentFlag(true);
	}
}

void PhysXTestActor::Update(float _DeltaTime)
{
	if (false== GameEngineInput::IsAnyKey())
	{
		return;
	}
	float4 Movedir = float4::ZERO;
	if (true == GameEngineInput::IsPress("CamMoveLeft"))
	{
		Movedir += GetTransform()->GetWorldLeftVector() * 500.f * _DeltaTime;
		//GetTransform()->AddLocalPosition(GetTransform()->GetWorldLeftVector() * 500.f * _DeltaTime);
	}
	if (true == GameEngineInput::IsPress("CamMoveRight"))
	{
		Movedir += GetTransform()->GetWorldRightVector() * 500.f * _DeltaTime;
		//GetTransform()->AddLocalPosition(GetTransform()->GetWorldRightVector() * 500.f * _DeltaTime);
	}
	if (true == GameEngineInput::IsPress("CamMoveUp"))
	{
		Movedir += GetTransform()->GetWorldUpVector() * 500.f * _DeltaTime;
		//GetTransform()->AddLocalPosition(GetTransform()->GetWorldUpVector() * 500.f * _DeltaTime);
	}
	if (true == GameEngineInput::IsPress("CamMoveDown"))
	{
		Movedir += GetTransform()->GetWorldDownVector() * 500.f * _DeltaTime;
		//GetTransform()->AddLocalPosition(GetTransform()->GetWorldDownVector() * 500.f * _DeltaTime);
	}
	if (true == GameEngineInput::IsPress("CamMoveForward"))
	{
		Movedir += GetTransform()->GetWorldForwardVector() * 500.f * _DeltaTime;
		//GetTransform()->AddLocalPosition(GetTransform()->GetWorldForwardVector() * 500.f * _DeltaTime);
	}
	if (true == GameEngineInput::IsPress("CamMoveBack"))
	{
		Movedir += GetTransform()->GetWorldBackVector() * 500.f * _DeltaTime;
		//GetTransform()->AddLocalPosition(GetTransform()->GetWorldBackVector() * 500.f * _DeltaTime);
	}
	
	Movedir.Normalize();
	//physx::PxTransform test= float4::PhysXTransformReturn(GetTransform()->GetWorldRotation(), Movedir * 1000.f * _DeltaTime);
	//physx::PxTransform test2 = m_pSphereComp->GetDynamic()->getGlobalPose();
	//test.p += test2.p;
	//m_pSphereComp->GetDynamic()->setGlobalPose(test);

	m_pSphereComp->SetUnlockAxis();
	m_pSphereComp->SetMoveSpeed(Movedir * 500);
	//m_pCapsuleComp->SetChangedRot(GetTransform()->GetWorldRotation());
};

