#include "PrecompileHeader.h"
#include "PhysXSphereComponent.h"


PhysXSphereComponent::PhysXSphereComponent()
{
}

PhysXSphereComponent::~PhysXSphereComponent()
{
}

physx::PxRigidDynamic* PhysXSphereComponent::CreatePhysXActors(physx::PxScene* _Scene, physx::PxPhysics* _physics, physx::PxVec3 _GeoMetryScale, float4 _GeoMetryRotation)
{
	m_pScene = _Scene;
	m_pPhysics = _physics;

	float4 tmpQuat = _GeoMetryRotation.EulerDegToQuaternion();

	// 부모 액터로부터 위치 생성
	physx::PxTransform localTm
	(
		physx::PxVec3
		(
			ParentActor.lock()->GetTransform()->GetWorldPosition().x
			, ParentActor.lock()->GetTransform()->GetWorldPosition().y
			, ParentActor.lock()->GetTransform()->GetWorldPosition().z
		),
		physx::PxQuat(tmpQuat.x, tmpQuat.y, tmpQuat.z, tmpQuat.w)
	);

	// Staticfriction : 정적마찰 // Dynamicfriction : 동적마찰 // Resitution : 탄성계수
	m_pMaterial = _physics->createMaterial(m_fStaticFriction, m_fDynamicFriction, m_fResitution);

	// TODO::배율을 적용할 경우 이쪽 코드를 사용
	//float4 tmpMagnification = { SIZE_MAGNIFICATION_RATIO };
	//physx::PxVec3 tmpGeoMetryScale(_GeoMetryScale.x * tmpMagnification.x * 0.5f, 
	//							   _GeoMetryScale.y * tmpMagnification.y * 0.5f, 
	//							   _GeoMetryScale.z * tmpMagnification.z * 0.5f);

	//GeoMetryScale = _GeoMetryScale;

	physx::PxVec3 tmpGeoMetryScale
	(
		_GeoMetryScale.x * 0.5f,
		_GeoMetryScale.y * 0.5f,
		_GeoMetryScale.z * 0.5f
	);

	// 충돌체의 종류
	m_pRigidDynamic = _physics->createRigidDynamic(localTm);
	// 특정 축을 따라/주위로 동작을 잠그는 메커니즘을 제공하는 플래그 모음
	m_pRigidDynamic->setRigidDynamicLockFlags
	(
		physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_X |
		physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y |
		physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z
	);

	// TODO::점프속력에 영향을 미침. 스테이지 작성후 자세한 수치는 나중에 조절
	// 플레이어 최대 속력
	// dynamic_->setMaxLinearVelocity(PLAYER_MAX_SPEED);

	float ScaledRadius = tmpGeoMetryScale.z;
	float ScaledHeight = tmpGeoMetryScale.y;

	// 충돌체의 형태
	// 충돌체의 크기는 절반의 크기를 설정하므로 실제 Renderer의 스케일은 충돌체의 2배로 설정되어야 함
	// TODO::부모 액터의 RenderUnit으로부터 Mesh의 Scale 과 WorldScale의 연산의 결과를 지오메트리의 Scale로 세팅해야함.
	m_pShape = physx::PxRigidActorExt::createExclusiveShape(*m_pRigidDynamic,physx::PxSphereGeometry(physx::PxReal(_GeoMetryScale.y*0.5f)), *m_pMaterial);

	m_pShape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, true);
	// RigidDynamic의 밀도를 설정
	physx::PxRigidBodyExt::updateMassAndInertia(*m_pRigidDynamic, 0.01f);

	//피벗 설정
	physx::PxVec3 DynamicCenter = physx::PxVec3{ 0.0f, ScaledHeight, 0.0f };
	physx::PxTransform relativePose(physx::PxQuat(physx::PxHalfPi, physx::PxVec3(0, 0, 1)));
	relativePose.p = DynamicCenter;
	m_pShape->setLocalPose(relativePose);

	//충돌할때 필요한 필터 데이터
	//m_pShape->setSimulationFilterData
	//(
	//	physx::PxFilterData
	//	(
	//		static_cast<physx::PxU32>(PhysXFilterGroup::Obstacle),
	//		static_cast<physx::PxU32>(PhysXFilterGroup::PlayerDynamic), 
	//		0,
	//		0
	//	)
	//);
	//m_pShape->setSimulationFilterData(physx::PxFilterData(static_cast<physx::PxU32>(PhysXFilterGroup::PlayerDynamic), static_cast<physx::PxU32>(PhysXFilterGroup::Ground), static_cast<physx::PxU32>(PhysXFilterGroup::Obstacle), 0));

	m_pShape->setContactOffset(0.2f);

	// 제동?
	//m_pRigidDynamic->setLinearDamping(physx::PxReal(0.01f));
	//m_pRigidDynamic->setMaxAngularVelocity(physx::PxReal(20.0f));
	//m_pRigidDynamic->setAngularDamping(physx::PxReal(2.0f));

	// Scene에 액터 추가
	_Scene->addActor(*m_pRigidDynamic);

	// TODO::virtualPhysXLevel에서 Callback 함수에 호출하기 위한 Dynamic 리턴
	return m_pRigidDynamic;
}

void PhysXSphereComponent::SetMoveSpeed(float4 _MoveSpeed)
{
	// RigidDynamic의 축을 고정하는 Flag -> 캐릭터가 쓰러지지 않고 서있을 수 있도록
	// 무언가와 충돌해서 쓰러져야 할경우에는 setRigidDynamicLockFlag({flag}, false)로 flag를 해제해야함.
	m_pRigidDynamic->setRigidDynamicLockFlags(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_X | physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y | physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z);
	//m_pRigidDynamic->clearForce();
	// 캐릭터의 방향을 힘으로 조절
	m_pRigidDynamic->addForce(physx::PxVec3(_MoveSpeed.x, _MoveSpeed.y, _MoveSpeed.z), physx::PxForceMode::eACCELERATION);
}


void PhysXSphereComponent::SetDynamicIdle()
{
	// 고정된 축을 해제
	m_pRigidDynamic->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_X, false);
	m_pRigidDynamic->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y, false);
	m_pRigidDynamic->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z, false);

	// Kinematic을 사용했을 경우, RigidDynamic으로 돌아갈 수 있도록 Flag해제
	//dynamic_->setRigidBodyFlag(physx::PxRigidBodyFlag::eKINEMATIC, false);
}

void PhysXSphereComponent::Start()
{
	// 부모의 정보의 저장
	ParentActor = GetActor()->DynamicThis<GameEngineActor>();
}

void PhysXSphereComponent::Update(float _DeltaTime)
{
	if (!(physx::PxIsFinite(m_pRigidDynamic->getGlobalPose().p.x) || physx::PxIsFinite(m_pRigidDynamic->getGlobalPose().p.y) || physx::PxIsFinite(m_pRigidDynamic->getGlobalPose().p.z))
		&& true == IsMain)
	{
		m_pRigidDynamic->setGlobalPose(RecentTransform);
	}

	// PhysX Actor의 상태에 맞춰서 부모의 Transform정보를 갱신
	float4 tmpWorldPos = { m_pRigidDynamic->getGlobalPose().p.x, m_pRigidDynamic->getGlobalPose().p.y, m_pRigidDynamic->getGlobalPose().p.z };
	float4 EulerRot = PhysXDefault::GetQuaternionEulerAngles(m_pRigidDynamic->getGlobalPose().q) * GameEngineMath::RadToDeg;

	ParentActor.lock()->GetTransform()->SetWorldRotation(float4{ EulerRot.x, EulerRot.y, EulerRot.z });
	ParentActor.lock()->GetTransform()->SetWorldPosition(tmpWorldPos);

	if (m_bSpeedLimit == true)
	{
		SpeedLimit();
	}
}

void PhysXSphereComponent::PushImpulse(float4 _ImpulsePower)
{
	// 고정된 축을 해제
	m_pRigidDynamic->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_X, false);
	m_pRigidDynamic->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y, false);
	m_pRigidDynamic->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z, false);

	m_pRigidDynamic->addForce(physx::PxVec3(_ImpulsePower.x, _ImpulsePower.y, _ImpulsePower.z), physx::PxForceMode::eIMPULSE);
}

void PhysXSphereComponent::PushImpulseAtLocalPos(float4 _ImpulsePower, float4 _Pos)
{
	// 고정된 축을 해제
	m_pRigidDynamic->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_X, false);
	m_pRigidDynamic->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z, false);
	physx::PxRigidBodyExt::addForceAtPos(*m_pRigidDynamic, physx::PxVec3(_Pos.x, _Pos.y * 0.9f, _Pos.z),
		physx::PxVec3(_ImpulsePower.x, _ImpulsePower.y, _ImpulsePower.z), physx::PxForceMode::eIMPULSE, true);
}

void PhysXSphereComponent::SpeedLimit()
{
	physx::PxVec3 Velo = m_pRigidDynamic->getLinearVelocity();
	physx::PxVec2 Velo2D(Velo.x, Velo.z);

	if (Velo2D.magnitude() > PLAYER_MAX_SPEED)
	{
		Velo2D.normalize();
		Velo2D *= PLAYER_MAX_SPEED;
		Velo.x = Velo2D.x;
		Velo.z = Velo2D.y;

		m_pRigidDynamic->setLinearVelocity(Velo);
	}
}

void PhysXSphereComponent::SetChangedRot(float4 _Rot)
{
	//dynamic_->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y, false);

	float4 tmpQuat = _Rot.EulerDegToQuaternion();
	const physx::PxQuat tmpPxQuat(tmpQuat.x, tmpQuat.y, tmpQuat.z, tmpQuat.w);
	const physx::PxTransform tmpTansform(m_pRigidDynamic->getGlobalPose().p, tmpPxQuat);

	m_pRigidDynamic->setGlobalPose(tmpTansform);
}

//플레이어 멈추는 함수
void PhysXSphereComponent::FreezeDynamic()
{
	m_pRigidDynamic->putToSleep();
}

void PhysXSphereComponent::WakeUpDynamic()
{
	m_pRigidDynamic->wakeUp();
}

void PhysXSphereComponent::ResetDynamic()
{
	float4 tmpQuat = float4{ 0.0f,0.0f,0.0f }.EulerDegToQuaternion();
	const physx::PxQuat tmpPxQuat(tmpQuat.x, tmpQuat.y, tmpQuat.z, tmpQuat.w);
	const physx::PxTransform tmpTansform(m_pRigidDynamic->getGlobalPose().p, tmpPxQuat);

	m_pRigidDynamic->setGlobalPose(tmpTansform);
	m_pRigidDynamic->setRigidDynamicLockFlags(
		physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_X |
		physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y |
		physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z);
}


