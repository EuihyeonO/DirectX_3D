#pragma once

#include <GameEngineCore/GameEngineComponent.h>
#include "PhysXDefault.h"

// ���� :
class PhysXBoxComponent : public GameEngineComponent, public PhysXDefault
{
public:
	// constrcuter destructer
	PhysXBoxComponent();
	~PhysXBoxComponent();

	// delete Function
	PhysXBoxComponent(const PhysXBoxComponent& _Other) = delete;
	PhysXBoxComponent(PhysXBoxComponent&& _Other) noexcept = delete;
	PhysXBoxComponent& operator=(const PhysXBoxComponent& _Other) = delete;
	PhysXBoxComponent& operator=(PhysXBoxComponent&& _Other) noexcept = delete;

	void CreatePhysXActors(physx::PxScene* _Scene, physx::PxPhysics* _physics, physx::PxVec3 _GeoMetryScale = physx::PxVec3(2.0f), float4 _GeoMetryRot = float4::ZERO);

	inline void ReleasePhysX()
	{
		if (true == m_pRigidDynamic->isReleasable())
		{
			m_pRigidDynamic->release();
			ParentActor.lock()->Death();
		}
	}

	inline void SetPositionSetFromParentFlag(bool _Flag)
	{
		PositionSetFromParentFlag = _Flag;
	}

protected:
	void Start() override;
	void Update(float _DeltaTime) override;

private:
	// Phys���� ������ �ʿ��� ����
	//physx::PxPhysics* physics_;
	//physx::PxScene* scene_;

	physx::PxMaterial* m_pMaterial;
	physx::PxShape* m_pShape;

	// �� ������Ʈ�� ������ �ִ� Parent�� ���� ����
	std::weak_ptr<GameEngineActor> ParentActor;

	bool PositionSetFromParentFlag = false;
};
