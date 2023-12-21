#include "PreCompileHeader.h"

#include "Boss_OldCrowEgg.h"

#include "Boss_OldCrow.h"

void Boss_OldCrow::InitAnimation()
{
	EnemyRenderer = CreateComponent<ContentFBXRenderer>();

	EnemyRenderer->SetFBXMesh("OldCrow.FBX", "ContentAniMeshDeffered");
	EnemyRenderer->CreateFBXAnimation("Idle", "OldCrow_Idle_Anim.FBX", { 0.033f, true });
	EnemyRenderer->ShadowOn();

	EnemyRenderer->CreateFBXAnimation("DashStart", "OldCrow_DashStart_Anim.FBX", { 0.02f, false });
	EnemyRenderer->CreateFBXAnimation("Dash", "OldCrow_Dash_Anim.FBX", { 0.033f, true });
	EnemyRenderer->SetAnimationStartFunc("Dash", 3, [this]
		{
			CurrentSpeed = BOSS_OLDCROW_DASHSPEED;
			GameEngineSound::Play("OldCrow_Walk.mp3");
		});
	EnemyRenderer->SetAnimationStartFunc("Dash", 5, [this]
		{
			CurrentSpeed = BOSS_OLDCROW_DASHSPEED;
			GameEngineSound::Play("OldCrow_Walk2.mp3");
		});
	EnemyRenderer->SetAnimationStartFunc("Dash", 17, [this]
		{
			CurrentSpeed = BOSS_OLDCROW_DASHSPEED2;
		});
	EnemyRenderer->CreateFBXAnimation("TurnLeft", "OldCrow_DashTurnL_Anim.FBX", { 0.033f, false });
	EnemyRenderer->SetAnimationStartFunc("TurnLeft", 3, [this]
		{
			IsTurn = true;
		});
	EnemyRenderer->SetAnimationStartFunc("TurnLeft", 15, [this]
		{
			IsTurn = false;
		});
	EnemyRenderer->CreateFBXAnimation("MegaDashPrep", "OldCrow_MegaDashPrep_Anim.FBX", { 0.033f, false });
	EnemyRenderer->CreateFBXAnimation("MegaDash", "OldCrow_MegaDash_Anim.FBX", { 0.033f, true });

	EnemyRenderer->CreateFBXAnimation("Jump", "OldCrow_Jump_Anim.FBX", { 0.033f, false });
	EnemyRenderer->CreateFBXAnimation("Slam", "OldCrow_Slam_Anim.FBX", { 0.033f, false });
	EnemyRenderer->CreateFBXAnimation("SlamImpact", "OldCrow_SlamImpact_Anim.FBX", { 0.008f, false });

	EnemyRenderer->CreateFBXAnimation("Egg", "OldCrow_Egg_Anim.FBX", { 0.033f, false });
	EnemyRenderer->SetAnimationStartFunc("Egg", 80, [this]
		{
			std::shared_ptr<GameEngineComponent> BonePivot = CreateComponent<GameEngineComponent>();
			BonePivot->GetTransform()->SetParent(GetTransform());
			BonePivot->GetTransform()->SetLocalPosition(float4{ 0, 14, 0.5 });

			float Value = 5.0f;

			BonePivot->GetTransform()->AddLocalRotation({ -70.0f, 0.0f, 0.0f });

			float4 EggDir = float4::ZERO;
			
			std::shared_ptr<Boss_OldCrowEgg> CrowEgg = GetLevel()->CreateActor<Boss_OldCrowEgg>();
			CrowEgg->SetCrowEgg(BonePivot->GetTransform()->GetWorldPosition(), BonePivot->GetTransform()->GetWorldRotation(), BonePivot->GetTransform()->GetWorldPosition() - GetTransform()->GetWorldPosition());

			GameEngineSound::Play("OldCrow_Landing.mp3");
		});

	EnemyRenderer->CreateFBXAnimation("Scream", "OldCrow_Scream_Anim.FBX", { 0.033f, false });

	EnemyRenderer->SetAnimationStartFunc("Scream", 0, [this]
		{
			StateCalTime = 0.0f;
			GameEngineSound::Play("OldCrow_IntroBGM.mp3");
		});
	EnemyRenderer->SetAnimationStartFunc("Scream", 55, [this]
		{
			GameEngineSound::Play("OldCrow_Scream.mp3");
		});
	//EnemyRenderer->SetAnimationStartFunc("Scream", 99, [this]
	//	{
	//		IntroDone = true;
	//	});

	EnemyRenderer->CreateFBXAnimation("ScreamMini", "OldCrow_ScreamMini_Anim.FBX", { 0.033f, false });
	EnemyRenderer->SetAnimationStartFunc("ScreamMini", 18, [this]
		{
			StateCalBool = true;
		});

	EnemyRenderer->CreateFBXAnimation("DeathInRunning", "OldCrow_DeathWhileRunning_Anim.FBX", { 0.033f, false });
	EnemyRenderer->CreateFBXAnimation("DeathInUpright", "OldCrow_DeathWhileUpright_Anim.FBX", { 0.033f, false });

	GetTransform()->SetLocalScale(float4::ONE * 50.0f);
	EnemyRenderer->GetTransform()->SetLocalRotation({ 0, -90, 0 });

	EnemyRenderer->ChangeAnimation("Scream");

}