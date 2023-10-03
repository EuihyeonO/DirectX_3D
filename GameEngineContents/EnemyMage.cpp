#include "PreCompileHeader.h"
#include "EnemyMage.h"


EnemyMage::EnemyMage() 
{
}

EnemyMage::~EnemyMage() 
{
}

void EnemyMage::InitAniamtion()
{
	EnemyRenderer = CreateComponent<ContentFBXRenderer>();
	EnemyRenderer->SetFBXMesh("_E_MAGE_MESH.FBX", "ContentAniMeshDeffered");

	EnemyRenderer->CreateFBXAnimation("IDLE", "_E_MAGE_IDLE.fbx", { 0.02f,true });
	EnemyRenderer->CreateFBXAnimation("SHOOT", "_E_MAGE_SHOOT.fbx", { 0.02f,false });
	EnemyRenderer->CreateFBXAnimation("TELEPORT", "_E_MAGE_TELEPORT.fbx", { 0.02f,false });
	EnemyRenderer->CreateFBXAnimation("TELEPORT_IN", "_E_MAGE_TELEPORT.fbx", { 0.02f,false });
	EnemyRenderer->CreateFBXAnimation("DEATH", "_E_MAGE_DEATH.fbx", { 0.02f,false });
	//_E_MAGE_SHOOT_THREE.fbx
	//_E_MAGE_SPIRAL.fbx

	EnemyRenderer->ChangeAnimation("IDLE");
}



void EnemyMage::Start()
{
	EnemyBase::Start();
	GetTransform()->SetLocalScale(float4::ONE * RENDERSCALE_MAGE);

	// physx
	{
		m_pCapsuleComp = CreateComponent<PhysXCapsuleComponent>();
		m_pCapsuleComp->SetPhysxMaterial(1.f, 1.f, 0.f);
		m_pCapsuleComp->CreatePhysXActors(PHYSXSCALE_MAGE);
	}
	SetFSMFUNC();
}

void EnemyMage::Update(float _DeltaTime)
{
	FSMObjectBase::Update(_DeltaTime);
}



void EnemyMage::SetFSMFUNC()
{
	InitFSM(EnemyMageState::MAX);

	SetChangeFSMCallBack([this]
		{

		});



	SetFSM(EnemyMageState::IDLE,
		[this]
		{
			EnemyRenderer->ChangeAnimation("IDLE");
		},
		[this](float Delta)
		{
			if (true == InRangePlayer(1000.0f))
			{
				SetNextState(EnemyMageState::SHOOT);
				return;
			}
		},
		[this]
		{
		}
	);

	SetFSM(EnemyMageState::SHOOT,
		[this]
		{
			EnemyRenderer->ChangeAnimation("SHOOT");
			AggroDir(m_pCapsuleComp);
		},
		[this](float Delta)
		{
			if (true == EnemyRenderer->IsAnimationEnd())
			{
				if (false == InRangePlayer(1000.0f)) // 1500 이상으로 멀어진다면
				{
					SetNextState(EnemyMageState::TELEPORT);
					return;
				}
				SetNextState(EnemyMageState::IDLE);
			}

		},
		[this]
		{
		}
	);

	SetFSM(EnemyMageState::TELEPORT,
		[this]
		{
			EnemyRenderer->ChangeAnimation("TELEPORT");
		},
		[this](float Delta)
		{
			// teleport in  할 위치 선정
			// 임시
			SetNextState(EnemyMageState::TELEPORT_IN);
		},
		[this]
		{
		}
	);

	SetFSM(EnemyMageState::TELEPORT_IN,
		[this]
		{
			EnemyRenderer->ChangeAnimation("TELEPORT_IN");
			AggroDir(m_pCapsuleComp);
		},
		[this](float Delta)
		{
			if (true == EnemyRenderer->IsAnimationEnd())
			{
				SetNextState(EnemyMageState::SHOOT);
				return;
			}
		},
		[this]
		{
		}
	);

	SetFSM(EnemyMageState::DEATH,
		[this]
		{
			EnemyRenderer->ChangeAnimation("DEATH");
		},
		[this](float Delta)
		{
		},
		[this]
		{
		}
	);
}