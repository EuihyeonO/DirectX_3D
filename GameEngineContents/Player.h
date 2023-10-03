#pragma once
#include <GameEngineBase/GameEngineNetObject.h>
#include "PlayerDefinition.h"
#include "FSMObjectBase.h"


// 설명 :
class Player : public FSMObjectBase, public GameEngineNetObject
{
public:
	static Player* MainPlayer;
public:
	// constrcuter destructer
	Player();
	~Player();

	// delete Function
	Player(const Player& _Other) = delete;
	Player(Player&& _Other) noexcept = delete;
	Player& operator=(const Player& _Other) = delete;
	Player& operator=(Player&& _Other) noexcept = delete;

	std::shared_ptr<class PhysXCapsuleComponent> GetPhysXComponent() const
	{
		return m_pCapsuleComp;
	}

protected:
	void Start() override;
	void Update(float _DeltaTime) override;

	void UserUpdate(float _DeltaTime);
	void ServerUpdate(float _DeltaTime);

private:

	// Struct
	enum class PlayerState
	{
		IDLE,			// Idle_0, Idle_1
		WALK,			// Walk, Run
		SKILL,			// 우클릭 Arrow, Arrow_bomb, Arrow_magic, Hookshot, Hookshot_fly
		HOOK_FLY,		// Hookshot_fly
		BASE_ATT,		// 좌클릭 Slash_Light_L_new, Slash_Light_R_new
		ROLL,			// 스페이스바 Roll, Roll_slash
		ROLL_ATT,		// 스페이스바+휠클릭 Charge_slam_overhead, Roll_slash_end
		CHARGE_ATT,		// 휠클릭 Charge_slash_L, Charge_slash_R
		HIT,			// 공격당함 Hit_back, Hit_idle,Hit_Recover
		CLIMB,			// 사다리 Climbing_ladder, Climbing_ladder_down, Climbing_off_ladder_top
		LEVER,			// 레버를 누름 Push_Lever
		ITEM,			// 아이템을 얻음 GetItem
		DEAD,			// 피격으로 인한 사망 Dead
		DROWN,			// 익사 Drown
		FALLING,			// 높이가 차가 있을 때 falling, Land
		MAX,
	};

	enum class PlayerSkill
	{
		ARROW,
		MAGIC,
		BOMB,
		HOOK,
		MAX,
	};




	// Init
	std::atomic_int AnimationLoadCount = 0;
	void InitInputKey();
	void InitPlayerAnimation();
	void SetFSMFunc();

	// Component
		// Render
	std::shared_ptr<ContentFBXRenderer> Renderer = nullptr;
		//physx
	std::shared_ptr<class PhysXCapsuleComponent> m_pCapsuleComp = nullptr;
		//Range
	std::shared_ptr<GameEngineActor> AttackRange = nullptr;

		// Attack
	PlayerSkill CurSkill = PlayerSkill::ARROW;
	void SetSkill();
	bool isRightAttack = true;
	// 베이스어택 3타에 대한 딜레이 체크용
	int AttackStack = 0;
	float StackDuration = 0.0f;
	
	// Direction
	//float4 NextForwardDir = float4::BACK; // 플레이어가 변화 할 방향
	float4 ForwardDir = float4::BACK; // 플레이어가 바라보는 방향
	float4 MoveDir = float4::FORWARD; // 플레이어가 다음으로 움직일 방향
	void DirectionUpdate(float _DeltaTime);
	float4 GetMousDirection();
	 
	
	// State Controll
	float StateInputDelayTime = 0.0f;

	void CheckFalling();
	void CheckDirInput(float _DeltaTime);
	void CheckStateInput(float _DeltaTime);
	void CheckState(float _DeltaTime);

	void CheckClimbInput(float _DeltaTime);
	void MoveUpdate(float _MoveVec, float4 _Dir = float4::ZERONULL);	//MoveDir에 해당하는 값만 처리하기 때문에



	void DefaultPhysX();

	


};

