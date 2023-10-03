#include "PreCompileHeader.h"
#include "EnemyGhoul.h"

EnemyGhoul::EnemyGhoul() 
{
}

EnemyGhoul::~EnemyGhoul() 
{
}

void EnemyGhoul::InitAniamtion()
{
	EnemyRenderer = CreateComponent<ContentFBXRenderer>();
	EnemyRenderer->SetFBXMesh("_E_GHOUL_MESH.FBX", "ContentAniMeshDeffered");

	EnemyRenderer->CreateFBXAnimation("IDLE_BOW", "_E_GHOUL_IDLE_BOW.fbx", { 0.02f,true });

	EnemyRenderer->CreateFBXAnimation("RUN_BOW", "_E_GHOUL_RUN_BOW.fbx", { 0.02f,true });
	EnemyRenderer->CreateFBXAnimation("WALK", "_E_GHOUL_WALK.fbx", { 0.02f,false });

	EnemyRenderer->CreateFBXAnimation("SHOOT_BOW", "_E_GHOUL_SHOOT_BOW.fbx", { 0.02f,false });
	EnemyRenderer->CreateFBXAnimation("SHOOT_BOW_RAPID", "_E_GHOUL_SHOOT_BOW_RAPID.fbx", { 0.02f,false });

	EnemyRenderer->CreateFBXAnimation("HIT_BOW", "_E_GHOUL_HIT_BOW.fbx", { 0.02f,false });
	EnemyRenderer->CreateFBXAnimation("DROWN", "_E_GHOUL_DROWN.fbx", { 0.02f,false });

	//EnemyRenderer->CreateFBXAnimation("IDLE", "_E_GHOUL_IDLE.fbx", { 0.02f,false });
	//EnemyRenderer->CreateFBXAnimation("IDLE_SWORD", "_E_GHOUL_IDLE_SWORD.fbx", { 0.02f,false });
	//EnemyRenderer->CreateFBXAnimation("RUN", "_E_GHOUL_RUN.fbx", { 0.02f,false });
	//EnemyRenderer->CreateFBXAnimation("RUN_SWORD", "_E_GHOUL_RUN_SWORD.fbx", { 0.02f,false });

	EnemyRenderer->ChangeAnimation("IDLE_BOW");
}


void EnemyGhoul::Start()
{
	EnemyBase::Start();
	GetTransform()->SetLocalScale(float4::ONE * RENDERSCALE_GHOUL);

	// physx
	{
		m_pCapsuleComp = CreateComponent<PhysXCapsuleComponent>();
		m_pCapsuleComp->SetPhysxMaterial(1.f, 1.f, 0.f);
		m_pCapsuleComp->CreatePhysXActors(PHYSXSCALE_GHOUL);
	}
	SetFSMFUNC();
}

void EnemyGhoul::Update(float _DeltaTime)
{
	FSMObjectBase::Update(_DeltaTime);
}

#define EnemyGhoulDir float4::BACK
#define GHOUL_MOVE_SPEED 100.0f

void EnemyGhoul::AggroMove(float _DeltaTime)
{
	if (false == GetStateChecker())
	{
		m_pCapsuleComp->SetMoveSpeed(AggroDir(m_pCapsuleComp, EnemyGhoulDir) * GHOUL_MOVE_SPEED);
	}
	else
	{
		m_pCapsuleComp->SetMoveSpeed(AggroDir(m_pCapsuleComp, EnemyGhoulDir) * GHOUL_MOVE_SPEED * 2.0f);

	}
}

void EnemyGhoul::SetFSMFUNC()
{
	InitFSM(EnemyGhoulState::MAX);

	SetChangeFSMCallBack([this]
		{
			//StateDuration = 0.0f;
			//StateChecker = false;
		});



	SetFSM(EnemyGhoulState::IDLE,
		[this]
		{
			EnemyRenderer->ChangeAnimation("IDLE_BOW");
		},
		[this](float Delta)
		{
			if (true == InRangePlayer(1500.0f))
			{
				SetNextState(EnemyGhoulState::MOVE);
				return;
			}
		},
		[this]
		{
		}
	);

	SetFSM(EnemyGhoulState::MOVE,
		[this]
		{
			EnemyRenderer->ChangeAnimation("WALK");
			AggroDir(m_pCapsuleComp);
		},
		[this](float Delta)
		{
			if (false == GetStateChecker() && true == EnemyRenderer->IsAnimationEnd())
			{
				EnemyRenderer->ChangeAnimation("RUN_BOW");
				SetStateCheckerOn();
				//StateChecker = true;
			}
			AggroMove(Delta);
			if (true == InRangePlayer(1000.0f))
			{
				SetNextState(EnemyGhoulState::SHOOT);
				return;
			}
		},
		[this]
		{
		}
	);

	SetFSM(EnemyGhoulState::SHOOT,
		[this]
		{
			EnemyRenderer->ChangeAnimation("SHOOT_BOW");
			AggroDir(m_pCapsuleComp);
		},
		[this](float Delta)
		{
			if (true == EnemyRenderer->IsAnimationEnd())
			{
				SetNextState(EnemyGhoulState::IDLE);
				return;
			}
		},
		[this]
		{
		}
	);

	SetFSM(EnemyGhoulState::HIT,
		[this]
		{
			EnemyRenderer->ChangeAnimation("HIT_BOW");
			AggroDir(m_pCapsuleComp);
		},
		[this](float Delta)
		{
			if (true == EnemyRenderer->IsAnimationEnd())
			{
				SetNextState(EnemyGhoulState::IDLE);
				return;
			}
		},
		[this]
		{
		}
	);

	SetFSM(EnemyGhoulState::DEATH,
		[this]
		{
			EnemyRenderer->ChangeAnimation("DROWN");
		},
		[this](float Delta)
		{
		},
		[this]
		{
		}
	);
}