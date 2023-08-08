#pragma once

#include "ThirdParty/PhysX_5/inc/PxPhysicsAPI.h"
//#include "ThirdParty/PhysX_5/inc/extensions/PxDefaultAllocator.h"
//#include "ThirdParty/PhysX_5/inc/extensions/PxDefaultErrorCallback.h"
// Ό³Έν :
class GameEnginePhysics
{
public:
	// constrcuter destructer
	GameEnginePhysics();
	~GameEnginePhysics();

	// delete Function
	GameEnginePhysics(const GameEnginePhysics& _Other) = delete;
	GameEnginePhysics(GameEnginePhysics&& _Other) noexcept = delete;
	GameEnginePhysics& operator=(const GameEnginePhysics& _Other) = delete;
	GameEnginePhysics& operator=(GameEnginePhysics&& _Other) noexcept = delete;

	static void Initialize();
	//static void Render(std::shared_ptr<class GameEngineLevel> Level, float _DeltaTime);
	static void Release();
	
	//static physx::PxDefaultErrorCallback gErrorCallback;
	//static physx::PxDefaultAllocator gAllocator;
	//static physx::PxPvd* Pvd;
	//static physx::PxFoundation* foundation;
	//static physx::PxPhysics* physics;
	//static physx::PxMaterial* material;
	//static physx::PxScene* scene;

protected:

private:
	static physx::PxDefaultErrorCallback m_ErrorCallback;
	static physx::PxDefaultAllocator m_Allocator;

	static physx::PxFoundation* m_pFoundation;
	static physx::PxPhysics* m_pPhysics;

	static physx::PxScene* m_pScene;

};

