#include "PreCompileHeader.h"
#include "EnemyBrute.h"

EnemyBrute::EnemyBrute() 
{
}

EnemyBrute::~EnemyBrute() 
{
}


void EnemyBrute::InitAniamtion()
{
	EnemyRenderer = CreateComponent<ContentFBXRenderer>();
	EnemyRenderer->SetFBXMesh("_E_BRUTE_MESH.FBX", "ContentAniMeshDeffered");

	EnemyRenderer->CreateFBXAnimation("IDLE", "_E_BRUTE_IDLE.fbx", { 0.02f,true });
	EnemyRenderer->CreateFBXAnimation("WALK", "_E_BRUTE_WALK.fbx", { 0.02f,false });
	EnemyRenderer->CreateFBXAnimation("RUN", "_E_BRUTE_RUN.fbx", { 0.02f,true });
	EnemyRenderer->CreateFBXAnimation("SLAM", "_E_BRUTE_SLAM.fbx", { 0.02f,false });
	EnemyRenderer->CreateFBXAnimation("SWING", "_E_BRUTE_SWING.fbx", { 0.02f,false });
	EnemyRenderer->CreateFBXAnimation("BREAK", "_E_BRUTE_BREAK.fbx", { 0.02f,false });
	EnemyRenderer->CreateFBXAnimation("THROW", "_E_BRUTE_THROW.fbx", { 0.02f,false });

	EnemyRenderer->ChangeAnimation("IDLE");
}




void EnemyBrute::Start()
{
	EnemyBase::Start();
	GetTransform()->SetLocalScale(float4::ONE * RENDERSCALE_BRUTE);

	// physx
	{
		m_pCapsuleComp = CreateComponent<PhysXCapsuleComponent>();
		m_pCapsuleComp->SetPhysxMaterial(1.f, 1.f, 0.f);
		m_pCapsuleComp->CreatePhysXActors(PHYSXSCALE_BRUTE);
	}
	SetFSMFUNC();
}


void EnemyBrute::Update(float _DeltaTime)
{
	FSMObjectBase::Update(_DeltaTime);

}



void EnemyBrute::AggroMove(float _DeltaTime)
{
	if (false == GetStateChecker())
	{
		m_pCapsuleComp->SetMoveSpeed(AggroDir(m_pCapsuleComp, DEFAULT_DIR_BRUTE) * GRUNT_MOVE_SPEED);
	}
	else
	{
		m_pCapsuleComp->SetMoveSpeed(AggroDir(m_pCapsuleComp, DEFAULT_DIR_BRUTE) * GRUNT_MOVE_SPEED * 2.0f);

	}
}


void EnemyBrute::SetFSMFUNC()
{
	InitFSM(EnemyBruteState::MAX);

	SetChangeFSMCallBack([this]
		{
			//StateDuration = 0.0f;
			//StateChecker = false;
		});



	SetFSM(EnemyBruteState::IDLE,
		[this]
		{
			EnemyRenderer->ChangeAnimation("IDLE");
		},
		[this](float Delta)
		{
			if (true == CheckHit())
			{
				SetNextState(EnemyBruteState::BREAK);
				return;
			}
			if(true == InRangePlayer(2000.0f))
			{
				SetNextState(EnemyBruteState::MOVE);
				return;
			}


		},
		[this]
		{
		}
	);

	SetFSM(EnemyBruteState::MOVE,
		[this]
		{
			EnemyRenderer->ChangeAnimation("WALK");
			AggroDir(m_pCapsuleComp, DEFAULT_DIR_BRUTE);
		},
		[this](float Delta)
		{
			//StateDuration += Delta;
			if (true == CheckHit())
			{
				SetNextState(EnemyBruteState::BREAK);
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
				SetNextState(EnemyBruteState::SWING);
				return;
			}
			if (GetStateDuration() > 5)
			{
				if (true == InRangePlayer(1500.0f))
				{
					SetNextState(EnemyBruteState::THROW);
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

	SetFSM(EnemyBruteState::SLAM,
		[this]
		{
			EnemyRenderer->ChangeAnimation("SLAM");
			AggroDir(m_pCapsuleComp, DEFAULT_DIR_BRUTE);

		},
		[this](float Delta)
		{
			if (true == EnemyRenderer->IsAnimationEnd())
			{
				SetNextState(EnemyBruteState::IDLE);
			}
			//AggroDir(m_pCapsuleComp);

		},
		[this]
		{
		}
	);
	SetFSM(EnemyBruteState::SWING,
		[this]
		{
			EnemyRenderer->ChangeAnimation("SWING");
			AggroDir(m_pCapsuleComp, DEFAULT_DIR_BRUTE);

		},
		[this](float Delta)
		{
			if (true == EnemyRenderer->IsAnimationEnd())
			{
				SetNextState(EnemyBruteState::SLAM);
			}
		},
		[this]
		{
		}
	);

	SetFSM(EnemyBruteState::THROW,
		[this]
		{
			EnemyRenderer->ChangeAnimation("THROW");
			AggroDir(m_pCapsuleComp, DEFAULT_DIR_BRUTE);
		},
		[this](float Delta)
		{
			if (true == EnemyRenderer->IsAnimationEnd())
			{
				SetNextState(EnemyBruteState::IDLE);
			}

		},
		[this]
		{
		}
	);

	SetFSM(EnemyBruteState::BREAK,
		[this]
		{
			EnemyRenderer->ChangeAnimation("BREAK");
		},
		[this](float Delta)
		{
			if (true == EnemyRenderer->IsAnimationEnd())
			{
				SetNextState(EnemyBruteState::IDLE);
			}
		},
		[this]
		{
		}
	);
}