#pragma once
#include <GameEngineBase/GameEngineNetObject.h>
#include "PlayerDefinition.h"
#include "FSMObjectBase.h"


// ���� :
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
		SKILL,			// ��Ŭ�� Arrow, Arrow_bomb, Arrow_magic, Hookshot, Hookshot_fly
		HOOK_FLY,		// Hookshot_fly
		BASE_ATT,		// ��Ŭ�� Slash_Light_L_new, Slash_Light_R_new
		ROLL,			// �����̽��� Roll, Roll_slash
		ROLL_ATT,		// �����̽���+��Ŭ�� Charge_slam_overhead, Roll_slash_end
		CHARGE_ATT,		// ��Ŭ�� Charge_slash_L, Charge_slash_R
		HIT,			// ���ݴ��� Hit_back, Hit_idle,Hit_Recover
		CLIMB,			// ��ٸ� Climbing_ladder, Climbing_ladder_down, Climbing_off_ladder_top
		LEVER,			// ������ ���� Push_Lever
		ITEM,			// �������� ���� GetItem
		DEAD,			// �ǰ����� ���� ��� Dead
		DROWN,			// �ͻ� Drown
		FALLING,			// ���̰� ���� ���� �� falling, Land
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
	// ���̽����� 3Ÿ�� ���� ������ üũ��
	int AttackStack = 0;
	float StackDuration = 0.0f;
	
	// Direction
	//float4 NextForwardDir = float4::BACK; // �÷��̾ ��ȭ �� ����
	float4 ForwardDir = float4::BACK; // �÷��̾ �ٶ󺸴� ����
	float4 MoveDir = float4::FORWARD; // �÷��̾ �������� ������ ����
	void DirectionUpdate(float _DeltaTime);
	float4 GetMousDirection();
	 
	
	// State Controll
	float StateInputDelayTime = 0.0f;

	void CheckFalling();
	void CheckDirInput(float _DeltaTime);
	void CheckStateInput(float _DeltaTime);
	void CheckState(float _DeltaTime);

	void CheckClimbInput(float _DeltaTime);
	void MoveUpdate(float _MoveVec, float4 _Dir = float4::ZERONULL);	//MoveDir�� �ش��ϴ� ���� ó���ϱ� ������



	void DefaultPhysX();

	


};

