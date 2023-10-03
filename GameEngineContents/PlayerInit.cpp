#include "PreCompileHeader.h"
#include "Player.h"



void Player::InitInputKey()
{
	if (false == GameEngineInput::IsKey("PlayerLeft"))
	{
		GameEngineInput::CreateKey("PlayerLeft", 'A');
		GameEngineInput::CreateKey("PlayerRight", 'D');
		GameEngineInput::CreateKey("PlayerUp", 'W');
		GameEngineInput::CreateKey("PlayerDown", 'S');
		GameEngineInput::CreateKey("PlayerRoll", VK_SPACE);
		GameEngineInput::CreateKey("PlayerLBUTTON", VK_LBUTTON);
		GameEngineInput::CreateKey("PlayerRBUTTON", VK_RBUTTON);
		GameEngineInput::CreateKey("PlayerMBUTTON", VK_MBUTTON);
		GameEngineInput::CreateKey("PlayerSkillArrow",	'1');
		GameEngineInput::CreateKey("PlayerSkillMagic",	'2');
		GameEngineInput::CreateKey("PlayerSkillBomb",	'3');
		GameEngineInput::CreateKey("PlayerSkillHook",	'4');
	}
	if (false == GameEngineInput::IsKey("PressN"))
	{
		GameEngineInput::CreateKey("PressN", 'N');
	}
}




void Player::InitPlayerAnimation()
{
	Renderer = CreateComponent<ContentFBXRenderer>();
	Renderer->SetFBXMesh("PLAYER_MESH.FBX", "ContentAniMeshDeffered");
	
	auto units = Renderer->GetAllRenderUnit();
	units[0][1]->Mask.UV_MaskingValue = 1.0f;


	//idle
	Renderer->CreateFBXAnimation("IDLE0", "PLAYER_IDLE_0.FBX", { 0.02f,false });
	Renderer->CreateFBXAnimation("IDLE1", "PLAYER_IDLE_1.FBX", { 0.01f,false });

	//skill
	Renderer->CreateFBXAnimation("ARROW", "PLAYER_ARROW.FBX", { 0.01f,false });
	Renderer->CreateFBXAnimation("BOMB", "PLAYER_ARROW_BOMB.FBX", { 0.01f,false });
	Renderer->CreateFBXAnimation("BOMB_END", "PLAYER_ARROW_BOMB_END.FBX", { 0.01f,false });
	Renderer->CreateFBXAnimation("MAGIC", "PLAYER_ARROW_MAGIC.FBX", { 0.01f,false });
	Renderer->CreateFBXAnimation("HOOK", "PLAYER_HOOK_SHOT.FBX", { 0.01f,false });
	Renderer->CreateFBXAnimation("HOOKE_FLY", "PLAYER_HOOKSHOT_FLY.FBX", { 0.01f,true });

	// move
	Renderer->CreateFBXAnimation("WALK", "PLAYER_WALK.FBX", { 0.02f,false ,0,5});
	Renderer->CreateFBXAnimation("RUN", "PLAYER_RUN.FBX", { 0.02f,true });

	//roll
	Renderer->CreateFBXAnimation("ROLL", "PLAYER_ROLL.FBX", { 0.02f,false });
	Renderer->CreateFBXAnimation("ROLL_SLASH", "PLAYER_ROLL_SLASH.FBX", { 0.01f,false });
	//Renderer->CreateFBXAnimation("Charge_slam_overhead", "Crow_Player_Charge_slam_overhead.FBX", { 0.01f,false });// 구르기 마우스 휠
	Renderer->CreateFBXAnimation("ROLL_SLASH_END", "PLAYER_ROLL_SLASH_END.FBX", { 0.01f,false });

	//hit 공격당함
	Renderer->CreateFBXAnimation("HIT_BACK", "PLAYER_HIT_BACK.FBX", { 0.01f,false });
	Renderer->CreateFBXAnimation("HIT_IDLE", "PLAYER_HIT_IDLE.FBX", { 0.01f,true });
	Renderer->CreateFBXAnimation("HIT_RECOVER", "PLAYER_HIT_RECOVER.FBX", { 0.01f,false });

	//climb
	Renderer->CreateFBXAnimation("CLIMBING_LADDER", "PLAYER_CLIMBING_LADDER.FBX", { 0.02f,true });
	Renderer->CreateFBXAnimation("CLIMBING_LADDER_DOWN", "PLAYER_CLIMBING_LADDER_DOWN.FBX", { 0.02f,true });
	Renderer->CreateFBXAnimation("CLIMBING_OFF_LADDER_TOP", "PLAYER_CLIMBING_OFF_LADDER_TOP.FBX", { 0.02f,false });

	//attack
		//left btn
	Renderer->CreateFBXAnimation("SLASH_LIGHT_L", "PLAYER_SLASH_LIGHT_R.FBX", { 0.02f,false }); // 일반 좌클릭
	Renderer->CreateFBXAnimation("SLASH_LIGHT_R", "PLAYER_SLASH_LIGHT_L.FBX", { 0.02f,false }); // 일반 좌클릭
	// mid btn
	Renderer->CreateFBXAnimation("CHARGE_SLASH_L", "PLAYER_CHARGE_SLASH_L.FBX", { 0.01f,false }); 
	Renderer->CreateFBXAnimation("CHARGE_SLASH_R", "PLAYER_CHARGE_SLASH_R.FBX", { 0.01f,false }); 
	Renderer->CreateFBXAnimation("CHARGE_MAX_L", "PLAYER_CHARGE_MAX_L.FBX", { 0.01f,true }); 
	Renderer->CreateFBXAnimation("CHARGE_MAX_R", "PLAYER_CHARGE_MAX_R.FBX", { 0.01f,true });

	//interaction
	Renderer->CreateFBXAnimation("PUSH_LEVER", "PLAYER_PUSH_LEVER.FBX", { 0.01f,false });
	Renderer->CreateFBXAnimation("GETITEM", "PLAYER_GETITEM.FBX", { 0.01f,false }); // 체크필요

	// Dead
	Renderer->CreateFBXAnimation("Dead", "PLAYER_DEAD.FBX", { 0.01f,true });
	Renderer->CreateFBXAnimation("DROWN", "PLAYER_DROWN.FBX", { 0.01f,true }); //물에 빠짐

	// falling & land
	Renderer->CreateFBXAnimation("FALLING", "PLAYER_FALLING.FBX", { 0.01f,true }); // 체크필요
	Renderer->CreateFBXAnimation("LAND", "PLAYER_LAND.FBX", { 0.01f,false }); // 착지


	// 추가 애니메이션
	//Renderer->CreateFBXAnimation("21", "PLAYER_IDLE_FEAR.fbx");
	//Renderer->CreateFBXAnimation("22", "PLAYER_INJURED_FALL.fbx");
	//Renderer->CreateFBXAnimation("23", "PLAYER_INJURED_GETUP.fbx");
	//Renderer->CreateFBXAnimation("24", "PLAYER_INJURED_LAND.fbx");
	//Renderer->CreateFBXAnimation("26", "PLAYER_PLUNGE.fbx");
	//Renderer->CreateFBXAnimation("27", "PLAYER_PLUNGE_LAND.fbx");
	//Renderer->CreateFBXAnimation("36", "PLAYER_SNEAK.fbx");
	//Renderer->CreateFBXAnimation("30", "PLAYER_ROLL_HEAVY.fbx");

	//std::shared_ptr<GameEngineActor> Pivot = GetLevel()->CreateActor<GameEngineActor>();
	//Pivot->GetTransform()->SetParent(GetTransform());
	//Pivot->GetTransform()->SetLocalPosition(float4::FORWARD * 10.0f);
	//Renderer->GetTransform()->SetParent(Pivot->GetTransform());
	Renderer->GetTransform()->SetLocalPosition(float4::BACK * 10.0f);
	GetTransform()->SetLocalScale(float4::ONE * PlayerScaleRatio);
}


