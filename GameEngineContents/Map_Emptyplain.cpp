#include "PrecompileHeader.h"
#include "Map_Emptyplain.h"

#include "PhysXTriangleComponent.h"
#include "PhysXBoxComponent.h"

Map_Emptyplain::Map_Emptyplain()
{
}

Map_Emptyplain::~Map_Emptyplain()
{
}

void Map_Emptyplain::Start()
{
	// ������Ʈ �ʱ�ȭ 
	InitComponent();
}

void Map_Emptyplain::Update(float _DeltaTime)
{
}

void Map_Emptyplain::InitComponent()
{
	// �� ������ 
	m_pRenderer = CreateComponent<GameEngineFBXRenderer>();
	m_pRenderer->SetFBXMesh("BigCrow_Floor_Navi.fbx", "MeshTexture");

	auto Unit = m_pRenderer->GetAllRenderUnit();
	Unit[0][0]->ShaderResHelper.SetTexture("DiffuseTexture", "CenterLevelBG.png");

	// �׺�޽� ��ġȮ�ο� ������ 
	/*m_pNaviRenderer = CreateComponent<GameEngineFBXRenderer>();
	m_pNaviRenderer->SetFBXMesh("Map_Office_Navi.fbx", "MeshTexture");*/


	// float4 UnitScale = m_pRenderer->GetFBXMesh()->GetRenderUnit(0)->BoundScaleBox;
	// float4 scale = m_pRenderer->GetMeshScale();
	// physx::PxVec3 vscale = UnitScale.PhysXVec3Return();

	// ������Ʈ 
	m_pTriangleComp = CreateComponent<PhysXTriangleComponent>();
	m_pTriangleComp->SetPhysxMaterial(0.f, 0.f, 0.f);
	m_pTriangleComp->CreatePhysXActors("BigCrow_Floor_Navi.fbx", true);
	//m_pTriangleComp->GetStatic()->setGlobalPose(float4::PhysXTransformReturn(float4::ZERO, float4{ 10000, 0, 10000 }));

}
