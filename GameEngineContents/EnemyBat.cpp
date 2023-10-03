#include "PreCompileHeader.h"
#include "EnemyBat.h"




EnemyBat::EnemyBat()
{
}

EnemyBat::~EnemyBat()
{
}

void EnemyBat::InitAniamtion()
{
	EnemyRenderer = CreateComponent<ContentFBXRenderer>();
	EnemyRenderer->SetFBXMesh("_E_BAT_Black Variant_MESH.FBX", "ContentAniMeshDeffered");

	EnemyRenderer->CreateFBXAnimation("IDLE", "_E_BAT_Black Variant_IDLE.fbx", { 0.02f,true });
	//EnemyRenderer->CreateFBXAnimation("FLY", "_E_BAT_Black Variant_FLY.fbx", { 0.02f,true });
	EnemyRenderer->CreateFBXAnimation("BITE", "_E_BAT_Black Variant_BITE.fbx", { 0.02f,false });
	EnemyRenderer->CreateFBXAnimation("IDLE_FLOOR", "_E_BAT_Black Variant_IDLE_FLOOR.fbx", { 0.02f,true });
	EnemyRenderer->CreateFBXAnimation("SHOCK", "_E_BAT_Black Variant_SHOCK.fbx", { 0.02f,false });
	EnemyRenderer->ChangeAnimation("IDLE");
}

void EnemyBat::Start()
{
	EnemyBase::Start();
	GetTransform()->SetLocalScale(float4::ONE * 20.0f);

	// physx
	{
		float4 scale = EnemyRenderer->GetMeshScale() * EnemyRenderer->GetTransform()->GetWorldScale() / EnemyRenderer->GetTransform()->GetLocalScale();
		// scale *= 2.0f;
		physx::PxVec3 vscale = physx::PxVec3(scale.x, scale.y, scale.z);
		m_pCapsuleComp = CreateComponent<PhysXCapsuleComponent>();
		m_pCapsuleComp->SetPhysxMaterial(1.f, 1.f, 0.f);
		m_pCapsuleComp->CreatePhysXActors(vscale);
	}
	SetFSMFUNC();
	SetNextState(EnemyBatState::FLY);
}

void EnemyBat::Update(float _DeltaTime)
{
	FSMObjectBase::Update(_DeltaTime);
}


void EnemyBat::IdleMove(float _DeltaTime)
{
	static float IdleDir = 0.0f;
	IdleDir += _DeltaTime*100.0f;
	m_pCapsuleComp->SetRotation(float4{0,IdleDir ,0});
	m_pCapsuleComp->SetMoveSpeed(GetTransform()->GetLocalForwardVector() * BAT_MOVE_SPEED);

}

void EnemyBat::AggroMove(float _DeltaTime)
{
	//EnemyBase::AggroDir(m_pCapsuleComp);
	//float4 PlayerDir = GetPlayerDir();
	//float4 CalRot = float4::ZERO;
	//CalRot.y = float4::GetAngleVectorToVectorDeg360(float4::FORWARD, PlayerDir);
	//m_pCapsuleComp->SetRotation(/*PlayerInitRotation*/ -CalRot);

	m_pCapsuleComp->SetMoveSpeed(AggroDir(m_pCapsuleComp) * BAT_MOVE_SPEED);

}



void EnemyBat::SetFSMFUNC()
{
	InitFSM(EnemyBatState::MAX);

	SetChangeFSMCallBack([this]
		{

		});



	SetFSM(EnemyBatState::IDLE,
		[this]
		{
			EnemyRenderer->ChangeAnimation("IDLE");
		},
		[this](float Delta)
		{
			if (true == InRangePlayer(1000.0f))
			{
				SetNextState(EnemyBatState::FLY);
				return;
			}
			IdleMove(Delta);
		},
		[this]
		{
		}
	);

	SetFSM(EnemyBatState::FLY,
		[this]
		{
			//EnemyRenderer->ChangeAnimation("FLY");

		},
		[this](float Delta)
		{
			if (true == InRangePlayer(300.0f))
			{
				SetNextState(EnemyBatState::BITE);
				return;
			}
			if (false == InRangePlayer(1000.0f))
			{
				SetNextState(EnemyBatState::IDLE);
			}
			AggroMove(Delta);
		},
		[this]
		{
		}
	);

	SetFSM(EnemyBatState::BITE,
		[this]
		{
			EnemyRenderer->ChangeAnimation("BITE");
		},
		[this](float Delta)
		{
			if (true == EnemyRenderer->IsAnimationEnd())
			{
				SetNextState(EnemyBatState::IDLE);
			}
		},
		[this]
		{
		}
	);


	SetFSM(EnemyBatState::SHOCK,
		[this]
		{
			EnemyRenderer->ChangeAnimation("SHOCK");
		},
		[this](float Delta)
		{
		},
		[this]
		{
		}
	);
}
