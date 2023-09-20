#pragma once

#include <GameEngineCore/GameEngineComponent.h>
#include "PhysXDefault.h"

// 설명 :
class PhysXTriangleComponent : public GameEngineComponent, public PhysXDefault
{
public:
	// constrcuter destructer
	PhysXTriangleComponent();
	~PhysXTriangleComponent();

	// delete Function
	PhysXTriangleComponent(const PhysXTriangleComponent& _Other) = delete;
	PhysXTriangleComponent(PhysXTriangleComponent&& _Other) noexcept = delete;
	PhysXTriangleComponent& operator=(const PhysXTriangleComponent& _Other) = delete;
	PhysXTriangleComponent& operator=(PhysXTriangleComponent&& _Other) noexcept = delete;
	
	void CreatePhysXActors(const std::string& _MeshName,bool _InverseIndex = true, float _Ratio = 1.f, float4 _GeoMetryRot = { 0.0f, 0.0f });


protected:
	void Start() override;
	void Update(float _DeltaTime) override;

	void CustomFBXLoad(const std::string& _MeshName,float _Ratio, bool _InverseIndex);

private:
	// Phys액터 생성에 필요한 정보
	physx::PxCooking* m_pCooking = nullptr;


	physx::PxConvexMesh* m_pConvexMesh;

	std::vector<std::vector<physx::PxVec3>> VertexVec;
	std::vector<std::vector<physx::PxU32>> IndexVec;
	std::shared_ptr<class GameEngineFBXMesh> Mesh;


};

