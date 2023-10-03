#include "PreCompileHeader.h"
#include "EnemyBruteGold.h"

EnemyBruteGold::EnemyBruteGold() 
{
}

EnemyBruteGold::~EnemyBruteGold() 
{
}


void EnemyBruteGold::InitAniamtion()
{
	EnemyRenderer = CreateComponent<ContentFBXRenderer>();
	EnemyRenderer->SetFBXMesh("_E_BRUTE_GOLD_MESH.FBX", "ContentAniMeshDeffered");

	EnemyRenderer->CreateFBXAnimation("IDLE", "_E_BRUTE_GOLD_IDLE.fbx", { 0.02f,true });
	EnemyRenderer->CreateFBXAnimation("WALK", "_E_BRUTE_GOLD_WALK.fbx", { 0.02f,false });
	EnemyRenderer->CreateFBXAnimation("RUN", "_E_BRUTE_GOLD_RUN.fbx", { 0.02f,true });
	EnemyRenderer->CreateFBXAnimation("SLAM", "_E_BRUTE_GOLD_SLAM.fbx", { 0.02f,false });
	EnemyRenderer->CreateFBXAnimation("SWING", "_E_BRUTE_GOLD_SWING.fbx", { 0.02f,false });
	EnemyRenderer->CreateFBXAnimation("BREAK", "_E_BRUTE_GOLD_BREAK.fbx", { 0.02f,false });
	EnemyRenderer->CreateFBXAnimation("THROW", "_E_BRUTE_GOLD_THROW.fbx", { 0.02f,false });

	EnemyRenderer->ChangeAnimation("IDLE");
}



void EnemyBruteGold::Start()
{
	EnemyBase::Start();
	GetTransform()->SetLocalScale(float4::ONE * RENDERSCALE_BRUTEGOLD);

	// physx
	{
		m_pCapsuleComp = CreateComponent<PhysXCapsuleComponent>();
		m_pCapsuleComp->SetPhysxMaterial(1.f, 1.f, 0.f);
		m_pCapsuleComp->CreatePhysXActors(PHYSXSCALE_BRUTEGOLD);
	}
	SetFSMFUNC();
}


void EnemyBruteGold::Update(float _DeltaTime)
{
	FSMObjectBase::Update(_DeltaTime);

}



void EnemyBruteGold::AggroMove(float _DeltaTime)
{
	if (false == GetStateChecker())
	{
		m_pCapsuleComp->SetMoveSpeed(AggroDir(m_pCapsuleComp, DEFAULT_DIR_BRUTEGOLD) * GRUNT_MOVE_SPEED);
	}
	else
	{
		m_pCapsuleComp->SetMoveSpeed(AggroDir(m_pCapsuleComp, DEFAULT_DIR_BRUTEGOLD) * GRUNT_MOVE_SPEED * 2.0f);

	}
}


void EnemyBruteGold::SetFSMFUNC()
{
	InitFSM(EnemyBruteGoldState::MAX);

	SetChangeFSMCallBack([this]
		{
			//StateDuration = 0.0f;
			//StateChecker = false;
		});



	SetFSM(EnemyBruteGoldState::IDLE,
		[this]
		{
			EnemyRenderer->ChangeAnimation("IDLE");
		},
		[this](float Delta)
		{
			if (true == CheckHit())
			{
				SetNextState(EnemyBruteGoldState::BREAK);
				return;
			}
			if(true == InRangePlayer(2000.0f))
			{
				SetNextState(EnemyBruteGoldState::MOVE);
				return;
			}


		},
		[this]
		{
		}
	);

	SetFSM(EnemyBruteGoldState::MOVE,
		[this]
		{
			EnemyRenderer->ChangeAnimation("WALK");
			AggroDir(m_pCapsuleComp, DEFAULT_DIR_BRUTEGOLD);
		},
		[this](float Delta)
		{
			//StateDuration += Delta;
			if (true == CheckHit())
			{
				SetNextState(EnemyBruteGoldState::BREAK);
				return;
			}
			if (false == GetStateChecker() && true == EnemyRenderer->IsAnimationEnd())
			{
				EnemyRenderer->ChangeAnimation("RUN");
				SetStateCheckerOn();
				//StateChecker = true;
			}
			if (true == InRangePlayer(300.0f))
			{
				SetNextState(EnemyBruteGoldState::SWING);
				return;
			}
			if (GetStateDuration() > 5)
			{
				if (true == InRangePlayer(1500.0f))
				{
					SetNextState(EnemyBruteGoldState::THROW);
					ResetStateDuration();
					//StateDuration = 0.0f;
					return;
				}
			}
			AggroMove(Delta);

		},
		[this]
		{
			m_pCapsuleComp->GetDynamic()->setLinearVelocity({ 0,0,0 });
			m_pCapsuleComp->SetMoveSpeed(float4::ZERO);
		}
	);

	SetFSM(EnemyBruteGoldState::SLAM,
		[this]
		{
			EnemyRenderer->ChangeAnimation("SLAM");
			AggroDir(m_pCapsuleComp, DEFAULT_DIR_BRUTEGOLD);

		},
		[this](float Delta)
		{
			if (true == EnemyRenderer->IsAnimationEnd())
			{
				SetNextState(EnemyBruteGoldState::IDLE);
			}
			//AggroDir(m_pCapsuleComp);

		},
		[this]
		{
		}
	);
	SetFSM(EnemyBruteGoldState::SWING,
		[this]
		{
			EnemyRenderer->ChangeAnimation("SWING");
			AggroDir(m_pCapsuleComp, DEFAULT_DIR_BRUTEGOLD);

		},
		[this](float Delta)
		{
			if (true == EnemyRenderer->IsAnimationEnd())
			{
				SetNextState(EnemyBruteGoldState::SLAM);
			}
		},
		[this]
		{
		}
	);

	SetFSM(EnemyBruteGoldState::THROW,
		[this]
		{
			EnemyRenderer->ChangeAnimation("THROW");
			AggroDir(m_pCapsuleComp, DEFAULT_DIR_BRUTEGOLD);
		},
		[this](float Delta)
		{
			if (true == EnemyRenderer->IsAnimationEnd())
			{
				SetNextState(EnemyBruteGoldState::IDLE);
			}

		},
		[this]
		{
		}
	);

	SetFSM(EnemyBruteGoldState::BREAK,
		[this]
		{
			EnemyRenderer->ChangeAnimation("BREAK");
		},
		[this](float Delta)
		{
			if (true == EnemyRenderer->IsAnimationEnd())
			{
				SetNextState(EnemyBruteGoldState::IDLE);
			}
		},
		[this]
		{
		}
	);
}