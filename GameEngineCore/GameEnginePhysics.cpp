#include "PrecompileHeader.h"
#include "GameEnginePhysics.h"


physx::PxDefaultErrorCallback	GameEnginePhysics::m_ErrorCallback;
physx::PxDefaultAllocator		GameEnginePhysics::m_Allocator;
physx::PxFoundation*			GameEnginePhysics::m_pFoundation  =nullptr;
physx::PxPhysics*				GameEnginePhysics::m_pPhysics     =nullptr;
physx::PxScene*					GameEnginePhysics::m_pScene       =nullptr;

GameEnginePhysics::GameEnginePhysics()
{
}

GameEnginePhysics::~GameEnginePhysics()
{
}

void GameEnginePhysics::Initialize()
{
	m_pFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, m_Allocator, m_ErrorCallback);
	m_pPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *m_pFoundation, physx::PxTolerancesScale());
	
	physx::PxSceneDesc sceneDesc(m_pPhysics->getTolerancesScale());
	sceneDesc.gravity = physx::PxVec3(0.0f, -9.81f, 0.0f); // 중력 설정
	m_pScene = m_pPhysics->createScene(sceneDesc);
}
//void GameEnginePhysics::Render(std::shared_ptr<class GameEngineLevel> Level, float _DeltaTime)
//{
//	physx::PxRigidDynamic* box = physx::PxCreateDynamic(*mphysics, physx::PxTransform(physx::PxVec3(0, 10, 0)), physx::PxBoxGeometry(1, 1, 1), *material, 10.0f);
//
//	scene->addActor(*box);
//	scene->simulate(_DeltaTime);
//	scene->fetchResults(true);
//}

void GameEnginePhysics::Release()
{
	if (m_pScene !=nullptr)
	{
		m_pScene->release();
	}
	if (m_pPhysics != nullptr)
	{
		m_pPhysics->release();
	}
	if (m_pFoundation != nullptr)
	{
		m_pFoundation->release();
	}
}